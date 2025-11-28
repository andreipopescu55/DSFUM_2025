#include <LiquidCrystal.h>

// Pinii LCD (modifică dacă e nevoie)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Definim două cadre (frames) pentru omuleț
byte man1[8] = {
  B00100,
  B01110,
  B00100,
  B01110,
  B10100,
  B00100,
  B01010,
  B10001
};

byte man2[8] = {
  B00100,
  B01110,
  B00100,
  B01110,
  B00101,
  B00100,
  B01010,
  B00001
};

void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, man1);
  lcd.createChar(1, man2);
  lcd.clear();
}

void loop() {
  // Omulețul "aleargă" de la dreapta spre stânga
  for (int pos = 15; pos >= 0; pos--) {
    lcd.clear();

    // Afișăm alternativ frame 1 și frame 2
    if (pos % 2 == 0)
      lcd.setCursor(pos, 0), lcd.write(byte(0));  // man1
    else
      lcd.setCursor(pos, 0), lcd.write(byte(1));  // man2

    delay(1000);  // viteza alergării
  }
}
