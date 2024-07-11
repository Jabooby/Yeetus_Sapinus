
#ifndef CLASSE_H
#define CLASSE_H

#include <LibS3GRO.h>
#include <math.h>

#define GAUCHE 0
#define DROITE 1


//#include <main_header.h>




//classes pince
class Pince
{
    public:
        Pince(MegaServo* ptr);
        ~Pince();
        
        bool getEtat();
        void prendre();
        void depot();

    private:

        MegaServo* ptrServo;

        void setPosition(int position);

        int posOuvert;
        int posFermer;

    protected:

};

//classes moteur
class Moteur
{
	public:

	Moteur(ArduinoX* AX_, uint8_t temp_ID = -1);
	~Moteur();
	void setSpeed(float Speed);
	void setID(uint8_t ID);
	int getPulse();
	
	private:

    ArduinoX* ptrAdruino;
	uint8_t ID;
};

// //classes deplacement

// /*class Deplacement
// {
//     public:

//     Deplacement(ArduinoX* ptr);
//     ~Deplacement();

//     void goHome();
//     void goDepot();

//     void uptadePID();
//     float getPositionX();
//     float getPositionY();
//     void Stabilisation();

//     private:
       
//     ArduinoX* ptrAx; // Pour les moteurs

//     void uptadeX(double errorX);
//     void uptadeY(double errorY);
//     float posX;
//     float posY;
//     bool AngleOk;
//     float toléranceX;

//     Moteur* moteurDeplacement;
//     Moteur* moteurElevation;

//     PID* pidX_;
//     PID* pidY_;


// }

// class Pendule
// {
// 	public:
	
//     Pendule(ArduinoX* AX_);
//     ~Pendule();

// 	float getAngle();
// 	bool getDirection();
// 	void updateAngleOk();
// 	bool getAngleOk();

// 	private:

//  ArduinoX* ptrArduino;
// 	float AngleMax;
// 	bool direction;
// 	bool AngleOk;
// };*/

#endif //CLASSE_H