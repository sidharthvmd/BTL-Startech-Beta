#include <Arduino.h>
#include "Display.h"
#include <Stepper.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {9, 8, 7, 6}; 


Display myDisplay;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Stepper myStepper(stepPin, directionPin);


// global constants adhering to stepper motion
static int clockWiseAngle = 30;
static int clockWiseStep = myStepper.convertAngleToSteps(clockWiseAngle);
static int antiClockWiseAngle = 40;
static int antiClockWiseStep = myStepper.convertAngleToSteps(antiClockWiseAngle);

static int oscillations = 5;
static int stepsElapsed;
static int speedOfPulse = 500;
static bool directionBool;

String clockWiseAngleInString = "";
String antiClockWiseAngleInString = "";

bool isStepperRunning = false;


void setup(){
Serial.begin(9600);
myDisplay.setup();
}


void loop() {
myStepper.moveStepper(clockWiseStep,antiClockWiseStep,stepsElapsed,oscillations,speedOfPulse);
}
