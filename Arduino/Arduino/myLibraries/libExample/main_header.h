#ifndef MAIN_HEADER_H
#define MAIN_HEADER_H

/*------------------------------ Librairies ---------------------------------*/
#include <LibS3GRO.h>

//custum
//#include <classes.h> //met 2 fois
//#include <communication.h>

ArduinoX AX_;                       // objet arduinoX
MegaServo servo_;                   // objet servomoteur

PID pid_;                           // objet PID

/*------------------------------ Constantes ---------------------------------*/

#define MAGPIN          32          // Port numerique pour electroaimant
#define POTPIN          A5          // Port analogique pour le potentiometre

#define PASPARTOUR      64          // Nombre de pas par tour du moteur
#define RAPPORTVITESSE  50          // Rapport de vitesse du moteur

/*---------------------------- variables globales ---------------------------*/

// ArduinoX AX_;                       // objet arduinoX
// MegaServo servo_;                   // objet servomoteur

// PID pid_;                           // objet PID


#endif // MAIN_HEADER_H