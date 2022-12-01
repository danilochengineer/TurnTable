#include "AccelStepper.h"

//Variable Declaration

int pinE = 12;            //Sensor Pin

int pinA = 16;            //Motor PinA
int pinB = 5;             //Motor PinB
int pinC = 4;             //Motor PinC
int pinD = 0;             //Motor PinD

int SP = 115200;          //Serial Speed

int RatioToTurns = 224;   // Motor axle ratio per revolutions, one turn = 32, seven turns = 224
int MAXAngle =  360;      // Maximum angle to turn.
int MINAngle = -360;      // Minimum angle to turn.

int speedA = 900;         // Maximum dry start speed without ramp
int speedB = 900;         // Maximum speed rotation of degrees
int speedC = 900;         // Rotação maxima dos graus rampa

String SerialString;      // Variavel da Leitura Serial String Bruta
int mySerial;             // Variavel da Leitura Serial Int

int movimentopronto = 1;  // Variavel de controle do movimento
int steps;                // Variavel resultado formula
int inicio = 0;           // Variavel controle para parar o giro do inicio 0 inicia 1 quebra o loop
bool estado;              // Variavel leitura inicial do sensor
int graus;

//AccelStepper Padrão com 4 pinos
AccelStepper stepper(AccelStepper::FULL4WIRE, pinA, pinB, pinC, pinD);


void setup(){

   stepper.setAcceleration(speedB);
   stepper.setMaxSpeed(speedC);
   stepper.setSpeed(speedA);
   
   estado = (digitalRead(pinE));
    
   Serial.begin(SP);
   Serial.println("");
   Serial.println(" ---- SERIAL START ---- ");
   Serial.println("");
}

void loop(){

startfun();

  while (Serial.available()>0){

    movimentopronto = 0;

    SerialString = Serial.readStringUntil('\n');   // Le o valor da Serial como String
    Serial.println("");
    Serial.println(" ------------------------------------- ");
    Serial.print(" I READ STRING ... ");
    Serial.println(SerialString);
    mySerial = SerialString.toInt();      // "toInt" transforma a String em Int
    Serial.print(" I READ INT ... ");
    Serial.println(mySerial);
    Serial.println(" ------------------------------------- ");

        
    if (SerialString == "Boot = 1"){
       ESP.reset(); 
    }
    if ((SerialString == "Info = 1") && (movimentopronto == 0)) {
        Serial.println(" ------------------------------------- ");
        Serial.print(" THE PLATE ARE IN STEPS ...  ");
        Serial.println(stepper.currentPosition());
        Serial.print(" THE PLATE ARE IN DEGRESS ... ");
        Serial.println(graus);
        Serial.println(" ------------------------------------- ");
        movimentopronto = 1;
    }
       
    

    
   if ((mySerial < MINAngle || mySerial > MAXAngle) && (movimentopronto == 0)) {
      Serial.println("");
      Serial.println(" -- ERROR -- INSERT A VALID NUMBER -- ERROR --");
      Serial.println("");
      movimentopronto = 1; //reset variavel
      
    }
      else if (movimentopronto == 0){
        
        steps = (RatioToTurns / 5.625) * (mySerial);
        Serial.println(" ------------------------------------- ");
        Serial.print(" ROTATION STEPS PROGRESS TO...  ");
        Serial.println(steps);
        Serial.print(" ROTATION DEGRESS PROGRESS TO... ");
        Serial.println(mySerial);
        Serial.println(" ------------------------------------- ");
        graus = mySerial;
        stepper.moveTo(steps);

      }
  }

      if (mySerial >= MINAngle && mySerial <= MAXAngle) {

        if ((stepper.distanceToGo() != 0)) {
            stepper.run();
        }

      if ((movimentopronto == 0) && (stepper.distanceToGo() == 0)) {
        stepper.disableOutputs();
        Serial.println(" ---------------------------------------------------------------------------------- ");
        Serial.println("                                 ROTATION COMPLETED");
        Serial.println("      INSERT DEGRESS TO ROTATE, Boot = 1 TO RESET, Info = 1 SHOW WHERE IS IT");
        Serial.println(" ---------------------------------------------------------------------------------- ");
        movimentopronto = 1; //reset variavel
      }
      }
}


void startfun(){  // Function for start 
  
  if((inicio == 0) and (estado == HIGH)){
        
        stepper.setSpeed(speedA);
        stepper.run();
          
        if (digitalRead(pinE) != estado){
            inicio = 1;
            stepper.disableOutputs();
            Serial.println("");
            Serial.println(" ---- ZERO DEGREE ----");
            stepper.setCurrentPosition(0);
            movimentopronto = 0;
            }
        else { }
        }
  
   if((inicio == 0) and (estado == LOW)) {
         
         stepper.setSpeed(-speedA);
         stepper.run();
           
         if (digitalRead(pinE) != estado){
             inicio = 1;
             stepper.disableOutputs();
             Serial.println("");
             Serial.println(" ---- ZERO DEGREE ----");
             stepper.setCurrentPosition(0);
             movimentopronto = 0;
             }    
        else { }
        }
}
