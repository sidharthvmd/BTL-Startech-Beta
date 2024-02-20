#include <Arduino.h>
#include "Display.h"
#include <Stepper.h>
#include <Keypad.h>

//keypad constants
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

byte rowPins[ROWS] = {11, 10, 9, 8}; 
byte colPins[COLS] = {7, 6, 5, 4};

//class declarations
Display myDisplay;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Stepper myStepper(stepPin, directionPin);


// global constants adhering to stepper motion
static int clockWiseAngle = 360;
static int clockWiseStep = myStepper.convertAngleToSteps(clockWiseAngle);
static int antiClockWiseAngle = 360;
static int antiClockWiseStep = myStepper.convertAngleToSteps(antiClockWiseAngle);

static int oscillations = 5;
static int stepsElapsed =0;
static int speedOfPulse = 500;
//static bool directionBool;

//String clockWiseAngleInString = "";
//String antiClockWiseAngleInString = "";

bool isStepperRunning = false;

class Events{
  public:
    bool checkAngle(int &angle){
    if (angle>360||angle<0){
        return false;
    }
    else if (angle>0 && angle<=360)
        return true;
  }

void updateGlobalDataWithKeypad(int &outsideValue, const char* displayText) {
    myDisplay.displayAngle(displayText, outsideValue);
    int result = 0;
    outsideValue = 0;
    char key;
    int insideValue;
    bool confirmed = false;

    while (true) {
        key = keypad.getKey();

        if (key) {
            if (key == '#') { // Terminate input on '#' key
                if (!confirmed) {
                    myDisplay.displayAngle("CW", outsideValue);
                    myDisplay.displayDuration("Confirm ?");
                    delay(1000);  // Add a delay for user confirmation
                    char confirmKey = keypad.getKey();
                    
                    if (confirmKey == '#') {
                        confirmed = true;
                        break;
                    } else {
                        // Reset if not confirmed
                        myDisplay.displayAngle(displayText, outsideValue);
                        result = 0;
                        outsideValue = 0;
                        continue;
                    }
                } else {
                    // Exit if already confirmed
                    break;
                }
            }

            if (key == '*') { // Clear input on '*' key
                outsideValue = 0;
                insideValue = 0;
                result = 0;
                confirmed = false;
                myDisplay.displayAngle(displayText, outsideValue);
                continue;
            }

            insideValue = key - '0';
            result = (result * 10) + insideValue;
            outsideValue = result;
            myDisplay.displayAngle("CW", outsideValue);
        }
    }
}

};


Events myEvent;

void setup(){
  Serial.begin(9600);
  myDisplay.setup();
  myDisplay.displayWelcomeText();
}


void loop() {
  //myEvent.updateGlobalDataWithKeypad(clockWiseAngle,"CW");
  //myEvent.updateGlobalDataWithKeypad(antiClockWiseAngle,"CCW");
  isStepperRunning = true;
  //clockWiseStep = myStepper.convertAngleToSteps(clockWiseAngle); 
  //antiClockWiseStep = myStepper.convertAngleToSteps(antiClockWiseAngle);
  myStepper.allIn(speedOfPulse);
  //myStepper.moveStepper(clockWiseStep,antiClockWiseStep,stepsElapsed,oscillations,speedOfPulse);
}
