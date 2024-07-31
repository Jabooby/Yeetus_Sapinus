#include <classes.h>
#include <Arduino.h>
#include <communication.h>


//CONSTANTES globales
const int posHome = 0;
const int posBac = 400;
const int hysX = 10; // Hysteresis du deplacement X
const int hysQ = 7;  // Hysteresis de langle

//var globales
double SpeedX = 0;
double SpeedQ = 0;
bool QEnable = false;
bool XEnable = false;
unsigned long time;
bool setup_angle = false;
double offset_angle = 0;
unsigned long delais_communication = 0;

//variables qui sont reçues
char state[10] = "Stop"; // ="start" ou ="stop"
 
//debug variables
unsigned long temps_print = 0;

//var globales pour switch case
bool caseFirstScan = true;
unsigned long caseStartTime[20];
int caseActif = 21;
//Timer pour case stabiisation
unsigned long temps_stabilisation = 0;


// Fonctions pour le PID
double PIDmeasurement();
void PIDcommand(double cmd);
void PIDgoalReached();
double getPositionY();
void uptadeY(double commande);
void PIDYgoalReached();

double getPositionX();
void uptadeX(double commande);
void PIDXgoalReached();

double GetAngle();
void uptadeQ(double commande);
void PIDQgoalReached();

// Creation objets
ArduinoX* AX_ = new ArduinoX;                       // objet arduinoX
MegaServo* servo_ = new MegaServo;                   // objet servomoteur

PID* pidX_ = new PID;                           
PID* pidY_ = new PID;
PID* pidQ_ = new PID;

Pince* gripper = new Pince(servo_);
Moteur* Moteur_Deplacement = new Moteur(AX_, 0);
Moteur* Moteur_Elevation = new Moteur(AX_, 1);
Pendule* Pendule_ = new Pendule(AX_);
//moteur deplacement();

// Deplacement robot(Moteur_Deplacement, Moteur_Elevation, Pendule_, pidX_, pidY_, pidQ_);

#define BAUD            115200


void setup() {
  Serial.begin(BAUD);
  // initCommunication();
  
  // Initialisation du arduinoX 
  AX_->init();                    

  Moteur_Elevation->resetEncodeur();
  Moteur_Deplacement->resetEncodeur();

  // Initialisation d u servo
  servo_->attach(8, 900, 2100);
  
  // Initialisation du PID 
  pidY_->setGains(0.2, 0.05, 0);
  pidY_->setMeasurementFunc(getPositionY);
  pidY_->setCommandFunc(uptadeY);
  pidY_->setAtGoalFunc(PIDYgoalReached);
  pidY_->setEpsilon(0.000000000001);
  pidY_->setPeriod(10);


  pidX_->setGains(0.005, 0.00,  0.0); // CAD 30 jullet initial 0.005, 0.000,  0.0
  pidX_->setMeasurementFunc(getPositionX);
  pidX_->setCommandFunc(uptadeX);
  pidX_->setAtGoalFunc(PIDXgoalReached);
  pidX_->setEpsilon(10);
  pidX_->setPeriod(10);

  pidQ_->setGains(0.015, 0,  0);
  pidQ_->setMeasurementFunc(GetAngle);
  pidQ_->setCommandFunc(uptadeQ);
  pidQ_->setAtGoalFunc(PIDQgoalReached);
  pidQ_->setEpsilon(7);
  pidQ_->setPeriod(10);

  gripper->depot();

  while(strcmp(state, "Start") != 0)
  {
    time = millis();
    readMsg();
    // envoie donnee de temps en temps 
    if ( time > (delais_communication + 100)){
      communicate();
      delais_communication = time;
    }
  }
  caseActif = 22;

}

/* Boucle principale (infinie)*/
void loop() {
  time = millis();
  switch (caseActif)
  {

  // Sequence principal 
  case 10:
    // Case depart pour le robot, lève et attends dêtre loader
    
    if (caseFirstScan){
      caseFirstScan = false;
      // Serial.print("case 10, lève et attends dêtre loader et ferme pince\n");

      // Enregistre la valeur de temps lors de l'entree dans le case
      caseStartTime[caseActif] = time;

      // Update les valeurs de PID
      pidY_->setGoal(40);
      pidQ_->setGoal(0);
      pidX_->setGoal(posHome);    

      //enable les PID 
      pidX_->enable();
        XEnable = true;
      pidY_->enable();
      pidQ_->enable();
        QEnable = true;

      // securite, Make sure que la pince est ouverte
      gripper->depot();
    }

    

    // Attente que le pidy_ soit en position 

    if ( (time - caseStartTime[caseActif]) > 1000) {

      if(!setup_angle)
      {
        setup_angle = true;

        offset_angle = 0-GetAngle();
      }

      // prend les sapins 
      gripper->prendre();
    }

    if ( (time - caseStartTime[caseActif]) > 1500) {
      
      
      // active le prochain case
      caseFirstScan = true;
      caseActif++;
    }

  break;  
 
 case 11:
    // case va a la position du bac
    
    if (caseFirstScan){
      caseFirstScan = false; 

      // debug
      // Serial.print("case 11, va a la position du bac\n");

      // Enregistre la valeur de temps lors de l'entree dans le case
      caseStartTime[caseActif] = time;
      
      // Update les valeurs de PID
      pidY_->setGoal(40);
      pidQ_->setGoal(0);
      pidX_->setGoal(posBac);

      //enable les PID
      pidX_->enable();
        XEnable = true;
      pidY_->enable();
      // on veut pas qu<il se stabilise durant le deplacement 
      pidQ_->disable();
      
    }

    // Check si arriver a destination
    if (pidX_->isAtGoal()){
      // active le prochain case 
      caseFirstScan = true;
      caseActif++;
    }

  break;

  case 12:
    // Stabilisation BAC

    if (caseFirstScan){
      caseFirstScan = false; 

      // debug
      // Serial.print("case 12, Stabilisation BAC\n");

      // Enregistre la valeur de temps lors de l'entree dans le case
      caseStartTime[caseActif] = time;

      // Update les valeurs de PID
      pidY_->setGoal(40);
      pidQ_->setGoal(0);
      pidX_->setGoal(posBac);

      //enable les PID
      pidX_->enable();
        XEnable = true;
      pidY_->enable();
      pidQ_->enable();
        QEnable = true;

      //initiallisation du timer d'initialisation
      temps_stabilisation = time;

      // debug variable
      // temps_print = 0;
    }

    // les deux PID travaille ensemble pour atteindre un angle minimum et position optimal
    // Ainsi si on corrige l'angle mais que ca affecte trop la position celui-ci va se remmettre a la bonne position

    //check angle
     if(fabs(GetAngle()) > hysQ && !QEnable)
    {
      pidQ_->enable();
      QEnable = true;

      // reinit le counter
      temps_stabilisation = (time - caseStartTime[caseActif]);
    }

    // check pos
    if( (fabs(getPositionX()) > posBac+hysX || fabs(getPositionX()) < posBac-hysX )  && !XEnable)
    {
      pidX_->enable();
      XEnable = true;

      // reinit le counter
      temps_stabilisation = (time - caseStartTime[caseActif]);
    }
    
    //debug if
    // if (temps_print < (time - caseStartTime[caseActif] + 1000) ) {
      // Serial.print("pidX_->isAtGoal() ");
      // Serial.print(pidX_->isAtGoal());
      // Serial.print(" pidQ_->isAtGoal() ");
      // Serial.println(pidQ_->isAtGoal());

      // Serial.print(" position X ");
      // Serial.println(getPositionX());

      // Serial.print(" Angle Q ");
      // Serial.println(GetAngle());

      // Serial.print(" temps_stabilisation + 1000 ");
      // Serial.println(temps_stabilisation + 1000);

      // Serial.print(" time - caseStartTime[caseActif] ");
      // Serial.println(time - caseStartTime[caseActif]);
      

    //   temps_print = (time - caseStartTime[caseActif] + 1000);

    // }


    //quand les deux PID on atteint leurs but et ca fait plus que X temps quon ait dans le case on switch
    if ( ( (time - caseStartTime[caseActif]) > temps_stabilisation + 1000) && pidX_->isAtGoal() && pidQ_->isAtGoal() ){
    
      // Serial.print(" position X ");
      // Serial.println(getPositionX());

      // Serial.print(" Angle Q ");
      // Serial.println(GetAngle());

      // active le prochain case
      caseFirstScan = true;
      caseActif++;
    }

  break;

  case 13:
    // Drop les sapins

    // Enregistre la valeur de temps lors de l'entree dans le case
    if (caseFirstScan){
      caseFirstScan = false; 

      //debug
      // Serial.print("case 13, drop les sapins \n");

      caseStartTime[caseActif] = time;
    }
    // lache les sapins
    gripper->depot();

    // attend petit temps pour être sur que les sapins on bien dropper
    if ((time - caseStartTime[caseActif]) > 400){
      // active le prochain case
      caseFirstScan = true;
      caseActif++;
    }

  break;

  case 14:
    // reviens au depart en se levant en meme temps

    // Enregistre la valeur de temps lors de l'entree dans le case
    if (caseFirstScan){
      caseFirstScan = false; 

      // Serial.print("case 14, reviens au depart \n");

      caseStartTime[caseActif] = time;

      // Update les valeurs de PID
      pidY_->setGoal(40);
      pidQ_->setGoal(0);
      pidX_->setGoal(posHome);

      //enable les PID
      pidX_->enable();
        XEnable = true;
      pidY_->enable();
      pidQ_->disable();
    }

    // Deplacement

    //rendu a home
    if (pidX_->isAtGoal()){
      // active le prochain case
      caseFirstScan = true;
      caseActif++;
    }

  break;

  case 15:
    // Stabilisation HOME

    if (caseFirstScan){
      caseFirstScan = false; 

      //debug
      // Serial.print("case 15, Stabilisation HOME \n");

      // Enregistre la valeur de temps lors de l'entree dans le case
      caseStartTime[caseActif] = time;

      // Update les valeurs de PID
      pidY_->setGoal(40);
      pidQ_->setGoal(0);
      pidX_->setGoal(posHome);

      // enable les PID
      pidX_->enable();
        XEnable = true;
      pidY_->enable();
      pidQ_->enable();
        QEnable = true;

       // initiallisation du timer d'initialisation
      temps_stabilisation = time;

      // debug
      // temps_print = 0;
    }

    // les deux PID travaille ensemble pour atteindre un angle minimum et position optimal
    // Ainsi si on corrige l'angle mais que ca affecte trop la position celui-ci va se remmettre a la bonne position

    //check angle
     if(fabs(GetAngle()) > hysQ && !QEnable)
    {
      pidQ_->enable();
      QEnable = true;
      // reinit le counter
      temps_stabilisation = (time - caseStartTime[caseActif]);
    }

    // check pos
    if( (fabs(getPositionX()) > posHome+hysX || fabs(getPositionX()) < posHome-hysX )  && !XEnable)
    {
      pidX_->enable();
      XEnable = true;
      // reinit le counter
      temps_stabilisation = (time - caseStartTime[caseActif]);
    }
    
    //debug if
    // if (temps_print < (time - caseStartTime[caseActif] + 1000) ) {
      // Serial.print("pidX_->isAtGoal() ");
      // Serial.print(pidX_->isAtGoal());
      // Serial.print(" pidQ_->isAtGoal() ");
      // Serial.println(pidQ_->isAtGoal());

      // Serial.print(" position X ");
      // Serial.println(getPositionX());

      // Serial.print(" Angle Q ");
      // Serial.println(GetAngle());

      // Serial.print(" temps_stabilisation + 1000 ");
      // Serial.println(temps_stabilisation + 1000);

      // Serial.print(" time - caseStartTime[caseActif] ");
      // Serial.println(time - caseStartTime[caseActif]);

    //   temps_print = (time - caseStartTime[caseActif] + 1000);

    // }

    //quand les deux PID on atteint leurs but et ca fait plus que X temps quon ait dans le case on switch
    if ( ( (time - caseStartTime[caseActif]) > temps_stabilisation + 1000) && pidX_->isAtGoal() && pidQ_->isAtGoal() ){
      // Serial.print(" position X ");
      // Serial.println(getPositionX());

      // Serial.print(" Angle Q ");
      // Serial.println(GetAngle());

      // active le prochain case
      caseFirstScan = true;
      caseActif = 10;
    }

  break;
 /////////////////////// DEBUG ////////////////////
  case 20:
  
      //check angle
     if(fabs(GetAngle()) > hysQ && !QEnable){
      pidQ_->enable();
      QEnable = true;
    }

    // check pos
    if( (fabs(getPositionX()) > posHome+hysX || fabs(getPositionX()) < posHome-hysX )  && !XEnable){
      pidX_->enable();
      XEnable = true;
    }

    
    // Serial.print(" Angle : ");
    // Serial.println(GetAngle());

  break;

  case 21:
    // case vide pour test de comm
    pidX_->disable();
    pidY_->disable();
    pidQ_->disable();
    SpeedX = 0;
    SpeedQ = 0;
    QEnable = false;
    XEnable = false;
    setup_angle = false;
    offset_angle = 0;
    Moteur_Elevation->resetEncodeur();
    Moteur_Deplacement->resetEncodeur();
    if(strcmp(state, "Start") == 0)
    {
      caseActif = 22; //à mettre n'importe quel case pour redémarrer le tout
    }
  break;

  case 22:
    // case vide pour test de comm
  break;



  ///////////////////////////////////////////////////////////
  //                  cases de test
  //////////////////////////////////////////////////////////
  
  case 1:
    Serial.print("\n ---- TEST PINCE -----\n");
    Serial.print("close\n");
    gripper->prendre();
    delay(1000);
    gripper->depot();
    Serial.print("open\n");
    delay(1000);
  break;

  case 2:
    Serial.print("\n ---- TEST DEPLACEMENT-----\n");
    Moteur_Deplacement->setSpeed(0.0);
    delay(500);
    Moteur_Deplacement->setSpeed(0.4);
    delay(2000);
    Moteur_Deplacement->setSpeed(-0.4);
    delay(2000);
    Moteur_Deplacement->setSpeed(0.0);
    delay(500);
    break;

  case 3:
    Serial.print("\n ---- TEST ELEVATION-----\n");
    gripper->prendre();
    Moteur_Elevation->setSpeed(-1);
    delay(1000);
    Moteur_Elevation->setSpeed(0.1);
    delay(1000);
    Moteur_Elevation->setSpeed(0.0);
    break;
  
  //test PID elevation
  case 4:
     Serial.print("\n ---- TEST PID  DEPLACEMENT-----\n");
    
     pidX_->setGoal(100); 
    
    break;
  
  case 5:
    //Serial.print("\n ---- JE SUIS DANS CASE 5-----\n");
    //Serial.print("\n ---- TEST PID  DEPLACEMENT-----\n");

    pidX_->setGoal(500);
    pidY_->setGoal(40);  

    gripper->prendre();
  
    break;

  case 6:
    //Serial.print("\n ---- JE SUIS DANS CASE 6-----\n");
    
    pidX_->setGoal(0);
    pidY_->setGoal(40); 
    gripper->depot();

    break;


  case 7:
    Serial.print("\n ---- JE SUIS DANS CASE 7-----\n");
    
    pidY_->setGoal(40);
    pidQ_->setGoal(0);
    pidX_->setGoal(0);


    if(fabs(Pendule_->getAngle()) > 5 && !QEnable)
    {
      pidQ_->enable();
      QEnable = true;
    }

    if(fabs(getPositionX()) > 10 && !XEnable)
    {
      pidX_->enable();
      XEnable = true;
    }

    break;

  case 8:
    Serial.print("\n ---- JE SUIS DANS CASE 8-----\n");

    Serial.print(GetAngle());

    Serial.print("\n");

    delay(1000);

  break;


  case 101:
    Serial.print("\n ---- TEST Fonctionnement de la pince -----\n");
    Serial.print("close\n");
    gripper->prendre();
    delay(1000);
    gripper->depot();
    Serial.print("open\n");
    delay(1000);
  break;

  case 102:
    Serial.print("\n ---- TEST Déplacement selon l'axe x -----\n");
    Moteur_Deplacement->setSpeed(0.0);
    delay(500);
    Moteur_Deplacement->setSpeed(0.4);
    delay(2000);
    Moteur_Deplacement->setSpeed(-0.4);
    delay(2000);
    Moteur_Deplacement->setSpeed(0.0);
    delay(500);
  break;

  case 103:
    Serial.print("\n ---- TEST Déplacement du point de pivot selon l'axe y -----\n");
    gripper->prendre();
    Moteur_Elevation->setSpeed(-1);
    delay(1000);
    Moteur_Elevation->setSpeed(0.1);
    delay(1000);
    Moteur_Elevation->setSpeed(0.0);
  break;

  case 104:
     Serial.print("\n ---- TEST PID déplacement selon l'axe x -----\n");
    
     pidX_->setGoal(100); 
    
  break;

  case 105:
     Serial.print("\n ---- TEST PID déplacement selon l'axe y -----\n");
    
     pidY_->setGoal(55); 
    
  break;

  case 106:
     Serial.print("\n ---- TEST Intégration PID x, y et la pince -----\n");
     
     pidX_->setGoal(300);
     
     
    
  break;

  case 107:
     Serial.print("\n ---- TEST PID stabilisation du pendule -----\n");
    
     pidQ_->setGoal(0);
    
  break;

  case 108:
     Serial.print("\n ---- TEST PID stabilisation du pendule & PID déplacement selon l'axe x -----\n");  // JSUIS PAS SUPER SÛRE
    

    pidQ_->setGoal(0);
    
    if (pidQ_->isAtGoal())
    {
      pidQ_->disable();
      pidX_->setGoal(posHome);
    }

  break;

  default:
    Serial.print("caseActif invalide\n");
    break;
  }
  
 
 
  pidY_->run();
  pidX_->run();
  pidQ_->run();

  // envoie donnee de temps en temps 
  if ( time > (delais_communication + 100)){
    communicate();
    delais_communication = time;
  }
  if(strcmp(state, "Start") != 0)
  {
    caseActif = 21;
  }
}




/*---------------------------Definition de fonctions ------------------------*/



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



double getPositionY() // Trouver relation angle distance y
{
    //Serial.print("\n ---- Get Position-----\n");
    double longeur_corp_A = 152.40; // mm
    double longeur_corp_B = 152.40; // mm


    int tmpPulse;
    int nbPulseTour = 64*50*2;

    double anglePulse;
    double angle;
    double angleA;
    double angleC;
    double hauteur;
    double loi_cosinus;
    int signe = 1;

    tmpPulse = Moteur_Elevation->getPulse();

    anglePulse = (2*PI)/nbPulseTour; // Manque gear ratio peut être

    angle = anglePulse*tmpPulse;

    // if(angle < 0) // Permet l'inversion de y
    // {
    //     angle *= -1;
    //     signe = -1;
    // }

    angleC = PI-angle;

    loi_cosinus = sqrt((longeur_corp_A * longeur_corp_A) + (longeur_corp_B * longeur_corp_B) -2 * longeur_corp_A * longeur_corp_B * cos(angleC)); // Donne longeur

    angleA = asin(((sin(angleC))*longeur_corp_A)/loi_cosinus);

    hauteur = -longeur_corp_A*sin(angleA)*signe;

    // Serial.print("\n hauteur : ");
    // Serial.print(hauteur);
    // Serial.print(" \n");

    if (hauteur > 80){
      Moteur_Elevation->setSpeed(0.1);
    }

    return hauteur;


}

void uptadeY(double commande)
{
    // Serial.print("\n ---- Uptade Y -----\n");
    // Serial.print("commande::  ");
    // Serial.print(-commande);
    // Serial.print("\n");
    commande = -commande;

    if (commande > 0.1){
      commande = 0.1;
    }
    Moteur_Elevation->setSpeed(commande);

}

void PIDYgoalReached(){
  
  // Serial.print("\n ---- PID_Y GOAL REACHED -----\n");
  Moteur_Elevation->setSpeed(-0.1);
  //delay(1000);
  //pidY_->SetEnable(true);
}


double getPositionX()
{
    int tmpPulse;
    float tmpDistance;
    float diametreRoue = 60;
    int nbPulseTour = 64*19;
    float distancePulse;

    tmpPulse = Moteur_Deplacement->getPulse();
    
    distancePulse = (diametreRoue*PI)/nbPulseTour;

    tmpDistance = tmpPulse*distancePulse;

    // Serial.print("\n POSITION X : ");
    // Serial.print(tmpDistance);
    // Serial.print(" \n");

    return tmpDistance;
}

void uptadeX(double commande)
{
    // Serial.print("commande::  ");
    // Serial.print(commande);
    // Serial.print("\n");
    SpeedX = commande;

    double Combine = SpeedX + SpeedQ;

    Moteur_Deplacement->setSpeed(Combine);
}

void PIDXgoalReached()
{
    
    // Serial.print("\n ---- PID_X GOAL REACHED -----\n");

    SpeedX = 0;
    double Combine = SpeedX + SpeedQ;

    Moteur_Deplacement->setSpeed(Combine);
    pidX_->disable();
    XEnable = false;

    if (caseActif == 5){
      caseActif = 6 ;
      pidX_->enable();
    }

    else{
     if (caseActif == 6) {
      caseActif = 5;
      pidX_->enable();
     }
    }
    // Serial.print("caseActif: "); 
    // Serial.print(caseActif); 
    // Serial.print("\n"); 

    // pidX_->enable();

}


double GetAngle()
{
  double longeur_corp_A = 152.0; // mm
  double longeur_corp_B = 152.0; // mm


  int tmpPulse;
  int nbPulseTour = 64*50*2;

  double anglePulse;
  double angle;
  double angleA;
  double angleC;
  double loi_cosinus;

  tmpPulse = Moteur_Elevation->getPulse();

  anglePulse = (2*PI)/nbPulseTour;

  angle = anglePulse*tmpPulse;

  angleC = PI-angle;

  loi_cosinus = sqrt((longeur_corp_A * longeur_corp_A) + (longeur_corp_B * longeur_corp_B) -2 * longeur_corp_A * longeur_corp_B * cos(angleC)); // Donne longeur

  angleA = asin(((sin(angleC))*longeur_corp_A)/loi_cosinus);

  // Partie pour angle

  double True_angleA;
  double angleA_2;
  double Rayon_roue = (62.75/2)-8.75;
  double Rayon_bearing = 12.7/2;
  double anglePendule;

  angleA_2 = asin((Rayon_roue-Rayon_bearing)/loi_cosinus);

  True_angleA = (angleA-angleA_2)*180/PI;

  anglePendule =  True_angleA - Pendule_->getAngle() + offset_angle;

  return anglePendule;


}

void uptadeQ(double commande)
{
  commande = commande; // Strange un bout fallait que sa soit inverser mais maintenant non
  SpeedQ = commande;

  double Combine = SpeedQ + SpeedX;

  Moteur_Deplacement->setSpeed(Combine);
}

void PIDQgoalReached()
{
    
  // Serial.print("\n ---- PID_X GOAL REACHED -----\n");
  SpeedQ = 0;
  double Combine = SpeedX + SpeedQ;

  Moteur_Deplacement->setSpeed(Combine);
  QEnable = false;
  pidQ_->disable();

}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////// SECTION FONTIONS POUR LA COMMUNICATION //////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

// Bools à activer pour contrôller la comm
// Tous remis à zéro quand leur fonction repective est run
bool shouldComm;
bool shouldSend;
bool shouldRead;
 
// Variables qui sont envoyées
/*
"time" -> le temps (ms) depuit le début d'opération
"potAngle" -> l'angle du pot
"goal" -> le but de position du robot
"voltage" -> le voltage d'opération
"current" -> le courant d'opération
"EtatRobot" -> Si le robot est à on (start) ou off (stop)
"isGoal" -> Si le robot a atteint son but
"positionX" -> La position horizontale du robot
"positionY" -> La position verticale du robot
*/
 

 
bool commandeManuelle; // =1 ou =0 pour si on est en mode manuel ou pas
JsonVariant goalManuel; // ="droite" ou = "fermerPince" ou ="goHome" ou ="goDepot" ou ...


void communicate(){
  sendMsg();
  readMsg();
  shouldComm = 0;
}
 
void sendMsg(){
  /* Envoit du message Json sur le port seriel */
  StaticJsonDocument<500> doc;
 
  // Elements du message
  // MODIFIER ICI  : Ce qu'on veut envoyer dans le json
  doc["time"] = millis();
  doc["potAngle"] = GetAngle();
  doc["voltage"] = AX_->getVoltage();
  doc["current"] = AX_->getCurrent(); 
  doc["EtatRobot"] = caseActif;
  doc["positionX"] = getPositionX();
  doc["positionY"] = getPositionY();
 
  // Serialisation
  serializeJson(doc, Serial);
  // Envoit
  Serial.println();
  shouldSend = 0;
}
 
void readMsg(){
  // Lecture du message Json
  StaticJsonDocument<500> doc;
  
  // Check if serial data is available
  if (Serial.available() > 0) {
    // Read the incoming JSON data from Serial
    DeserializationError error = deserializeJson(doc, Serial);
    
    // Si erreur dans le message
    if (error) {
      //Serial.print("deserialize() failed: ");
      //Serial.println(error.c_str());
      return;
    }

    // Analyse des éléments du message
    // MODIFIER ICI : Ce qu'on veut recevoir par json
    if (doc.containsKey("state")) {
      const char* stateValue = doc["state"];
      strcpy(state, stateValue); // Copy the state value to the state variable
    }

    if (doc.containsKey("setGoal")) {
      // Uncomment and modify the code according to how you handle pid
      // pid_.disable();
      // pid_.setGains(doc["setGoal"][0], doc["setGoal"][1], doc["setGoal"][2]);
      // pid_.setEpsilon(doc["setGoal"][3]);
      // pid_.setGoal(doc["setGoal"][4]);
      // pid_.enable();
    }

    if (doc.containsKey("commandeManuelle")) {
      commandeManuelle = doc["commandeManuelle"];
    }

    if (doc.containsKey("goalManuel")) {
      goalManuel = doc["goalManuel"];
    }

    shouldRead = false; // Example to indicate reading is done
  }
}