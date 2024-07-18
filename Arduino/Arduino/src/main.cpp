#include <classes.h>
#include <Arduino.h>

//var globales
int choix = 0;

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

// Creation objets
ArduinoX* AX_ = new ArduinoX;                       // objet arduinoX
MegaServo* servo_ = new MegaServo;                   // objet servomoteur

PID* pidX_ = new PID;                           
PID* pidY_ = new PID;
PID* pidQ_ = new PID;

Pince* gripper = new Pince(servo_);
Moteur* Moteur_Deplacement = new Moteur(AX_, 0);
Moteur* Moteur_Elevation = new Moteur(AX_, 1);
//Pendule* Pendule_ = new Pendule(AX_);
//moteur deplacement();

// Deplacement robot(Moteur_Deplacement, Moteur_Elevation, Pendule_, pidX_, pidY_, pidQ_);

#define BAUD            115200


void setup() {
  Serial.begin(BAUD);
  // initCommunication();
  AX_->init();                       // initialisation de la carte ArduinoX 

  Moteur_Elevation->resetEncodeur();
  Moteur_Deplacement->resetEncodeur();

  
  // // Initialisation du PID 
  // pid_.setGains(0.25,0.1 ,0);
  // // Attache des fonctions de retour
  // pid_.setMeasurementFunc(PIDmeasurement);
  // pid_.setCommandFunc(PIDcommand);
  // pid_.setAtGoalFunc(PIDgoalReached);
  // pid_.setEpsilon(0.001);
  // pid_.setPeriod(200);
  

  servo_->attach(8, 900, 2100);
  
  pidY_->setGains(0.2, 0.05, 0);
  pidY_->setMeasurementFunc(getPositionY);
  pidY_->setCommandFunc(uptadeY);
  pidY_->setAtGoalFunc(PIDYgoalReached);
  pidY_->setEpsilon(0.000000000001);
  pidY_->setPeriod(10);


  pidX_->setGains(0.005, 0,  0);
  pidX_->setMeasurementFunc(getPositionX);
  pidX_->setCommandFunc(uptadeX);
  pidX_->setAtGoalFunc(PIDXgoalReached);
  pidX_->setEpsilon(8);
  pidX_->setPeriod(100);

  pidX_->enable();
  pidY_->enable();
  choix = 3;
}

/* Boucle principale (infinie)*/
void loop() {

  switch (choix)
  {
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

    pidX_->setGoal(300);
    pidY_->setGoal(40);  

    gripper->prendre();
  
    break;

  case 6:
    //Serial.print("\n ---- JE SUIS DANS CASE 6-----\n");
    
    pidX_->setGoal(0);
    pidY_->setGoal(40); 
    gripper->depot();

    break;


  default:
    Serial.print("choix invalide\n");
    break;
  }
  
 
 pidY_->run();
 pidX_->run();
        
}

/*---------------------------Definition de fonctions ------------------------*/




/////////////////////////////////////////////////////////////////////////////

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
    Moteur_Deplacement->setSpeed(commande);
}

void PIDXgoalReached()
{
    Moteur_Deplacement->setSpeed(0);
    // Serial.print("\n ---- PID_X GOAL REACHED -----\n");
 
    pidX_->disable();

    if (choix == 5){
      choix = 6 ;
      pidX_->enable();
    }

    else{
     if (choix == 6) {
      choix = 5;
      pidX_->enable();
     }
    }
    // Serial.print("choix: "); 
    // Serial.print(choix); 
    // Serial.print("\n"); 

    // pidX_->enable();

   
  

}




