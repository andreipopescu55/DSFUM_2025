#include <LiquidCrystal.h>

// Conectarea LCD-ului (exemplu pentru un LCD 16x2)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

String text = "Bun venit la laboratorul 6!";   // textul care se derulează
int line = 0;                
int scrollDelay = 300;       // întârzierea între pași (ms)

int buttonPin = A0;
int readButton() {
  int val = analogRead(buttonPin);
  if (val < 50) return 1;      // RIGHT
  if (val < 150) return 2;     // UP
  if (val < 350) return 3;     // DOWN
  if (val < 550) return 4;     // LEFT
  if (val < 850) return 5;     // SELECT
  return 0;                    // nimic apăsat
}

void setup() {
  lcd.begin(16, 2);          // inițializează LCD-ul 16x2
  lcd.clear();
}

void loop() {

    int btn = readButton();
  if (btn == 2 && scrollDelay > 50) {      // UP -> mai repede
    scrollDelay -= 50;
    delay(200); // debounce
  }
  if (btn == 3 && scrollDelay < 1000) {    // DOWN -> mai încet
    scrollDelay += 50;
    delay(200); // debounce
  }


  for (int i = 0; i < text.length() + 16; i++) {
    lcd.setCursor(0, line);
    String frame = "";

    for (int j = 0; j < 16; j++) {
      int pos = i + j - 16;
      if (pos >= 0 && pos < text.length())
        frame += text[pos];
      else
        frame += " ";
    }

    lcd.print(frame);        // afișează fereastra curentă
    delay(scrollDelay);      // pauză între pași
    if(readButton() != 0) break;
  }

}
