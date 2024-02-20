#include <Arduino.h>
//#include "Display.h"
#include<Adafruit_SSD1306.h>
#include <Stepper.h>
#include <Keypad.h>
#include <SPI.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET     -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

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

    bool checkDuration(int &angle){
    if (angle<=0){
        return false;
    }
    else
        return true;
  }

void updateGlobalDataWithKeypad(int &outsideValue, const char* displayText) {
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(displayText);
    display.print(": ");
    display.print(outsideValue);
    display.display();

    int result = 0;
    char key;
    int insideValue;
    bool confirmed = false;
    bool angle = false;
    bool valid = false;

    while (true) {
        key = keypad.getKey();

        if (key) {
            if (key == '#') {
                confirmed = true;
            } else if (key == '*') {
                outsideValue = 0;
                insideValue = 0;
                result = 0;
                confirmed = false;
            } else {
                insideValue = key - '0';
                result = (result * 10) + insideValue;
                outsideValue = result;
            }

            display.clearDisplay();
            display.setTextSize(3);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 0);
            display.print(displayText);
            display.print(":");
            display.print(outsideValue);
            display.display();

            angle = checkAngle(outsideValue);

            if (confirmed) {
                if (angle) {
                    display.clearDisplay();
                    display.setTextSize(3);
                    display.setTextColor(SSD1306_WHITE);
                    display.setCursor(0, 0);
                    display.print("Confirm?");
                    display.print(": ");
                    display.print(outsideValue);
                    display.display();

                    while (true) {
                        key = keypad.getKey();
                        if (key == '#') {
                            valid = true;
                            break;
                        } else if (key == '*') {
                            outsideValue = 0;
                            insideValue = 0;
                            result = 0;
                            confirmed = false;
                            break;
                        }
                    }
                }

                if (valid) {
                    break;
                } else {
                    display.clearDisplay();
                    display.setTextSize(3);
                    display.setTextColor(SSD1306_WHITE);
                    display.setCursor(0, 0);
                    display.print("Invalid");
                    display.display();
                    confirmed = false;
                }
            }
        }
    }
}


void updateDuration(int &outsideValue) {
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("OSC");
    display.print(":");
    display.print(outsideValue);
    display.display();

    int result = 0;
    char key;
    int insideValue;
    bool confirmed = false;
    bool angle = false;
    bool valid = false;

    while (true) {
        key = keypad.getKey();

        if (key) {
            if (key == '#') {
                confirmed = true;
            } else if (key == '*') {
                outsideValue = 0;
                insideValue = 0;
                result = 0;
                confirmed = false;
            } else {
                insideValue = key - '0';
                result = (result * 10) + insideValue;
                outsideValue = result;
            }

            display.clearDisplay();
            display.setTextSize(3);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 0);
            display.print("OSC");
            display.print(": ");
            display.print(outsideValue);
            display.display();

            angle = checkDuration(outsideValue);

            if (confirmed) {
                if (angle) {
                    display.clearDisplay();
                    display.setTextSize(3);
                    display.setTextColor(SSD1306_WHITE);
                    display.setCursor(0, 0);
                    display.print("Confirm?");
                    display.print(": ");
                    display.print(outsideValue);
                    display.display();

                    while (true) {
                        key = keypad.getKey();
                        if (key == '#') {
                            valid = true;
                            break;
                        } else if (key == '*') {
                            outsideValue = 0;
                            insideValue = 0;
                            result = 0;
                            confirmed = false;
                            break;
                        }
                    }
                }

                if (valid) {
                    break;
                } else {
                    display.clearDisplay();
                    display.setTextSize(3);
                    display.setTextColor(SSD1306_WHITE);
                    display.setCursor(0, 0);
                    display.print("Invalid");
                    display.print(": ");
                    display.print(outsideValue);
                    display.display();
                    confirmed = false;
                }
            }
        }
    }
}


  void displayText(int &clockWiseAngle, int &antiClockWiseAngle, int &oscillations) {
      display.clearDisplay();
      
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);

      display.setCursor(0, 0);
      display.print("CW : ");
      display.println(String(clockWiseAngle));

      display.setCursor(0, 20); 
      display.print("CCW :");
      display.println(String(antiClockWiseAngle));

      display.setTextSize(1);
      display.setCursor(0, 50); 
      display.print("OSC :");
      display.println(String(oscillations));
      display.display();
  }

  void displayText(int &oscillations) {
      display.clearDisplay();
      
      display.setTextSize(3);
      display.setTextColor(SSD1306_WHITE);

      display.setCursor(0, 0);
      display.print("OSC:");
      display.println(String(oscillations));
      display.display();
  }




};


Events myEvent;

void setup(){
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}


void loop() {
  myEvent.updateGlobalDataWithKeypad(clockWiseAngle,"CW");
  myEvent.updateGlobalDataWithKeypad(antiClockWiseAngle,"CCW");
  myEvent.updateDuration(oscillations);
  isStepperRunning = true;
  myEvent.displayText(clockWiseAngle,antiClockWiseAngle,oscillations);
  if (clockWiseAngle==360 && antiClockWiseAngle==360){
    myStepper.allIn(speedOfPulse,oscillations,stepsElapsed);
  }
  else{
    clockWiseStep = myStepper.convertAngleToSteps(clockWiseAngle);
    antiClockWiseStep = myStepper.convertAngleToSteps(antiClockWiseAngle);
      while(oscillations!=0){
      myStepper.moveStepperRight(clockWiseStep,stepsElapsed, speedOfPulse);
      stepsElapsed=0;
      myStepper.moveStepperLeft(antiClockWiseStep,stepsElapsed, speedOfPulse);
      stepsElapsed=0;
      oscillations--;
      myEvent.displayText(oscillations);
    }
}
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(3);
  display.setCursor(0,0);
  display.println("Done");
  display.display();
  delay(2000);
}
