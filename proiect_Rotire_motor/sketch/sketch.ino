#include <Arduino.h>
#include <IRremote.h>
#include <AccelStepper.h>

/*
 Autor: Popescu Andrei

 FORMAT COMANDA:
 DirectieYUnghiYRepetariYVitezaX

 Y- butonul play de pe telecomanda
 X- butonul power de pe telecomanda
 
*/

// ================= PINI =================
#define IR_PIN     15
#define STEP_PIN   14
#define DIR_PIN    12
#define ENABLE_PIN 13
#define LED_PIN    2


#define BTN_POWER 162   // X
#define BTN_PLAY  168   // Y

#define BTN_0 104
#define BTN_1 48
#define BTN_2 24
#define BTN_3 122
#define BTN_4 16
#define BTN_5 56
#define BTN_6 90
#define BTN_7 66
#define BTN_8 74
#define BTN_9 82

// ============ STEPPER ===================
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// ============ VARIABILE =================
bool inConfigMode = false;
unsigned long powerPressStart = 0;
bool powerPressed = false;

String commandString = "";

// parametri
int p_dir, p_angle, p_reps, p_speed;

// =======================================
char getChar(uint16_t code) {
  if (code == BTN_0) return '0';
  if (code == BTN_1) return '1';
  if (code == BTN_2) return '2';
  if (code == BTN_3) return '3';
  if (code == BTN_4) return '4';
  if (code == BTN_5) return '5';
  if (code == BTN_6) return '6';
  if (code == BTN_7) return '7';
  if (code == BTN_8) return '8';
  if (code == BTN_9) return '9';
  if (code == BTN_PLAY) return 'Y';
  return '?';
}

// =======================================
void executeMotor() {
  Serial.println("\n=== EXECUTIE ===");
  Serial.println("Comanda: " + commandString);

  int y1 = commandString.indexOf('Y');
  int y2 = commandString.indexOf('Y', y1 + 1);
  int y3 = commandString.lastIndexOf('Y');

  if (y1 < 0 || y2 < 0 || y3 < 0) {
    Serial.println("EROARE FORMAT");
    return;
  }

  p_dir   = commandString.substring(0, y1).toInt();
  p_angle = commandString.substring(y1 + 1, y2).toInt();
  p_reps  = commandString.substring(y2 + 1, y3).toInt();
  p_speed = commandString.substring(y3 + 1).toInt();

  if (p_dir < 0 || p_dir > 1 || p_angle <= 0 || p_reps <= 0 || p_speed < 1 || p_speed > 9) {
    Serial.println("EROARE PARAMETRI");
    return;
  }

  // ===== SETARI MOTOR =====
  long stepsPerRev = 200; 
  long steps = (p_angle * stepsPerRev) / 360;
  long target = (p_dir == 0) ? steps : -steps;

  float speed = map(p_speed, 1, 9, 2000, 300);
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(speed);

  digitalWrite(ENABLE_PIN, LOW);

  // ===== EXECUTIE =====
  for (int i = 0; i < p_reps; i++) {
    Serial.printf("Repetare %d/%d\n", i + 1, p_reps);

    // RESET POZITIE
    stepper.setCurrentPosition(0);

    // DUS
    stepper.moveTo(target);
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }

    delay(200);

    // INTOARCERE
    stepper.moveTo(0);
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }

    delay(300);
  }

  digitalWrite(ENABLE_PIN, HIGH);
  Serial.println("=== FINAL ===\n");
}

// =======================================
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, HIGH);

  IrReceiver.begin(IR_PIN);

  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(1000);

  Serial.println("Sistem gata. Tine X pentru configurare.");
}

// =======================================
void loop() {

  if (IrReceiver.decode()) {
    uint16_t cmd = IrReceiver.decodedIRData.command;
    bool repeat = IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT;

    if (cmd == BTN_POWER && !repeat) {
      if (!inConfigMode) {
        inConfigMode = true;
        commandString = "";
        digitalWrite(LED_PIN, HIGH);
        Serial.print("\nCONFIG: ");
      } else {
        inConfigMode = false;
        digitalWrite(LED_PIN, LOW);
        executeMotor();
      }
    }

    if (inConfigMode && !repeat && cmd != BTN_POWER) {
      char c = getChar(cmd);
      if (c != '?') {
        commandString += c;
        Serial.print(c);
      }
    }

    IrReceiver.resume();
  }
}
