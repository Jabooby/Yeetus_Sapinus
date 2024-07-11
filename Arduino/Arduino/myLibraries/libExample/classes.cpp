#include <classes.h>


// Class Pince
Pince::Pince(MegaServo* ptr)
{
    posOuvert = 70; // A tester
    posFermer = 0; // A tester  

    ptrServo = ptr;

    //Serial.print("\nAttach");
    
}


Pince::~Pince()
{
    
}

bool Pince::getEtat()
{
    int tmp_position = posOuvert; // Si il y a une erreur renvoie ouvert

    tmp_position =  ptrServo->read();

    if(tmp_position <= posFermer)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Pince::prendre()
{
    //Serial.print("prendre\n");
    setPosition(posFermer); // Checker pour mettre un delai. S'assurer qu'il ferme
}


void Pince::depot()
{
    //Serial.print("depot\n");
    setPosition(posOuvert); // Checker pour mettre un delai. S'assurer qu'il ouvre
}


void Pince::setPosition(int position)

{
    //Serial.print("Setposition\n");
    ptrServo->write(position);


    //Serial.print("la valeur de pos envoyer est: ");
    //Serial.print(ptrServo->read());
    //Serial.print("\n");
}





// Constructeur
Moteur::Moteur(ArduinoX* AX_, uint8_t ID)
{
	this-> ID = ID;
    ptrAdruino = AX_;
}

// Destructeur
Moteur::~Moteur()
{

}

// Setters
void Moteur::setSpeed(float Speed)
{
	ptrAdruino->setMotorPWM(ID, Speed);
}

void Moteur::setID(uint8_t ID)
{
	this-> ID = ID;
}

// Getters
int Moteur::getPulse()
{
	return ptrAdruino->readResetEncoder(ID);
}

////////////////////////////////////////////////////////////////////////////////

// Constructeur
//Pendule::Pendule()
//{
//	AngleMax = 0; // Valeur maximale à laquelle le pendule s'est déplacé.
//	direction = GAUCHE;
//	AngleOk = 1; // Angle Valide
//  ptrAdruino = AX_;
//}

// Destructeur
//Pendule::~Pendule()
//{

//}

// Getters
//float Pendule::getAngle()
//{

//	tmp_angle = ptrAdruino->analogRead(J15);

//	if(tmp_angle > AngleMax){
//		AngleMax = tmp_angle;
//	}

//	return tmp_angle;
//}

//bool Pendule::getDirection()
//{
//	int i = 0;
	
//	while(i == 0){
//		int tmp_angle = Pendule::getAngle();

//		if(tmp_angle < 0){
//			if(direction = DROITE)
//			{
//				AngleMax = 0;
//			}
//			direction = GAUCHE;
//			return GAUCHE;
//		}
//		else if(tmp_angle > 0){
//			if(direction = GAUCHE)
//			{
//				AngleMax = 0;
//			}
//			direction = DROITE;
//			return DROITE;
//		}
//	}
//}

//void Pendule::updateAngleOk()
//{
//	if(abs(AngleMax) <= 5){ // Déterminer la tolérance que l'on accepte
//		AngleOk = 1;
//	}
//	else{
//		AngleOk = 0;
//	}
//}


//bool Pendule::getAngleOk()
//{
//	return AngleOk;
//}



/////////////////////////////////////////////////////////////////////////////

/*Deplacement::Deplacement(Moteur* ptrX, Moteur* ptrY, PID* ptrPIDX, PID* ptrPIDY)
{
    moteurDeplacement = ptrX;
    moteurElevation = ptrY;
    pidX_ = ptrPIDX;
    pidY_ = ptrPIDY;

    pidX_->setGains(0,0,0,0)
    pidX_->setMeasurementFunc(getPositionX);
    pidX_->setCommandFunc(uptadeX);


    pidY_->setGains(0,0,0,0)

    // // Initialisation du PID 
  // pid_.setGains(0.25,0.1 ,0);
  // // Attache des fonctions de retour
  // pid_.setMeasurementFunc(PIDmeasurement);
  // pid_.setCommandFunc(PIDcommand);
  // pid_.setAtGoalFunc(PIDgoalReached);
  // pid_.setEpsilon(0.001);
  // pid_.setPeriod(200);
}

Deplacement::~Deplacement()
{

}

void Deplacement::goHome()
{
    posX = 0; // A tester mm
    posY = 0; // A tester mm
}

void Deplacement::goDepot()
{
    posX = 100; // A tester mm
    posY = 100; // A tester mm
}

void Deplacement::uptadePID()
{
    uptadeX();
    uptadeY();
}

float Deplacement::getPositionX()
{
    int tmpPulse;
    float tmpDistance;
    float diametreRoue = 30;
    int nbPulseTour = 64*19;
    float distancePulse;

    tmpPulse = moteurDeplacement->getPulse();
    
    distancePulse = (diametreRoue*PI)/nbPulseTour;

    tmpDistance = tmpPulse*nbPulseTour;

    return tmpDistance;
}

float Deplacement::getPositionY() // Trouver relation angle distance y
{
    int tmpPulse;
    float tmpDistance;
    float diametreRoue = 30;
    int nbPulseTour = 3200;
    float distancePulse;

    tmpPulse = moteurDeplacement->getPulse();
    
    distancePulse = (diametreRoue*PI)/nbPulseTour;

    tmpDistance = tmpPulse*nbPulseTour;

    return tmpDistance;
}

void Deplacement::Stabilisation()
{

}

void Deplacement::uptadeX(double errorX)
{
    moteurDeplacement->setSpeed(errorX)
}

void Deplacement::uptadeY(double errorY)
{
    moteurElevation->setSpeed(errorY)
}


*/



