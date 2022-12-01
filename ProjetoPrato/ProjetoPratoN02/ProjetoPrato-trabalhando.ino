#include "AccelStepper.h"

//Declaração das variaveis


int inicio = 0;    // Variavel controle para parar o giro do inicio 0 inicia 1 quebra o loop
bool estado;       // Variavel leitura inicial do sensor

int pinE = 12;                                  // pino do sensor
int pinA = 16, pinB = 5, pinC = 4, pinD = 0;    // pinos do motor

int RatioToTurns = 224;    // Relação voltas do pinhao 1 volta = 32, 7 voltas = 224

int speedA = 900;         // Rotação maxima do inicio seca sem rampa
int speedB = 900;         // Rotação maxima dos graus
int speedC = 900;         // Rotação maxima dos graus rampa

int leituraserial;        // Variavel da Leitura Serial
int movimentopronto = 1;  // Variavel de controle do movimento
int steps;                // Variavel resultado formula


//AccelStepper Padrão com 4 pinos
AccelStepper stepper(AccelStepper::FULL4WIRE, pinA, pinB, pinC, pinD);


void setup() {

   stepper.setAcceleration(speedB);
   stepper.setMaxSpeed(speedC);
   stepper.setSpeed(speedA);
   
   estado = (digitalRead(pinE));
    
   Serial.begin(115200);
   Serial.println("");
   Serial.println(" ---- SERIAL START ---- ");
   Serial.println("");
   
   
}

void loop() {

startfun();


  while (Serial.available()>0) {

    movimentopronto = 0;

   leituraserial = Serial.parseInt(); // valor numerico buffer rodar
    
    if (leituraserial == 5000){
       ESP.reset(); 
    }
       
    if (leituraserial < -360 || leituraserial > 360) {
      Serial.println("");
      Serial.println(" ---- ERROR -- INSERT A VALID NUMBER -- ERROR --");
      Serial.println("");
      
    }
      else {
        
        steps = (RatioToTurns / 5.625) * (leituraserial);
        
        Serial.print(" ---- ROTATION IN STEPS PROGRESS...  ");
        Serial.println(steps);
        Serial.print(" ---- ROTATION IN DEGRESS PROGRESS... ");
        Serial.println(leituraserial);
       
        stepper.moveTo(steps);

        delay(50); // aguarda antes de mover 
      }
  }

      if (leituraserial >= -360 && leituraserial <= 360) {

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


void startfun() {
  
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
