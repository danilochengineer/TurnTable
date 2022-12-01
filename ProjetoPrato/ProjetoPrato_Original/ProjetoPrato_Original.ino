#include "AccelStepper.h"

//Declaração das variaveis

 
int var = 0;       // Variavel controle start
bool estado;       // Variavel leitura inicial do rele

int rotate = 900;
long leituraserial; // Variavel da Leitura Serial
int movimentopronto = 1; //Variavel de controle do movimento


//AccelStepper Padrão com 4 pinos
AccelStepper stepper(AccelStepper::FULL4WIRE, 16, 5, 4, 0);


void setup() {


   stepper.setMaxSpeed(900);
   stepper.setSpeed(rotate);
   stepper.setAcceleration(900);
   estado = (digitalRead(12));
   stepper.run();
  
   Serial.begin(115200);
   Serial.println("");
   Serial.println("---- PLATE ON ---- ");
   
   
}

void loop() {

inicio();


  while (Serial.available()>0) {

    movimentopronto = 0;

    leituraserial= Serial.parseInt(); // valor numerico buffer rodar
    
    if (leituraserial == 5000){
       ESP.reset(); 
    }
       
    if (leituraserial < -3000 || leituraserial > 3000) {
      Serial.println("");
      Serial.println("Insira valor - ou + não maior ou menor que 3000.....");
      Serial.println("");
      
    }
      else {
        Serial.print("Movendo os passos para a posição: ");
        Serial.println(leituraserial);
        
        stepper.moveTo(leituraserial);
        

        delay(50); // aguarda antes de mover 
      }
  }

      if (leituraserial >= -3000 && leituraserial <= 3000) {

        if ((stepper.distanceToGo() != 0)) {
            stepper.run();
        }

      if ((movimentopronto == 0) && (stepper.distanceToGo() == 0)) {
        stepper.disableOutputs();
        Serial.println("");
        Serial.println("---- Giro Completado----");
        Serial.println("");
        Serial.println("DIGITE UM NUMERO PARA RODAR POSITIVO OU NEGATIVO -> : ");
        movimentopronto = 1; //reset variavel
      }
      }
}


void inicio() {
  
  if((var == 0) and (estado == HIGH)){
        
        stepper.setSpeed(rotate);
        stepper.run();
        Serial.println(" Girando horario ");
  
        if (digitalRead(12) != estado){
            var = 1;
            stepper.disableOutputs();
            Serial.println(" ---- Angulo 0 -----");
            stepper.setCurrentPosition(0);
            movimentopronto = 0;
            }
        else { }
        }
  
   if((var == 0) and (estado == LOW)) {
         
         stepper.setSpeed(-rotate);
         stepper.run();
         Serial.println(" Girando Anti horario ");
  
         if (digitalRead(12) != estado){
             var = 1;
             stepper.disableOutputs();
             Serial.println(" ---- Angulo 0 -----");
             stepper.setCurrentPosition(0);
             movimentopronto = 0;
             }    
        else { }
        }
}
