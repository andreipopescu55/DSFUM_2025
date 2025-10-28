
#define p_tip     8   // ieșire principală (buzzer/difuzor)
#define p_r1      9   // opțional – poți folosi pentru alt canal sau LED
#define p_r2      10  // opțional
#define p_sleeve  11  // GND comun (de regulă conectat la GND fizic)

// Frecvențe note (octava 4)
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

int melody[] = {
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, 0,
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, 0,
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4
};

//(4 = pătrime, 8 = optime etc.)
int noteDurations[] = {
  8, 8, 8, 4, 8, 8, 4, 8,
  8, 8, 8, 4, 8, 8, 4, 8,
  8, 8, 8, 4, 8, 8, 2
};

void setup() {
  pinMode(p_tip, OUTPUT);
  pinMode(p_r1, OUTPUT);
  pinMode(p_r2, OUTPUT);
  pinMode(p_sleeve, OUTPUT);

  
}

void loop() {
  for (int thisNote = 0; thisNote < 23; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    int note = melody[thisNote];

    if (note > 0) {
      tone(p_tip, note, noteDuration);    
      digitalWrite(p_r1, HIGH);           
      digitalWrite(p_r2, HIGH);
    }

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    noTone(p_tip);
    digitalWrite(p_r1, LOW);
    digitalWrite(p_r2, LOW);
  }

  
  delay(1000);
}
