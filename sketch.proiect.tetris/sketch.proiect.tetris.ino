#include <LedControl.h>

// Pini pentru matricea LED
#define DIN 11
#define CS 10
#define CLK 13

#define BTN_UP 41    // W - rotire
#define BTN_DOWN 47  // S - coborare rapida
#define BTN_LEFT 43  // A - stanga
#define BTN_RIGHT 45 // D - dreapta

#define BUZZER 49

#define MATRIX_WIDTH 16
#define MATRIX_HEIGHT 32
#define NUM_MATRICES 8

LedControl lc1 = LedControl(DIN, CLK, CS, 4);  


bool grid[MATRIX_HEIGHT][MATRIX_WIDTH];

byte displayBuffer[4][8];

// Definirea pieselor Tetris (format: 4x4)
const byte TETROMINOS[7][4][4] PROGMEM = {
  // I
  {{0,0,0,0},
   {1,1,1,1},
   {0,0,0,0},
   {0,0,0,0}},
  // O
  {{0,0,0,0},
   {0,1,1,0},
   {0,1,1,0},
   {0,0,0,0}},
  // T
  {{0,0,0,0},
   {0,1,0,0},
   {1,1,1,0},
   {0,0,0,0}},
  // S
  {{0,0,0,0},
   {0,1,1,0},
   {1,1,0,0},
   {0,0,0,0}},
  // Z
  {{0,0,0,0},
   {1,1,0,0},
   {0,1,1,0},
   {0,0,0,0}},
  // J
  {{0,0,0,0},
   {1,0,0,0},
   {1,1,1,0},
   {0,0,0,0}},
  // L
  {{0,0,0,0},
   {0,0,1,0},
   {1,1,1,0},
   {0,0,0,0}}
};

const int gameOverMelody[] PROGMEM = {262, 196, 196, 220, 196, 0, 247, 262};
const int gameOverDurations[] PROGMEM = {4, 8, 8, 4, 4, 4, 4, 4};

int currentPiece = 0;
int currentRotation = 0;
int currentX = 2;
int currentY = 0;
byte currentShape[4][4];


unsigned long lastFall = 0;
unsigned long fallDelay = 500;
bool gameOver = false;
bool gameOverShown = false; 
int score = 0;
bool fastFall = false;


unsigned long lastButtonPress = 0;
const int debounceDelay = 150;

void setup() {
  Serial.begin(9600);
  
  // Initializare butoane
  pinMode(BTN_UP, INPUT);
  pinMode(BTN_DOWN, INPUT);
  pinMode(BTN_LEFT, INPUT);
  pinMode(BTN_RIGHT, INPUT);
  pinMode(BUZZER, OUTPUT);
  
  // Initializare matrice LED
  for(int i = 0; i < 4; i++) {
    lc1.shutdown(i, false);
    lc1.setIntensity(i, 2);
    lc1.clearDisplay(i);
  }
  
  // Initializare buffer display
  for(int matrix = 0; matrix < 4; matrix++) {
    for(int row = 0; row < 8; row++) {
      displayBuffer[matrix][row] = 0;
    }
  }
  
  clearGrid();
  
  spawnNewPiece();
  
  Serial.println("Tetris Start!");
  playStartSound();
}

void loop() {
  
  if(gameOver) {
    if(!gameOverShown) {
      displayGameOver();
      gameOverShown = true; 
    }
    return; 
  }
  
  handleInput();
  
  unsigned long currentTime = millis();
  int currentDelay = fastFall ? 50 : fallDelay;
  
  if(currentTime - lastFall > currentDelay) {
    lastFall = currentTime;
    
    if(!movePiece(0, 1)) {
      
      lockPiece();
      clearLines();
      spawnNewPiece();
      
      
      if(checkCollision(currentX, currentY, currentRotation)) {
        gameOver = true;
        playGameOverSound();
      }
    }
  }
  
 
  updateDisplay();
  delay(10);
}

void handleInput() {
  unsigned long currentTime = millis();
  
  if(currentTime - lastButtonPress < debounceDelay) {
    return;
  }
  
  // Stanga
  if(digitalRead(BTN_LEFT) == HIGH) {
    if(movePiece(-1, 0)) {
      Serial.println("Buton: STANGA"); // MODIFICARE: Afisare Serial
      playMoveSound();
      lastButtonPress = currentTime;
    }
  }
  
  // Dreapta
  if(digitalRead(BTN_RIGHT) == HIGH) {
    if(movePiece(1, 0)) {
      Serial.println("Buton: DREAPTA"); // MODIFICARE: Afisare Serial
      playMoveSound();
      lastButtonPress = currentTime;
    }
  }
  
  // Rotire
  if(digitalRead(BTN_UP) == HIGH) {
    if(rotatePiece()) {
      Serial.println("Buton: ROTIRE"); // MODIFICARE: Afisare Serial
      playMoveSound();
      lastButtonPress = currentTime;
    }
  }
  
  // Coborare rapida
  if(digitalRead(BTN_DOWN) == HIGH) {
    if(!fastFall) { // Afisam doar cand se activeaza
       Serial.println("Buton: COBORARE RAPIDA"); // MODIFICARE: Afisare Serial
    }
    fastFall = true;
  } else {
    fastFall = false;
  }
}
//ai{
bool movePiece(int dx, int dy) {
  int newX = currentX + dx;
  int newY = currentY + dy;
  
  if(checkCollision(newX, newY, currentRotation)) {
    return false;
  }
  
  currentX = newX;
  currentY = newY;
  return true;
}

bool rotatePiece() {
  int newRotation = (currentRotation + 1) % 4;
  
  if(checkCollision(currentX, currentY, newRotation)) {
    return false;
  }
  
  currentRotation = newRotation;
  loadShape();
  return true;
}

bool checkCollision(int x, int y, int rotation) {
  for(int row = 0; row < 4; row++) {
    for(int col = 0; col < 4; col++) {
      byte block = pgm_read_byte(&TETROMINOS[currentPiece][row][col]);
      
      // Aplicare rotatie
      int px = col;
      int py = row;
      for(int r = 0; r < rotation; r++) {
        int temp = px;
        px = 2 - py;
        py = temp;
      }
      
      if(block) {
        int gridX = x + px;
        int gridY = y + py;
        
        // Verificare margini
        if(gridX < 0 || gridX >= MATRIX_WIDTH || gridY >= MATRIX_HEIGHT) {
          return true;
        }
        
        // Verificare coliziune cu piese existente
        if(gridY >= 0 && grid[gridY][gridX]) {
          return true;
        }
      }
    }
  }
  return false;
}

void lockPiece() {
  for(int row = 0; row < 4; row++) {
    for(int col = 0; col < 4; col++) {
      if(currentShape[row][col]) {
        int gridY = currentY + row;
        int gridX = currentX + col;
        
        if(gridY >= 0 && gridY < MATRIX_HEIGHT && gridX >= 0 && gridX < MATRIX_WIDTH) {
          grid[gridY][gridX] = true;
        }
      }
    }
  }
}

void clearLines() {
  int linesCleared = 0;
  
  for(int row = MATRIX_HEIGHT - 1; row >= 0; row--) {
    bool fullLine = true;
    
    for(int col = 0; col < MATRIX_WIDTH; col++) {
      if(!grid[row][col]) {
        fullLine = false;
        break;
      }
    }
    
    if(fullLine) {
      linesCleared++;
      
      // Coborare randuri superioare
      for(int r = row; r > 0; r--) {
        for(int col = 0; col < MATRIX_WIDTH; col++) {
          grid[r][col] = grid[r-1][col];
        }
      }
      
      // Golire rand de sus
      for(int col = 0; col < MATRIX_WIDTH; col++) {
        grid[0][col] = false;
      }
      
      row++; // Verificare din nou acelasi rand
    }
  }
  
  if(linesCleared > 0) {
    score += linesCleared * 100;
    playLineClearSound();
    Serial.print("Lines cleared: ");
    Serial.print(linesCleared);
    Serial.print(" | Score: ");
    Serial.println(score);
  }
}
//ai}
void spawnNewPiece() {
  currentPiece = random(7);
  currentRotation = 0;
  currentX = 2;
  currentY = 0;
  loadShape();
}

void loadShape() {
  for(int row = 0; row < 4; row++) {
    for(int col = 0; col < 4; col++) {
      byte block = pgm_read_byte(&TETROMINOS[currentPiece][row][col]);
      
      // Aplicare rotatie
      int px = col;
      int py = row;
      for(int r = 0; r < currentRotation; r++) {
        int temp = px;
        px = 2 - py;
        py = temp;
      }
      
      currentShape[py][px] = block;
    }
  }
}

void updateDisplay() {
  // Resetare buffer
  for(int matrix = 0; matrix < 4; matrix++) {
    for(int row = 0; row < 8; row++) {
      displayBuffer[matrix][row] = 0;
    }
  }
  
  // Adaugare grid in buffer
  for(int row = 0; row < MATRIX_HEIGHT; row++) {
    for(int col = 0; col < MATRIX_WIDTH; col++) {
      if(grid[row][col]) {
        int matrixNum = row / 8;
        int matrixRow = row % 8;
        if(matrixNum < 4) {
          displayBuffer[matrixNum][matrixRow] |= (1 << col);
        }
      }
    }
  }
  
  // Adaugare piesa curenta in buffer
  for(int row = 0; row < 4; row++) {
    for(int col = 0; col < 4; col++) {
      if(currentShape[row][col]) {
        int y = currentY + row;
        int x = currentX + col;
        
        if(y >= 0 && y < MATRIX_HEIGHT && x >= 0 && x < MATRIX_WIDTH) {
          int matrixNum = y / 8;
          int matrixRow = y % 8;
          if(matrixNum < 4) {
            displayBuffer[matrixNum][matrixRow] |= (1 << x);
          }
        }
      }
    }
  }
  
  // Scriere buffer pe matrice
  for(int matrix = 0; matrix < 4; matrix++) {
    for(int row = 0; row < 8; row++) {
      lc1.setRow(matrix, row, displayBuffer[matrix][row]);
    }
  }
}

void clearGrid() {
  for(int row = 0; row < MATRIX_HEIGHT; row++) {
    for(int col = 0; col < MATRIX_WIDTH; col++) {
      grid[row][col] = false;
    }
  }
}

// Sunete
void playMoveSound() {
  tone(BUZZER, 800, 50);
}

void playLineClearSound() {
  tone(BUZZER, 523, 150);
  delay(150);
  tone(BUZZER, 659, 150);
  delay(150);
  tone(BUZZER, 784, 200);
  delay(200);
}

void playStartSound() {
  tone(BUZZER, 523, 100);
  delay(120);
  tone(BUZZER, 659, 100);
  delay(120);
}

void playGameOverSound() {
  for(int i = 0; i < 8; i++) {
    int note = pgm_read_word(&gameOverMelody[i]);
    int duration = 1000 / pgm_read_word(&gameOverDurations[i]);
    
    if(note > 0) {
      tone(BUZZER, note, duration);
    }
    delay(duration * 1.3);
    noTone(BUZZER);
  }
}

void displayGameOver() {
  Serial.println("-----------------");
  Serial.println("GAME OVER!");
  Serial.print("Final Score: ");
  Serial.println(score);
  Serial.println("-----------------");
}