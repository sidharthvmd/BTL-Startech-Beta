#include "Stepper.h"

int directionPin = 2;
int stepPin = 3;
int stepDivision;

void Stepper::moveStepperRight(int &totalSteps, int &stepsElapsed, int &speed ){
  digitalWrite(directionPin, HIGH);
  for (; stepsElapsed<=totalSteps; stepsElapsed++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }

  digitalWrite(directionPin,LOW);
  for (; stepsElapsed!=0; stepsElapsed--) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
}

void Stepper::moveStepperLeft(int &totalSteps, int &stepsElapsed, int &speed ){
  digitalWrite(directionPin, LOW);
  for (; stepsElapsed<=totalSteps; stepsElapsed++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }

  digitalWrite(directionPin,HIGH);
  for (; stepsElapsed!=0; stepsElapsed--) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
}

int Stepper::convertAngleToSteps(int &angle){
  return floor(map(angle,0,360,0,6400));
}

void Stepper::moveStepper(int &rightTotalSteps,int &leftTotalSteps, int &stepsElapsed, int &oscillations, int &speed){
  while(oscillations!=0){
    moveStepperRight(rightTotalSteps,stepsElapsed, speed);
    stepsElapsed=0;
    moveStepperLeft(leftTotalSteps,stepsElapsed, speed);
    stepsElapsed=0;
    oscillations--;
    }
}

void Stepper::allIn(int &speed){
  digitalWrite(directionPin, HIGH);
  while (true) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
}