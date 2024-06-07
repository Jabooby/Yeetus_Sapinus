#include <Arduino.h>
#include <librobus.h>

// Constants and variables
//
const uint8_t Electromagnet_pin[2] = {SCL, SDA}; //J10
int pinJ19 = A5;
int pot;
float angle;

// put function declarations here:
float readpot( int pin );
void electromagnet_control( uint8_t state,  unsigned int duration );

void setup() {
  pinMode(Electromagnet_pin, OUTPUT);
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

void electromagnet_control( uint8_t state,  unsigned long duration ) {
  //Active ou non l'électroaimant
  digitalWrite(Electromagnet_pin, state);

  //Ecrit le state de l'électroaimant
  if (state == HIGH) {
      Serial.println("Electromagnet is ON");
  } else {
      Serial.println("Electromagnet is OFF");
  }

  //Durée de l'activation
  delay(duration);

}