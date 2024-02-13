#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "constants.h"
#include<Keypad.h>

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//There's a reason class is declared before global variables
class Stepper{
  public:
  Stepper(){
    pinMode(stepPin, OUTPUT);
    pinMode(directionPin, OUTPUT);
  }
    void moveStepperRight(int &totalSteps, int &stepsElapsed);
    void moveStepperLeft(int &totalSteps, int &stepsElapsed);
    int convertAngleToSteps(int& angle);
    void moveStepper(int &rightTotalSteps,int &leftTotalSteps, int &stepsElapsed, int &oscillations);
};


Stepper myStepper;

/* void getPotValue(){
  int clockWiseAngleRaw = analogRead(A0);
  int clockWiseAngleVoltage = clockWiseAngleRaw * (5.0/1024);
  int clockWiseAngle = map(clockWiseAngleVoltage, 0, 5, 0, 360);
  Serial.println(clockWiseAngle);
  delay(1000);
} */

// global constants adhering to stepper motion
static int clockWiseAngle = 30;
static int clockWiseStep = myStepper.convertAngleToSteps(clockWiseAngle);
static int antiClockWiseAngle = 40;
static int antiClockWiseStep = myStepper.convertAngleToSteps(antiClockWiseAngle);

static int oscillations = 5;
static int stepsElapsed;
static int speedOfPulse = 500;
static bool directionBool;

String angleInString = "";

enum State {
  ENTER_CLOCKWISE,
  ENTER_ANTICLOCKWISE,
  CONFIRMATION,
  IDLE
};

bool isStepperRunning = false;
State currentState = IDLE;

const char firstLine[] = "CW : ";
const char secondLine[] = "CCW : ";
const char thirdLine[] = "Duration : "; 

class Display {
private:
  Adafruit_SSD1306 oledDisplay;

public:
  Display() : oledDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

  void setup() {
    if (!oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("SSD1306 allocation failed"));
      for (;;);
    }
  }

  void displayText(const char *line1, const char *line2) {
    oledDisplay.clearDisplay();
    
    oledDisplay.setTextSize(2);
    oledDisplay.setTextColor(SSD1306_WHITE);

    oledDisplay.setCursor(0, 0);
    oledDisplay.print(line1);
  

    oledDisplay.setCursor(0, 20); 
    oledDisplay.print(line2);

    oledDisplay.display();
  }

  void displayText() {
    oledDisplay.clearDisplay();
    
    oledDisplay.setTextSize(2);
    oledDisplay.setTextColor(SSD1306_WHITE);

    oledDisplay.setCursor(0, 0);
    oledDisplay.print(firstLine);
    oledDisplay.println(String(clockWiseAngle));

    oledDisplay.setCursor(0, 20); 
    oledDisplay.print(secondLine);
    oledDisplay.println(String(antiClockWiseAngle));

    oledDisplay.setTextSize(1);
    oledDisplay.setCursor(0, 50); 
    oledDisplay.print(thirdLine);
    oledDisplay.println(String(oscillations));
  }

  void displayDuration(){
    oledDisplay.setCursor(0, 50); 
    oledDisplay.print(thirdLine);
    oledDisplay.println(String(oscillations));
    oledDisplay.display();
  }
};

Display myDisplay;

class Event {
public:
  void handleKeypadInput() {
    if (!isStepperRunning) {
      char key = keypad.getKey();

      if (key) {
        switch (key) {
          case 'A':
            currentState = ENTER_CLOCKWISE;
            break;
          case 'B':
            currentState = ENTER_ANTICLOCKWISE;
            break;
          case 'C':
            currentState = CONFIRMATION;
            break;
          case 'D':
            resetStepper(myStepper);
            currentState = IDLE;
            break;
          case '#':
            clearInput();
            break;
          default:
            updateInput(key);
            break;
        }
      }
    }
  }

  void handleState();
  private:
    void clearInput();
    void  updateInput(int key);
    void resetStepper(class &stepper);
};


void setup(){
  Serial.begin(9600);
  myDisplay.setup();
}

Event myEvent;

void loop() {
  Event.handleKeyPadInput();
  Event.handleState();
}

void Stepper::moveStepperRight(int &totalSteps, int &stepsElapsed ){
  digitalWrite(directionPin, HIGH);
  for (; stepsElapsed<=totalSteps; stepsElapsed++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speedOfPulse);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speedOfPulse);
  }

  digitalWrite(directionPin,LOW);
  for (; stepsElapsed!=0; stepsElapsed--) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speedOfPulse);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speedOfPulse);
  }
}

void Stepper::moveStepperLeft(int &totalSteps, int &stepsElapsed ){
  digitalWrite(directionPin, LOW);
  for (; stepsElapsed<=totalSteps; stepsElapsed++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speedOfPulse);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speedOfPulse);
  }

  digitalWrite(directionPin,HIGH);
  for (; stepsElapsed!=0; stepsElapsed--) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speedOfPulse);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speedOfPulse);
  }
}


int Stepper::convertAngleToSteps(int &angle){
  return floor(map(angle,0,360,0,6400));
}

void Stepper::moveStepper(int &rightTotalSteps,int &leftTotalSteps, int &stepsElapsed, int &oscillations){
  while(oscillations!=0){
    moveStepperRight(rightTotalSteps,stepsElapsed);
    stepsElapsed=0;
    moveStepperRight(leftTotalSteps,stepsElapsed);
    stepsElapsed=0;
    oscillations--;
    }
}