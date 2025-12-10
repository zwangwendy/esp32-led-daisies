#include <FastLED.h>

#define LED_PIN     18
#define NUM_LEDS    64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define BRIGHTNESS  200
#define MATRIX_WIDTH  8
#define MATRIX_HEIGHT 8

// ---- SONG SETTINGS (DAISIES) ----
#define BPM 126
#define BEAT_INTERVAL (60000 / BPM)

unsigned long lastBeat = 0;
unsigned long startTime = 0;
uint16_t beatCount = 0;

// ---- XY MAP ----
uint16_t XY(uint8_t x, uint8_t y) {
  if (y % 2 == 0) return y * MATRIX_WIDTH + x;
  else return y * MATRIX_WIDTH + (MATRIX_WIDTH - 1 - x);
}

// ---- EFFECTS ----

void softBackground() {
  for (int y = 0; y < MATRIX_HEIGHT; y++) {
    for (int x = 0; x < MATRIX_WIDTH; x++) {
      uint8_t hue =
        sin8(x * 25 + millis() / 8) +
        cos8(y * 25 + millis() / 10);
      leds[XY(x, y)] = CHSV(hue, 180, 180);
    }
  }
  FastLED.show();
}

void beatKick() {
  fill_solid(leds, NUM_LEDS, CHSV(200 + random8(40), 255, 255));
  FastLED.show();
}

void snareFlash() {
  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB::White;
  FastLED.show();
}

void dropWave() {
  for (int r = 0; r < 4; r++) {
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
      for (int x = 0; x < MATRIX_WIDTH; x++) {
        leds[XY(x, y)] = CHSV(0 + r * 40, 255, 255);
      }
    }
    FastLED.show();
    delay(10);
  }
}

// ---- BEAT ENGINE ----
void onBeat() {
  beatCount++;

  beatKick();                // Every beat

  if (beatCount % 4 == 0) {  // Snare every 4 beats
    snareFlash();
  }

  if (beatCount % 16 == 0) { // Big drop every 16 beats
    dropWave();
  }
}

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  startTime = millis();
}

void loop() {
  unsigned long now = millis();

  // Continuous ambient motion
  softBackground();

  // Beat timing
  if (now - lastBeat >= BEAT_INTERVAL) {
    lastBeat = now;
    onBeat();
  }

  delay(10);
}

