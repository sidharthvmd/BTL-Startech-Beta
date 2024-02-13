#include "Display.h"

void Display::setup() {
    if (!oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
}

void Display::displayText(const char *line1, const char *line2) {
    oledDisplay.clearDisplay();
    
    oledDisplay.setTextSize(2);
    oledDisplay.setTextColor(SSD1306_WHITE);

    oledDisplay.setCursor(0, 0);
    oledDisplay.print(line1);

    oledDisplay.setCursor(0, 20); 
    oledDisplay.print(line2);

    oledDisplay.display();
}

void Display::displayText(String line1, String angle) {
    oledDisplay.clearDisplay();
    oledDisplay.setTextSize(3);
    oledDisplay.setTextColor(SSD1306_WHITE);

    oledDisplay.setCursor(0, 0);
    oledDisplay.print(firstLine);
    oledDisplay.println(String(angle));
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
}

void Display::displayDuration(int &oscillations) {
    oledDisplay.setCursor(0, 50); 
    oledDisplay.print(thirdLine);
    oledDisplay.println(String(oscillations));
    oledDisplay.display();
}
