
#ifndef CLASSE_H
#define CLASSE_H

#include <LibS3GRO.h>
#include <math.h>

#define GAUCHE 0
#define DROITE 1

#define MAX_MOTOR_SPEED 1
#define MIN_MOTOR_SPEED -1

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
	void resetEncodeur();
	
	private:

    ArduinoX* ptrAdruino;
	uint8_t ID;
};


class Pendule
{
	public:
	
    Pendule(ArduinoX* AX_);
    ~Pendule();

	float getAngle();
	bool getDirection();
	void updateAngleOk();
	bool getAngleOk();

	private:

    ArduinoX* ptrArduino;
	float AngleMax;
	bool direction;
	bool AngleOk;
};

//classes deplacement

// class Deplacement
// {
//     public:

//     Deplacement(Moteur* ptrX, Moteur* ptrY, Pendule* ptr_potentio, PID* ptr_pidx, PID* ptr_pidy, PID* ptr_pidq);
//     ~Deplacement();

//     // void init(double p, double i, double d, double *MeasurementFunc, double* CommandFunc, int periode, double epsilon);

//     void goHome();
//     void goDepot();

//     void goUp();

//     void uptadePID();
//     double getPositionX();
//     double getPositionY();
//     void Stabilisation();
    

//     private:

//     void uptadeX(double errorX);
//     void uptadeY(double errorY);
//     float posX;
//     float posY;
//     bool AngleOk;
//     float toleranceX;

//     Moteur* moteurDeplacement;
//     Moteur* moteurElevation;

//     // Calcul test;

//     PID* pidX_;
//     PID* pidY_;
//     PID* pidQ_;

//     Pendule* potentio;


// };



#endif //CLASSE_H