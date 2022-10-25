#include "AccelStepper.h"

//Variable Declaration

int pinE = 13;            //Sensor Pin

int pinA = 5;             //Motor PinA
int pinB = 14;             //Motor PinB
int pinC = 4;            //Motor PinC
int pinD = 12;            //Motor PinD

int SP = 115200;          //Serial Speed

int RatioToTurns = 224;   // Motor axle ratio per revolutions, one turn = 32, seven turns = 224
int MAXAngle =  170;      // Maximum angle to turn.
int MINAngle = -170;      // Minimum angle to turn.

int speedA = 400;         // Maximum dry start speed without ramp
int speedB = 400;         // Maximum speed rotation of degrees
int speedC = 400;         // Rotação maxima dos graus rampa

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
   Serial.print("TurnTable>");
   Serial.println("ON - Serial Start ");
   }

void loop(){

startfun();

  while (Serial.available() > 0){

    movimentopronto = 0;

    SerialString = Serial.readStringUntil('\n');   // Le o valor da Serial como String
    mySerial = SerialString.toInt();      // "toInt" transforma a String em Int
    
    

        
    if (SerialString == "600"){
       ESP.reset(); 
    }
    
    if ((SerialString == "500") && (movimentopronto == 0)) {
        Serial.println("500");
        Serial.print("TurnTable>Angle:");
        Serial.println(graus);
        Serial.print("TurnTable>");     
        movimentopronto = 1;
    }

    if ((SerialString == "999") && (movimentopronto == 0)) {
        Serial.println("999");
        Serial.println("TurnTable>Table Available");
        Serial.print("TurnTable>");
        movimentopronto = 1;
    }
       
    

    
   if ((mySerial < MINAngle || mySerial > MAXAngle) && (movimentopronto == 0)) {
      Serial.println("Error Insert Valid Number");
      Serial.print("TurnTable>");
      movimentopronto = 1; //reset variavel
      
    }
      else if (movimentopronto == 0){
        
        steps = (RatioToTurns / 5.625) * (mySerial);
        graus = mySerial;
        Serial.print("RotateTo:");
        Serial.println(graus);
        Serial.print("TurnTable>");
        stepper.moveTo(steps);

      }
  }

      if (mySerial >= MINAngle && mySerial <= MAXAngle) {

        if ((stepper.distanceToGo() != 0)) {
            stepper.run();
        }

      if ((movimentopronto == 0) && (stepper.distanceToGo() == 0)) {
        stepper.disableOutputs();
        Serial.print("Angle:");
        Serial.println(graus);
        Serial.println("TurnTable>Rotation Completed");
        Serial.print("TurnTable>");
        
       
      
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
            Serial.print("TurnTable>");
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
             Serial.print("TurnTable>");
             stepper.setCurrentPosition(0);
             movimentopronto = 0;
             }    
        else { }
        }
}
