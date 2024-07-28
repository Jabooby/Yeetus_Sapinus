#include <classes.h>


// Class Pince
Pince::Pince(MegaServo* ptr)
{
    posOuvert = 88; // A tester
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
	setID(ID);
    ptrAdruino = AX_;

   
}

// Destructeur
Moteur::~Moteur()
{

}

void Moteur::resetEncodeur()
{
    ptrAdruino->resetEncoder(ID);
}
// Setters
void Moteur::setSpeed(float Speed)
{
    if (Speed > MAX_MOTOR_SPEED){
        Speed = MAX_MOTOR_SPEED;
    }

    if (Speed < MIN_MOTOR_SPEED){
        Speed = MIN_MOTOR_SPEED;
    }
	ptrAdruino->setMotorPWM(ID, Speed);
}

void Moteur::setID(uint8_t ID)
{
	this-> ID = ID;
}

// Getters
int Moteur::getPulse()
{
	return ptrAdruino->readEncoder(ID); // WF : Pas sur qu'on veut read reset dans le cas du PID
}

////////////////////////////////////////////////////////////////////////////////

// Constructeur
Pendule::Pendule(ArduinoX* AX_)
{
	AngleMax = 0; // Valeur maximale à laquelle le pendule s'est déplacé.
	direction = GAUCHE;
	AngleOk = 1; // Angle Valide
    ptrArduino = AX_;
}

// Destructeur
Pendule::~Pendule()
{

}

// Getters
float Pendule::getAngle()
{

	float offset_0 = 265.0/2;
    float angle = ((float)analogRead(A5) / 1023.0) * 265.0 - offset_0;
    return -angle;
}

bool Pendule::getDirection()
{
	int i = 0;
	
	while(i == 0){
		int tmp_angle = Pendule::getAngle();

		if(tmp_angle < 0){
			if(direction == DROITE)
			{
				AngleMax = 0;
			}
			direction = GAUCHE;
			return GAUCHE;
		}
		else if(tmp_angle > 0){
			if(direction == GAUCHE)
			{
				AngleMax = 0;
			}
			direction = DROITE;
			return DROITE;
		}
	}

    return 0;
}

void Pendule::updateAngleOk()
{
	if(abs(AngleMax) <= 5){ // Déterminer la tolérance que l'on accepte
		AngleOk = 1;
	}
	else{
		AngleOk = 0;
	}
}


bool Pendule::getAngleOk()
{
	return AngleOk;
}



/////////////////////////////////////////////////////////////////////////////

// Deplacement::Deplacement(Moteur* ptrX, Moteur* ptrY, Pendule* ptr_potentio, PID* ptr_pidx, PID* ptr_pidy, PID* ptr_pidq)
// {
//     moteurDeplacement = ptrX;
//     moteurElevation = ptrY;
//     potentio = ptr_potentio;

//     pidX_ = ptr_pidx;
//     pidY_ = ptr_pidy;
//     pidQ_ = ptr_pidq;

//     pidX_->setGains(27,1.5,0);
//     pidX_->setMeasurementFunc(getPositionX);
//     pidX_->setCommandFunc(uptadeX);
//     pidX_->setPeriod(200); // JSP a tester
//     pidX_->setEpsilon(0.001); // Basically c la tolerance pour savoir si le PID est at goal


//     pidY_->setGains(800,300,12);
//     pidY_->setMeasurementFunc(getPositionY);
//     pidY_->setCommandFunc(uptadeY);
//     pidY_->setPeriod(200); // JSP a tester
//     pidY_->setEpsilon(0.001); // Basically c la tolerance pour savoir si le PID est at goal

//     // pidQ_->setGains(400,15,20);
//     // pidQ_->setMeasurementFunc(getPositionY);
//     // pidQ_->setCommandFunc(uptadeY);
//     // pidQ_->setPeriod(200); // JSP a tester
//     // pidQ_->setEpsilon(0.001); // Basically c la tolerance pour savoir si le PID est at goal

// }

// Deplacement::~Deplacement()
// {

// }

// // void Deplacement::init(){

// // }



// void Deplacement::goHome()
// {
//     posX = 0; // A tester mm
//     posY = 0; // A tester mm
//     pidX_->setGoal(posX);
//     pidX_->enable();
// }

// void Deplacement::goDepot()
// {
//     posX = 100; // A tester mm
//     posY = 0; // A tester mm
//     pidX_->setGoal(posX);
//     pidX_->enable();
// }

// void Deplacement::goUp() //.h
// {
//     posY = -10; // A tester mm
//     pidY_->setGoal(posY);
//     pidY_->enable();
// }

// void Deplacement::uptadePID()
// {
//     //pidX_->run();

//     pidY_->run();
// }

// double Deplacement::getPositionX()
// {
//     int tmpPulse;
//     float tmpDistance;
//     float diametreRoue = 30;
//     int nbPulseTour = 64*19;
//     float distancePulse;

//     tmpPulse = moteurDeplacement->getPulse();
    
//     distancePulse = (diametreRoue*PI)/nbPulseTour;

//     tmpDistance = tmpPulse*distancePulse;

//     return tmpDistance;
// }

// double Deplacement::getPositionY() // Trouver relation angle distance y
// {
//     double longeur_corp_A = 152.40; // mm
//     double longeur_corp_B = 152.40; // mm


//     int tmpPulse;
//     int nbPulseTour = 64*50*2;

//     double anglePulse;
//     double angle;
//     double angleA;
//     double angleC;
//     double hauteur;
//     double loi_cosinus;
//     int signe = 1;

//     tmpPulse = moteurElevation->getPulse();

//     anglePulse = (2*PI)/nbPulseTour; // Manque gear ratio peut être

//     angle = anglePulse*tmpPulse;

//     if(angle < 0) // Permet l'inversion de y
//     {
//         angle *= -1;
//         signe = -1;
//     }

//     angleC = PI-angle;

//     loi_cosinus = sqrt((longeur_corp_A * longeur_corp_A) + (longeur_corp_B * longeur_corp_B) -2 * longeur_corp_A * longeur_corp_B * cos(angleC)); // Donne longeur

//     angleA = asin((sin(angleC)/loi_cosinus)*longeur_corp_A);

//     hauteur = longeur_corp_A*sin(angleA)*signe;

//     return hauteur;
// }

// void Deplacement::Stabilisation()
// {

// }

// void Deplacement::uptadeX(double errorX)
// {
//     moteurDeplacement->setSpeed(errorX);
// }

// void Deplacement::uptadeY(double errorY)
// {
//     moteurElevation->setSpeed(errorY);
// }

