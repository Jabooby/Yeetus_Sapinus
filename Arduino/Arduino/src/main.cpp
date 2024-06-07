#include <Arduino.h>
#include <librobus.h>

// Constants and variables
//
int pinJ19 = A5;
int pot;
float angle;
// put function declarations here:
float readpot( int pin );

void setup() {
  //BoardInit(); //faut trouver façon de just init le arduino X mais jsp programmer objet faque j'ai pas réussi
  Serial.begin(9600);
}

void loop() {
  
  //Lecture du potentiometre
  pot = analogRead(pinJ19);
  
  //Utilisation de la fonction de calcul de l'angle
  angle = readpot( pinJ19 );

  //Print les résultats
  Serial.print(pot);
  Serial.print("---");
  Serial.print(angle);
  Serial.print("\n");
  delay(1000);

}


//SECTIONS POUR LES FONCTIONS 
float readpot( int pin ) {

  // la pin de donnée pour J19 est : A5

  float offset_0 = 132;
  float angle = ((float)analogRead(pin) / 1023.0) * 265.0 - offset_0;
  return angle;

}