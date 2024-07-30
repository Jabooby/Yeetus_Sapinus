// #include <communication.h>
// // Bools à activer pour contrôller la comm
// // Tous remis à zéro quand leur fonction repective est run
// bool shouldComm;
// bool shouldSend;
// bool shouldRead;
 
// // Variables qui sont envoyées
// /*
// "time" -> le temps (ms) depuit le début d'opération
// "potAngle" -> l'angle du pot
// "goal" -> le but de position du robot
// "voltage" -> le voltage d'opération
// "current" -> le courant d'opération
// "EtatRobot" -> Si le robot est à on (start) ou off (stop)
// "isGoal" -> Si le robot a atteint son but
// "positionX" -> La position horizontale du robot
// "positionY" -> La position verticale du robot
// */
 
// //variables qui sont reçues
// String state; // ="start" ou ="stop"
 
// bool commandeManuelle; // =1 ou =0 pour si on est en mode manuel ou pas
// String goalManuel; // ="droite" ou = "fermerPince" ou ="goHome" ou ="goDepot" ou ...
// void communicate(){
//   sendMsg();
//   readMsg();
//   shouldComm = 0;
// }
 
// void sendMsg(){
//   /* Envoit du message Json sur le port seriel */
//   StaticJsonDocument<500> doc;
 
//   // Elements du message
//   // MODIFIER ICI  : Ce qu'on veut envoyer dans le json
//   doc["time"] = millis();
//   doc["potAngle"] = GetAngle();
//   //doc["directionPendule"] = getDirection(); À revenir?
//   doc["goal"] = pid_.getGoal();
//   doc["voltage"] = AX_.getVoltage();
//   doc["current"] = AX_.getCurrent(); 
//   doc["EtatRobot"] = state;
//   doc["isGoal"] = pid_.isAtGoal();
//   // doc["actualTime"] = pid_.getActualDt(); sert à quoi?
//   doc["positionX"] = getPositionX();
//   doc["positionY"] = getPositionY();
 
//   // Serialisation
//   serializeJson(doc, Serial);
//   // Envoit
//   Serial.println();
//   shouldSend = 0;
// }
 
// void readMsg(){
//   // Lecture du message Json
//   StaticJsonDocument<500> doc;
//   JsonVariant parse_msg;
 
//   // Lecture sur le port Seriel
//   DeserializationError error = deserializeJson(doc, Serial);
//   // Lorsque shouldComm est à 1, call communication
 
//   // Si erreur dans le message
//   if (error) {
//     Serial.print("deserialize() failed: ");
//     Serial.println(error.c_str());
//     return;
//   }
//   // Analyse des éléments du message
//   // MODIFIER ICI : Ce qu'on veut recevoir par json
//    parse_msg = doc["state"];
//   if(!parse_msg.isNull()){
//      state = doc["state"];
//   }
 
//   // À edit selon la façon dont le pid est fait
//    parse_msg = doc["setGoal"];
//   if(!parse_msg.isNull()){
//     pid_.disable();
//     pid_.setGains(doc["setGoal"][0], doc["setGoal"][1], doc["setGoal"][2]);
//     pid_.setEpsilon(doc["setGoal"][3]);
//     pid_.setGoal(doc["setGoal"][4]);
//     pid_.enable();
//   }
 
//    parse_msg = doc["commandeManuelle"];
//   if(!parse_msg.isNull()){
//      commandeManuelle = doc["commandeManuelle"];
//   }
 
//    parse_msg = doc["goalManuel"];
//   if(!parse_msg.isNull()){
//      goalManuel = doc["goalManuel"];
//   }
 
//   shouldRead = 0;
// }
// // void initCommunication(){

// //   Serial.begin(BAUD);               // initialisation de la communication serielle
  
// //   // Chronometre envoie message
// //   timerSendMsg_.setDelay(UPDATE_PERIODE);
// //   timerSendMsg_.setCallback(timerCallback);
// //   timerSendMsg_.enable();


// // }

// // void serialEvent(){shouldRead_ = true;}

// // void timerCallback(){shouldSend_ = true;}

// // void sendMsg(){
// //   /* Envoit du message Json sur le port seriel */
// //   StaticJsonDocument<500> doc;
// //   // Elements du message

// //   doc["time"] = millis();
// //   doc["potVex"] = 0; // analogRead(POTPIN);
// //   doc["encVex"] = 0; // vexEncoder_.getCount();
// //   doc["goal"] = 0; // pid_.getGoal();
// //   doc["measurements"] = 0;//PIDmeasurement();
// //   doc["voltage"] = 0; // AX_.getVoltage();
// //   doc["current"] = 0; // AX_.getCurrent(); 
// //   doc["pulsePWM"] = 0 ; //pulsePWM_;
// //   doc["pulseTime"] = 0 ; //pulseTime_;
// //   doc["inPulse"] = 0 ; //isInPulse_;
// //   doc["accelX"] = 0; // imu_.getAccelX();
// //   doc["accelY"] = 0; // imu_.getAccelY();
// //   doc["accelZ"] = 0; // imu_.getAccelZ();
// //   doc["gyroX"] = 0; // imu_.getGyroX();
// //   doc["gyroY"] = 0; // imu_.getGyroY();
// //   doc["gyroZ"] = 0; // imu_.getGyroZ();
// //   doc["isGoal"] = 0; // pid_.isAtGoal();
// //   doc["actualTime"] = 0; // pid_.getActualDt();

// //   // Serialisation
// //   serializeJson(doc, Serial);
// //   // Envoit
// //   Serial.println();
// //   shouldSend_ = false;
// // }

// // void readMsg(){
// //   // Lecture du message Json
// //   StaticJsonDocument<500> doc;
// //   JsonVariant parse_msg;

// //   // Lecture sur le port Seriel
// //   DeserializationError error = deserializeJson(doc, Serial);
// //   shouldRead_ = false;

// //   // Si erreur dans le message
// //   if (error) {
// //     Serial.print("deserialize() failed: ");
// //     Serial.println(error.c_str());
// //     return;
// //   }
  
// //   // Analyse des éléments du message message
// //   parse_msg = doc["pulsePWM"];
// //   if(!parse_msg.isNull()){
// //      pulsePWM_ = doc["pulsePWM"].as<float>();
// //   }

// //   parse_msg = doc["pulseTime"];
// //   if(!parse_msg.isNull()){
// //      pulseTime_ = doc["pulseTime"].as<float>();
// //   }

// //   parse_msg = doc["pulse"];
// //   if(!parse_msg.isNull()){
// //      shouldPulse_ = doc["pulse"];
// //   }
// //   parse_msg = doc["setGoal"];
// //   if(!parse_msg.isNull()){
// //     pid_.disable();
// //     pid_.setGains(doc["setGoal"][0], doc["setGoal"][1], doc["setGoal"][2]);
// //     pid_.setEpsilon(doc["setGoal"][3]);
// //     pid_.setGoal(doc["setGoal"][4]);
// //     pid_.enable();
// //   }
// // }