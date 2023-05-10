#include "AccelStepper.h"

//Variable Declaration

int pinE = 4;            //Sensor Pin.

int pinA = 14;            //Engine PinA.
int pinB = 13;            //Engine PinB.
int pinC = 12;            //Engine PinC.
int pinD = 15;            //Engine PinD.

int SP = 115200;          //Serial Speed.

int RatioToTurns = 224;   // Motor axle ratio per revolutions, one turn = 32, seven turns = 224.
int MAXAngle  =  170;     // Maximum angle positive to turn zero to anticlockwise.
int MAXAngle2 =  170;     // Maximum angle positive to turn zero to anticlockwise.

int MINAngle  = -170;     // Minimum angle degress negative - zero to clockwise.
int MINAngle2 =  190;     // Minimum angle degrees to turn  - zero to clockwise.

int speedA = 300;         // Maximum dry start speed without ramp.
int speedB = 300;          // Maximum rotation degrees engine ramp.
int speedC = 300;         // Maximum speed rotation of degrees.

String SerialString;      // Variable Read serial string.
int mySerial;             // Variable converted string to Integer.

int variablecontrol = 1;  // Variable moviment control.
int steps;                // Variable operation result.
int searchforzero = 0;    // Variable control to stop the loop engine looking for 0 degree.
bool condition;           // Variable inicial read of sensor.
int degree;               // variable return degrees.


//AccelStepper standard with 4 pins
AccelStepper stepper(AccelStepper::FULL4WIRE, pinA, pinB, pinC, pinD);


void setup(){

  stepper.setSpeed(speedA);
  stepper.setAcceleration(speedB);
  stepper.setMaxSpeed(speedC);
   
   
  condition = (digitalRead(pinE));
    
  Serial.begin(SP);
  Serial.println("");
  Serial.print("TurnTable>");
  Serial.println(" ON - Serial Start ");
  }

void loop(){

findzero();

  while (Serial.available() > 0){
    

    variablecontrol = 0;

    SerialString = Serial.readStringUntil('\n');    // Read Serial as String.
    mySerial = SerialString.toInt();                // Convert string to Integer "toInt".

        
    if (SerialString == "600"){
       ESP.restart(); 
    }
    
    if ((SerialString == "500") && (variablecontrol == 0)) {
        if (degree < 0) {          
        Serial.println("500");
        Serial.print("TurnTable>Angle:");
        Serial.println(360 + degree);
        Serial.print("TurnTable>");     
        variablecontrol = 1;
          } else {
        Serial.println("500");
        Serial.print("TurnTable>Angle:");
        Serial.println(degree);
        Serial.print("TurnTable>");     
        variablecontrol = 1;
        }     
    }

    if ((SerialString == "999") && (variablecontrol == 0)) {
        Serial.println("999");
        Serial.println("TurnTable>Table Available");
        Serial.print("TurnTable>");
        variablecontrol = 1;
    }

    if ((SerialString == "400") && (variablecontrol == 0)) {
        Serial.println("400");
        Serial.print("TurnTable>Serial:");
        Serial.println(ESP.getSdkVersion());
        Serial.print("TurnTable>");
        variablecontrol = 1;
    }
       
    
   if ((mySerial >= MINAngle2 && mySerial <= 360) && (variablecontrol == 0)) {
        mySerial = (mySerial - 360);
        //Serial.println(mySerial);
    }
    
   if (((mySerial < MINAngle || mySerial > MAXAngle) || (mySerial > MINAngle2 || mySerial > MAXAngle2)) && (variablecontrol == 0)) {
      Serial.println("Error Insert Valid Number");
      Serial.print("TurnTable>");
      variablecontrol = 1; //reset variable
      
    }
      else if (variablecontrol == 0){
        
        steps = (RatioToTurns / 5.625) * (mySerial);
        degree = mySerial;
          if (degree < 0) {          
        Serial.print("RotateTo:");
        Serial.println(360 + degree);
        Serial.print("TurnTable>");
        stepper.moveTo(steps);
          } else {
        Serial.print("RotateTo:");
        Serial.println(degree);
        Serial.print("TurnTable>");
        stepper.moveTo(steps);
        }

      }
  }

      if (mySerial >= MINAngle && mySerial <= MAXAngle) {

        if ((stepper.distanceToGo() != 0)) {
            stepper.run();
        }

      if ((variablecontrol == 0) && (stepper.distanceToGo() == 0)) {
        stepper.disableOutputs();
          if (degree < 0) {
        Serial.print("Angle:");
        Serial.println(360 + degree);
        Serial.println("TurnTable>Rotation Completed");
        Serial.print("TurnTable>");
          } else {
        Serial.print("Angle:");
        Serial.println(degree);
        Serial.println("TurnTable>Rotation Completed");
        Serial.print("TurnTable>");
          }
       
      
        variablecontrol = 1; //reset variable
      }
      }
}


void findzero(){  // Function to search position 0 
  
  if((searchforzero == 0) and (condition == HIGH)){
        
        stepper.setSpeed(speedA);
        stepper.run();
          
        if (digitalRead(pinE) != condition){
            searchforzero = 1;
            stepper.disableOutputs();
            Serial.print("TurnTable>");
            stepper.setCurrentPosition(0);
            variablecontrol = 0;
            }
        else { }
        }
  
   if((searchforzero == 0) and (condition == LOW)) {
         
         stepper.setSpeed(-speedA);
         stepper.run();
           
         if (digitalRead(pinE) != condition){
             searchforzero = 1;
             stepper.disableOutputs();
             Serial.print("TurnTable>");
             stepper.setCurrentPosition(0);
             variablecontrol = 0;
             }    
        else { }
        }
}
