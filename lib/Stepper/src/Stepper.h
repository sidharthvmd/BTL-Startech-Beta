#ifndef STEPPER
#define STEPPER

#include <Arduino.h>

extern int directionPin;
extern int stepPin;
extern int stepDivision;
const int stepsForFullRevolution = 6400;


class Stepper{
  public:
  Stepper(int &stepPin, int& directionPin){
    pinMode(stepPin, OUTPUT);
    pinMode(directionPin, OUTPUT);
  }
    void moveStepperRight(int &totalSteps, int &stepsElapsed, int &speed);
    void moveStepperLeft(int &totalSteps, int &stepsElapsed, int &speed);
    int convertAngleToSteps(int& angle);
    void allIn(int &speed);
    void moveStepper(int &rightTotalSteps,int &leftTotalSteps, int &stepsElapsed, int &oscillations, int &speed);
};


#endif