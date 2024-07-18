#include <classes.h>
#include <Arduino.h>

// Fonctions pour le PID
double PIDmeasurement();
void PIDcommand(double cmd);
void PIDgoalReached();

// Creation objets
ArduinoX* AX_ = new ArduinoX;                       // objet arduinoX
MegaServo* servo_ = new MegaServo;                   // objet servomoteur

PID* pidX_ = new PID;                           // objet PID
PID* pidY_ = new PID;

Pince gripper(servo_);
Moteur Moteur_Deplacement(AX_, 0);
Moteur Moteur_Elevation(AX_, 1);
//moteur deplacement();
#define BAUD            115200



void setup() {
  Serial.begin(BAUD);
  // initCommunication();
  AX_->init();                       // initialisation de la carte ArduinoX 

  
  // // Initialisation du PID 
  // pid_.setGains(0.25,0.1 ,0);
  // // Attache des fonctions de retour
  // pid_.setMeasurementFunc(PIDmeasurement);
  // pid_.setCommandFunc(PIDcommand);
  // pid_.setAtGoalFunc(PIDgoalReached);
  // pid_.setEpsilon(0.001);
  // pid_.setPeriod(200);
  
  servo_->attach(8, 900, 2100);
  
}

/* Boucle principale (infinie)*/
void loop() {
  int choix = 3;

  switch (choix)
  {
  case 1:
    Serial.print("\n ---- TEST PINCE -----\n");
    Serial.print("close\n");
    gripper.prendre();
    delay(1000);
    gripper.depot();
    Serial.print("open\n");
    delay(1000);
    break;

  case 2:
    Serial.print("\n ---- TEST DEPLACEMENT-----\n");
    Moteur_Deplacement.setSpeed(0.0);
    delay(500);
    Moteur_Deplacement.setSpeed(0.4);
    delay(2000);
    Moteur_Deplacement.setSpeed(-0.4);
    delay(2000);
    Moteur_Deplacement.setSpeed(0.0);
    delay(500);
    break;

  case 3:
    Serial.print("\n ---- TEST ELEVATION-----\n");
    Moteur_Elevation.setSpeed(-0.6);
    delay(1000);
    Moteur_Elevation.setSpeed(0.1);
    delay(1000);
    Moteur_Elevation.setSpeed(0.0);
    break;

  default:
    Serial.print("choix invalide\n");
    break;
  }


  //test class
  
 
// TEST MOT
  





  //pid_.run();         
}

/*---------------------------Definition de fonctions ------------------------*/



// Fonctions pour le PID
double PIDmeasurement(){
  // To do
  return 0.0;
}
void PIDcommand(double cmd){
  // To do

}
void PIDgoalReached(){
  // To do
}

double getPositionX()
{
    int tmpPulse;
    float tmpDistance;
    float diametreRoue = 30;
    int nbPulseTour = 64*19;
    float distancePulse;

    tmpPulse = Moteur_Deplacement.getPulse();
    
    distancePulse = (diametreRoue*PI)/nbPulseTour;

    tmpDistance = tmpPulse*distancePulse;

    return tmpDistance;
}