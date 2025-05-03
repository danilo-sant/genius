#include <Vector.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

struct keys {
  int toneFreq;
  String color;
  int ledPin;
  int keyPin;
};

keys key[4] = {
  {261, "yellow", 12, 13},
  {440, "blue", 4, 15},
  {329, "red", 16, 19},
  {392, "green", 14, 27}
};

const int MAX_STEPS = 20;
int stepArray[MAX_STEPS];
typedef Vector<int> stepVector;
stepVector stepSequence;

const int startButton = 23;
const int buzzerPin = 25;
const int powerLed = 26;

const float BASE_FREQ = 1.2;      // Hz
const float FREQ_INCREMENT = 0.2; // Hz
const int FREQ_UPDATE_EVERY = 5;  // A cada 5 acertos
const int MAX_LEVEL = 20;

enum GameState { AGUARDANDO_START, JOGANDO, GAME_OVER };
GameState gameState = AGUARDANDO_START;

int level = 1;

void showMessage(String line1, String line2 = "", int delayTime = 500) {
  display.clearDisplay();
  display.setTextSize(1); // Texto uniforme
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println(line1);
  if (line2 != "") {
    display.setCursor(0, 30);
    display.println(line2);
  }
  display.display();
  delay(delayTime);
}

void playStepDynamic(int k, int tempo_ms) {
  digitalWrite(key[k].ledPin, HIGH);
  tone(buzzerPin, key[k].toneFreq);
  delay(tempo_ms);
  digitalWrite(key[k].ledPin, LOW);
  noTone(buzzerPin);
  delay(tempo_ms / 2);
}

void storeSequence(int k) {
  stepSequence.push_back(k);
  Serial.print("blinks: ");
  Serial.println(stepSequence.size());
  level = stepSequence.size(); // Atualiza o nível atual
}

int generateRandomStep() {
  return random(0, 4);
}

float calculateFrequency() {
  int progress = (level - 1) / FREQ_UPDATE_EVERY;
  float newFreq = BASE_FREQ + FREQ_INCREMENT * progress;
  if (level > MAX_LEVEL) newFreq = BASE_FREQ + FREQ_INCREMENT * ((MAX_LEVEL - 1) / FREQ_UPDATE_EVERY);
  return newFreq;
}

void computerPlay() {
  float freq = calculateFrequency();
  int tempo_ms = (int)(1000.0 / freq);

  showMessage("Jogada da CPU", "Nivel: " + String(level), 400);

  for (int i : stepSequence) {
    playStepDynamic(i, tempo_ms);
  }
}

bool compareSequence() {
  showMessage("Sua vez!", "", 300);
  for (int i = 0; i < stepSequence.size(); i++) {
    unsigned long startTime = millis();
    while (millis() - startTime < 5000) {
      for (int j = 0; j < 4; j++) {
        if (!digitalRead(key[j].keyPin)) {
          delay(80);
          if (!digitalRead(key[j].keyPin)) {
            playStepDynamic(j, 150);
            if (j != stepSequence[i]) {
              return false;
            }
            goto NEXT_STEP;
          }
        }
      }
    }
    showMessage("Tempo Esgotado!", "", 1000);
    return false;

    NEXT_STEP:
    continue;
  }
  return true;
}

void notifyGameOver() {
  showMessage("FIM DE JOGO!", "Nivel: " + String(level), 1000);
  Serial.println("END GAME...RESTARTING");

  for (int i = 0; i < 4; i++) {
    digitalWrite(key[i].ledPin, HIGH);
    delay(100);
    digitalWrite(key[i].ledPin, LOW);
    delay(100);
  }
  tone(buzzerPin, 100, 200);
  delay(500);
}

void showStartupAnimation() {
  for (int i = 3; i >= 1; i--) {
    showMessage("Iniciando em", String(i), 300);
  }
  showMessage("Vamos jogar!", "", 600);
}

void setup() {
  stepSequence.setStorage(stepArray);
  Serial.begin(115200);
  Wire.begin(21, 22);
  Wire.setClock(400000); // I2C rápido

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.display();

  pinMode(startButton, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(powerLed, OUTPUT);
  digitalWrite(powerLed, HIGH);

  for (int i = 0; i < 4; i++) {
    pinMode(key[i].keyPin, INPUT_PULLUP);
    pinMode(key[i].ledPin, OUTPUT);
  }

  randomSeed(analogRead(34));

  showMessage("Genius Game", "", 800);
  showStartupAnimation();
}

void loop() {
  switch (gameState) {
    case AGUARDANDO_START: {
      showMessage("Pressione", "START", 400);

      while (digitalRead(startButton)) {
        delay(50);
      }
      delay(200); // debounce

      stepSequence.clear();
      level = 1;
      storeSequence(generateRandomStep());
      gameState = JOGANDO;
      break;
    }

    case JOGANDO: {
      computerPlay();
      if (!compareSequence()) {
        gameState = GAME_OVER;
      } else {
        storeSequence(generateRandomStep());
      }
      break;
    }

    case GAME_OVER: {
      notifyGameOver();
      gameState = AGUARDANDO_START;
      break;
    }
  }
}
