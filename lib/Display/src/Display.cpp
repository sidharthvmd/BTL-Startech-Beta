#include "Display.h"

void Display::setup() {
    if (!oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    oledDisplay.setTextColor(SSD1306_WHITE);
}

void Display::displayWelcomeText(){
    oledDisplay.clearDisplay();
    oledDisplay.setTextSize(3);
    oledDisplay.setTextColor(SSD1306_WHITE);
    oledDisplay.setCursor(0,20);
    oledDisplay.print("Welcome");
    oledDisplay.display();
}


void Display::displayText(int &clockWiseAngle, int &antiClockWiseAngle, int &oscillations) {
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
    oledDisplay.display();
}

void Display::displayDuration(int &oscillations) {
    oledDisplay.setCursor(0, 50); 
    oledDisplay.print(thirdLine);
    oledDisplay.println(String(oscillations));
    oledDisplay.display();
}


void Display::displayAngle(const char* text, int& angle){
    oledDisplay.clearDisplay();
    oledDisplay.setTextSize(2);
    oledDisplay.setCursor(0,0);
    oledDisplay.print(text);
    oledDisplay.display();
    oledDisplay.setCursor(0,20);
    oledDisplay.print(String(angle));
    oledDisplay.display();
}

void Display::errorDisplay(String text){
    oledDisplay.setCursor(0,0);
    oledDisplay.print("Error Invalid");
    oledDisplay.println(text);
}
