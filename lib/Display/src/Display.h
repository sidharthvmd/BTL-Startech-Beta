#ifndef DISPLAY_H
#define DISPLAY_H

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET     -1
#include <Adafruit_SSD1306.h>

const char firstLine[] = "CW : ";
const char secondLine[] = "CCW : ";
const char thirdLine[] = "Duration : "; 

class Display {
private:
  Adafruit_SSD1306 oledDisplay;

public:
    Display() : oledDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}
    void setup();
    void displayText(int &clockWiseAngle, int &antiClockWiseAngle, int &oscillations);
    void displayDuration(int &oscillations);
    void displayWelcomeText();;
    void displayAngle(const char* text, int& angle);
    void errorDisplay(String text);
};


#endif