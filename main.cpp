#include <Arduino.h>
#include "Stepper.h"
#include "TinyStepLib.h"

using namespace zmdev;

//-----------------------------
// const uint8_t X_DIR = 5; 
// const uint8_t X_STP = 2; 
// const uint8_t Y_DIR = 6; 
// const uint8_t Y_STP = 3; 
// const uint8_t Z_DIR = 7; 
// const uint8_t Z_STP = 4; 
const String X_DIR = "PD5"; //PD5
const String X_STP = "PD2"; //PD2
const String Y_DIR = "PD6"; //PD6
const String Y_STP = "PD3"; //PD3
const String Z_DIR = "PD7"; //PD7
const String Z_STP = "PD4"; //PD4
//-----------------------------
TinyStepLib driver;
Stepper* stepper_1;
Stepper* stepper_2;
Stepper* stepper_3;

//-----------------------------
const uint8_t numChars = 32; //max number of command chars
char receivedChars[numChars]; 
char tempChars[numChars];
char instruction[4] = {0};
char argument[7] = {0};
//char* instruction;
//char* arg;
int  argVal = 0;
bool newData = false;
//-----------------------------


//declaring funcs:
void recvCommand();
void parseCommand();
void execCommand();
void moveStepperWith(char* stepper,int val);
void setStepperSpeed(char* stepper,int val);
void setStepperDir(char* stepper,int val);
void stopStepper(char* stepper);
void clearBuffer(){
  while(Serial.available()) Serial.read();
}



void setup() { 

  //TODO: 
  // - ADD SLEEP FUNCTIONALLITY TO IMPROVE POWER CONSUMPTION
  // - ADD SPEED CONTROL
  // - MAKE A PROCESSING SKETCH TO PROVIDE A GUI

  Serial.begin(9600);

  stepper_1 = new Stepper(X_STP,X_DIR,360,1.8/16.0,0,0);
  stepper_2 = new Stepper(Y_STP,Y_DIR,180,1.8/16.0,0,0);
  //stepper_3 = new Stepper(Z_STP,Z_DIR,360,1.8/16.0,0,0);

  stepper_1->setOnTargetReachedCallback([]{
    //this is a callback, it triggers when stepper 1 reaches its target angle
    Serial.println("Stepper 1 has reached its target angle");
  });

  stepper_2->setOnTargetReachedCallback([]{
    //this is a callback, it triggers when stepper 1 reaches its target angle
    Serial.println("Stepper 2 has reached its target angle");
  });

  //setup the driver, this initializes TIMER2
  driver.begin(); 

  //attach the motors
  driver.attach(stepper_1);
  driver.attach(stepper_2);
  //driver.attach(stepper_3);


}

void loop() {

  recvCommand();
  if(newData){
    parseCommand();
    execCommand();
    newData = false;
  }

}


//----------------------------------------------
void recvCommand(){
  static boolean recvInProgress = false ;
  static uint8_t index = 0;
  char startMarker = '>';
  char endMarker   = '\n';
  char in;

  while(Serial.available() && newData == false){
      in = Serial.read();

      if(recvInProgress)
      {
        if(in != endMarker)
        {
          receivedChars[index] = in;
          index++;
          if(index > numChars){
            Serial.println("Command longer than expected !");
            clearBuffer();
            index = 0;
          }
        }
        else
        {
          receivedChars[index] = '\0'; //terminate the string
          recvInProgress = false;
          newData = true;
          index = 0;
        }
        
      }
      else if(in == startMarker)
      {
        recvInProgress = true;
      }

  }
}

void parseCommand(){

  char* buff;
  strcpy(tempChars, receivedChars); //tempCars is needed to protect the original command in case needed later


  buff = strtok(tempChars," ");
  strcpy(instruction,buff);

  buff = strtok(NULL," ") ;
  strcpy(argument,buff);
 
  buff = strtok(NULL," ") ;
  argVal    = atoi(buff);

}

void execCommand(){
  
  if(strcmp(instruction,"mov") == 0)
  {
    moveStepperWith(argument,argVal);
  }
  else if(strcmp(instruction,"dir") == 0)
  {
    setStepperDir(argument,argVal);
  }
  else if(strcmp(instruction,"spd") == 0)
  {
    setStepperSpeed(argument,argVal);
  }
  else if(strcmp(instruction,"stp") == 0)
  {
    stopStepper(argument);
  }

}


//----------------------------------------------
void moveStepperWith(char* stepper,int val){
  if(strcmp(stepper,"stpr1") == 0) stepper_1->moveWith(val);
  else if(strcmp(stepper,"stpr2") == 0) stepper_2->moveWith(val);
  //else if(strcmp(stepper,"stpr3") == 0) stepper_3->moveWith(val);

}

void setStepperDir(char* stepper,int val){
  if(strcmp(stepper,"stpr1") == 0) stepper_1->setDirection(val);
  else if(strcmp(stepper,"stpr2") == 0) stepper_2->setDirection(val);
  //else if(strcmp(stepper,"stpr3") == 0) stepper_3->setDirection(val);
}

void setStepperSpeed(char* stepper,int val){
  if(strcmp(stepper,"stpr1") == 0) stepper_1->setSpeed(val);
  else if(strcmp(stepper,"stpr2") == 0) stepper_2->setSpeed(val);
  //else if(strcmp(stepper,"stpr3") == 0) stepper_3->setMaxSpeed(val);
}

void stopStepper(char* stepper){
  if(strcmp(stepper,"stpr1") == 0) stepper_1->stop();
  else if(strcmp(stepper,"stpr2") == 0) stepper_2->stop();
  //else if(strcmp(stepper,"stpr3") == 0) stepper_3->stop();
}
