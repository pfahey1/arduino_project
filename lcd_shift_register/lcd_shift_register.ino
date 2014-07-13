#include <LiquidCrystal595.h> // include the library

LiquidCrystal595 lcd(7,8,9);     // datapin, latchpin, clockpin

void setup() {
    lcd.begin(16,2);             // 16 characters, 2 rows

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Wow. 3 pins!");
    lcd.setCursor(0,1);
    lcd.print("Fabulous");
}

void loop() {
    // not used.
}
