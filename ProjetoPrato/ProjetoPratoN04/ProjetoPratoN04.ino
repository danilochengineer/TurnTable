#include "AccelStepper.h"

//Declaração das variaveis

int pinE = 12;            // pino do sensor

int pinA = 16;            // pinos do motor
int pinB = 5;             // pinos do motor
int pinC = 4;             // pinos do motor
int pinD = 0;             // pinos do motor

int boot = 5000;          // Numero para dar Boot na placa
int sayposition = 4000;   // Numero para dizer a posição

int RatioToTurns = 224;   // Relação voltas do pinhao 1 volta = 32, 7 voltas = 224
int MaxAngle =  360;      // Maximum angle to turn.
int MinAngle = -360;      // Minimum angle to turn.

int speedA = 900;         // Rotação maxima do inicio seca sem rampa
int speedB = 900;         // Rotação maxima dos graus
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
    
   Serial.begin(115200);
   Serial.println("");
   Serial.println(" ---- SERIAL START ---- ");
   Serial.println("");
}

void loop(){

startfun();

  while (Serial.available()>0){

    movimentopronto = 0;

    SerialString = Serial.readString();   // Le o valor da Serial como String
    mySerial = SerialString.toInt();      // "toInt" transforma a String em Int
    
    if (mySerial == boot){
       ESP.reset(); 
    }
    if ((mySerial == sayposition) && (movimentopronto == 0)) {
        Serial.println(" ------------------------------ ");
        Serial.print(" WHE ARE IN STEPS ...  ");
        Serial.println(stepper.currentPosition());
        Serial.print(" WHE ARE IN DEGRESS ... ");
        Serial.println(graus);
        Serial.println(" ------------------------------ ");
        movimentopronto = 1;
    }
       
       
   if ((mySerial < MinAngle || mySerial > MaxAngle) && (movimentopronto == 0)) {
      Serial.println("");
      Serial.println(" -- ERROR -- INSERT A VALID NUMBER -- ERROR --");
      Serial.println("");
      
    }
      else if (movimentopronto == 0){
        
        steps = (RatioToTurns / 5.625) * (mySerial);
        Serial.println(" ------------------------------ ");
        Serial.print(" ROTATION STEPS PROGRESS TO...  ");
        Serial.println(steps);
        Serial.print(" ROTATION DEGRESS PROGRESS TO... ");
        Serial.println(mySerial);
        Serial.println(" ------------------------------ ");
        graus = mySerial;
        stepper.moveTo(steps);

        //delay(10); // aguarda antes de mover 
      }
  }

      if (mySerial >= MinAngle && mySerial <= MaxAngle) {

        if ((stepper.distanceToGo() != 0)) {
            stepper.run();
        }

      if ((movimentopronto == 0) && (stepper.distanceToGo() == 0)) {
        stepper.disableOutputs();
        Serial.println("");
        Serial.println(" ---- ROTATION COMPLETED----");
        Serial.println("");
        Serial.println(" ---- INSERT DEGRESS TO ROTATE OR 5000 TO RESET -> : ");
        Serial.println("");
        movimentopronto = 1; //reset variavel
      }
      }
}

void startfun(){
  
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
