#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// =====================================================
// ESP32 Color Guessing Game
// Hardware: ESP32 + ST7789 TFT Display + Rotary Encoder
// Author: Alp Çağatay Turda
// =====================================================

// --- Pin Definitions ---
#define TFT_CS     5
#define TFT_DC     2
#define TFT_RST    4

#define ENC_A     32
#define ENC_B     33
#define ENC_PUSH  25
#define KEY_BACK  26

// --- Game Settings ---
#define MAX_ATTEMPTS 8
#define CODE_LENGTH  4
#define NUM_COLORS   8

// --- Color Palette ---
uint16_t palette[NUM_COLORS] = {
  ST77XX_RED,
  ST77XX_GREEN,
  ST77XX_BLUE,
  ST77XX_YELLOW,
  ST77XX_MAGENTA,
  ST77XX_CYAN,
  ST77XX_ORANGE,
  0x780F
};

// --- Game State Machine ---
enum GameState {
  INIT,
  DRAW_STATIC_UI,
  INPUT_MOVE,
  CHECK_GUESS,
  UPDATE_UI,
  END_WIN,
  END_LOSE
};

GameState currentState = INIT;

// --- TFT Object ---
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// --- Game Data ---
int secretCode[CODE_LENGTH];
int currentGuess[CODE_LENGTH];
int guessHistory[MAX_ATTEMPTS][CODE_LENGTH];

// Feedback values:
// -1: Empty
//  0: No match / White
//  1: Correct color, wrong position / Yellow
//  2: Correct color, correct position / Green
int feedbackHistory[MAX_ATTEMPTS][CODE_LENGTH];

int currentAttempt = 0;
int currentSlot = 0;
int selectedColorIdx = 0;

// --- Encoder Variables ---
int lastEncA;

// --- Animation Variables ---
unsigned long lastAnimTime = 0;
bool animState = false;

// --- Display Layout Settings ---
const int boxSize = 28;
const int horizontalGap = 14;
const int verticalStep = 36;
const int startY = 10;

// --- Feedback Indicator Size ---
const int feedbackW = 20;
const int feedbackH = 5;

void setup() {
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(ENC_PUSH, INPUT_PULLUP);
  pinMode(KEY_BACK, INPUT_PULLUP);

  tft.init(240, 320);
  tft.setRotation(2);
  tft.fillScreen(ST77XX_WHITE);

  lastEncA = digitalRead(ENC_A);

  // Creates a random seed for the secret code
  randomSeed(analogRead(0));
}

void loop() {
  switch (currentState) {
    case INIT:
      generateSecretCode();

      for (int i = 0; i < MAX_ATTEMPTS; i++) {
        for (int j = 0; j < CODE_LENGTH; j++) {
          feedbackHistory[i][j] = -1;
        }
      }

      currentAttempt = 0;
      currentSlot = 0;
      currentState = DRAW_STATIC_UI;
      break;

    case DRAW_STATIC_UI:
      drawBoard();
      drawPalette();
      currentState = INPUT_MOVE;
      break;

    case INPUT_MOVE:
      handleEncoder();
      handleButtons();
      drawActiveSlotAnim();
      break;

    case CHECK_GUESS:
      processGuess();
      break;

    case UPDATE_UI:
      drawBoard();
      drawPalette();

      if (currentAttempt >= MAX_ATTEMPTS) {
        currentState = END_LOSE;
      } else {
        currentState = INPUT_MOVE;
      }
      break;

    case END_WIN:
      showEndGame("YOU WIN!");
      break;

    case END_LOSE:
      showEndGame("GAME OVER!");
      break;
  }
}

// Generates a random secret color code
void generateSecretCode() {
  for (int i = 0; i < CODE_LENGTH; i++) {
    secretCode[i] = random(NUM_COLORS);
  }
}

// Draws the main game board
void drawBoard() {
  tft.fillScreen(ST77XX_WHITE);

  int startX = (240 - (CODE_LENGTH * boxSize + (CODE_LENGTH - 1) * horizontalGap)) / 2;

  for (int i = 0; i < MAX_ATTEMPTS; i++) {
    for (int j = 0; j < CODE_LENGTH; j++) {
      int px = startX + j * (boxSize + horizontalGap);
      int py = startY + i * verticalStep;

      // Draw empty box
      tft.drawRect(px, py, boxSize, boxSize, ST77XX_BLACK);

      if (i < currentAttempt) {
        // Draw previous guess color
        tft.fillRect(px + 2, py + 2, boxSize - 4, boxSize - 4, palette[guessHistory[i][j]]);

        // Draw feedback indicator under each box
        uint16_t feedCol = ST77XX_WHITE;

        if (feedbackHistory[i][j] == 2) {
          feedCol = ST77XX_GREEN;
        } else if (feedbackHistory[i][j] == 1) {
          feedCol = ST77XX_YELLOW;
        }

        int feedX = px + (boxSize - feedbackW) / 2;
        int feedY = py + boxSize + 2;

        tft.fillRect(feedX, feedY, feedbackW, feedbackH, feedCol);
        tft.drawRect(feedX, feedY, feedbackW, feedbackH, feedCol);
      }

      else if (i == currentAttempt && j < currentSlot) {
        // Draw already selected colors in the current row
        tft.fillRect(px + 2, py + 2, boxSize - 4, boxSize - 4, palette[currentGuess[j]]);
      }
    }
  }
}

// Draws the color palette on both sides of the screen
void drawPalette() {
  int pSize = 18;
  int gap = 6;
  int paletteStartY = 320 - (4 * (pSize + gap)) - 10;

  for (int i = 0; i < 4; i++) {
    // Left side colors
    tft.fillRect(10, paletteStartY + i * (pSize + gap), pSize, pSize, palette[i]);
    tft.drawRect(10, paletteStartY + i * (pSize + gap), pSize, pSize, ST77XX_BLACK);

    // Right side colors
    tft.fillRect(240 - pSize - 10, paletteStartY + i * (pSize + gap), pSize, pSize, palette[i + 4]);
    tft.drawRect(240 - pSize - 10, paletteStartY + i * (pSize + gap), pSize, pSize, ST77XX_BLACK);
  }
}

// Reads rotary encoder movement and changes selected color
void handleEncoder() {
  int valA = digitalRead(ENC_A);

  if (valA != lastEncA && valA == LOW) {
    if (digitalRead(ENC_B) != valA) {
      selectedColorIdx = (selectedColorIdx + 1) % NUM_COLORS;
    } else {
      selectedColorIdx = (selectedColorIdx - 1 + NUM_COLORS) % NUM_COLORS;
    }
  }

  lastEncA = valA;
}

// Handles encoder push button and back button
void handleButtons() {
  if (digitalRead(ENC_PUSH) == LOW) {
    int startX = (240 - (CODE_LENGTH * boxSize + (CODE_LENGTH - 1) * horizontalGap)) / 2;
    int px = startX + currentSlot * (boxSize + horizontalGap);
    int py = startY + currentAttempt * verticalStep;

    // Draw selected color in the active slot
    tft.fillRect(px + 2, py + 2, boxSize - 4, boxSize - 4, palette[selectedColorIdx]);

    delay(250);

    currentGuess[currentSlot] = selectedColorIdx;
    currentSlot++;

    if (currentSlot >= CODE_LENGTH) {
      currentState = CHECK_GUESS;
    }
  }

  if (digitalRead(KEY_BACK) == LOW) {
    delay(250);

    if (currentSlot > 0) {
      currentSlot--;

      int startX = (240 - (CODE_LENGTH * boxSize + (CODE_LENGTH - 1) * horizontalGap)) / 2;
      int px = startX + currentSlot * (boxSize + horizontalGap);
      int py = startY + currentAttempt * verticalStep;

      // Clear the last selected slot
      tft.fillRect(px + 2, py + 2, boxSize - 4, boxSize - 4, ST77XX_WHITE);
    }
  }
}

// Checks the current guess and calculates feedback
void processGuess() {
  bool usedSecret[CODE_LENGTH] = {false};
  bool usedGuess[CODE_LENGTH] = {false};

  int results[CODE_LENGTH] = {0};
  int correctCount = 0;

  // First pass:
  // Checks correct color in the correct position
  for (int i = 0; i < CODE_LENGTH; i++) {
    if (currentGuess[i] == secretCode[i]) {
      results[i] = 2;
      usedSecret[i] = true;
      usedGuess[i] = true;
      correctCount++;
    }
  }

  // Second pass:
  // Checks correct color in the wrong position
  for (int i = 0; i < CODE_LENGTH; i++) {
    if (usedGuess[i]) {
      continue;
    }

    for (int j = 0; j < CODE_LENGTH; j++) {
      if (!usedSecret[j] && currentGuess[i] == secretCode[j]) {
        results[i] = 1;
        usedSecret[j] = true;
        break;
      }
    }
  }

  // Saves current guess and feedback to history
  for (int i = 0; i < CODE_LENGTH; i++) {
    guessHistory[currentAttempt][i] = currentGuess[i];
    feedbackHistory[currentAttempt][i] = results[i];
  }

  currentAttempt++;
  currentSlot = 0;

  if (correctCount == CODE_LENGTH) {
    currentState = END_WIN;
  } else {
    currentState = UPDATE_UI;
  }

  delay(500);
}

// Blinks the active slot with the currently selected color
void drawActiveSlotAnim() {
  if (millis() - lastAnimTime > 300) {
    lastAnimTime = millis();
    animState = !animState;

    int startX = (240 - (CODE_LENGTH * boxSize + (CODE_LENGTH - 1) * horizontalGap)) / 2;
    int px = startX + currentSlot * (boxSize + horizontalGap);
    int py = startY + currentAttempt * verticalStep;

    tft.fillRect(
      px + 2,
      py + 2,
      boxSize - 4,
      boxSize - 4,
      animState ? palette[selectedColorIdx] : ST77XX_WHITE
    );
  }
}

// Shows win or lose screen and restarts the game
void showEndGame(String msg) {
  tft.fillRect(30, 100, 180, 110, ST77XX_WHITE);
  tft.drawRect(30, 100, 180, 110, ST77XX_BLACK);

  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(65, 120);
  tft.print(msg);

  // Shows the secret code at the end of the game
  for (int i = 0; i < CODE_LENGTH; i++) {
    tft.fillRect(60 + i * 35, 165, 25, 25, palette[secretCode[i]]);
  }

  delay(6000);

  currentState = INIT;
}
