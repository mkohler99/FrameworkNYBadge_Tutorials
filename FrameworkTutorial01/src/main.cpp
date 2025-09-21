//Tutorial 01
//NTSC Color Bars Scrolling on

#include <Arduino.h>
#include <FastLED.h>


//Pin Definitions
#define PIXEL_PIN 4


//Screen Definitions
#define WIDTH 15
#define HEIGHT 7
#define NUM_LEDS (WIDTH * HEIGHT)
#define COLOR_ORDER GRB
#define CHIPSET WS2812
uint8_t dispBrightness = 15;


//Fastled/Neomatrix config
CRGB leds[NUM_LEDS];


// Speeds
#define PER_PIXEL_DELAY_MS   30   // time between pixels within a column
#define PER_COLUMN_PAUSE_MS  60   // brief pause after each column finishes
#define HOLD_COMPLETE_MS     600  // how long to hold full bars before clearing
#define LOOP_CLEAR_PAUSE_MS  300  // pause after clearing


// NTSC/SMPTE top bars left→right: White, Yellow, Cyan, Green, Magenta, Red, Blue
const CRGB BARS[7] = {
  CRGB(255,255,255), // White
  CRGB(255,255,0),   // Yellow
  CRGB(0,255,255),   // Cyan
  CRGB(0,255,0),     // Green
  CRGB(255,0,255),   // Magenta
  CRGB(255,0,0),     // Red
  CRGB(0,0,255)      // Blue
};


// Column widths: 2,2,2,2,2,2,3 (15 total)
const uint8_t BAR_WIDTHS[7] = {2,2,2,2,2,2,3};

// Linear index for non-serpentine columns (top→bottom in every column)
uint16_t xy(uint8_t x, uint8_t y) {
  return x * HEIGHT + y;
}

// Which bar color does this column belong to?
uint8_t barIndexForColumn(uint8_t col) {
  uint8_t acc = 0;
  for (uint8_t i = 0; i < 7; i++) {
    acc += BAR_WIDTHS[i];
    if (col < acc) return i;
  }
  return 6; // fallback (blue)
}

inline CRGB colorForColumn(uint8_t col) {
  return BARS[barIndexForColumn(col)];
}

// Wipe a single column on vertically (top→bottom) in its bar color
void wipeColumnVertical(uint8_t col, uint16_t perPixelDelayMs) {
  CRGB c = colorForColumn(col);
  for (uint8_t row = 0; row < HEIGHT; row++) {
    leds[xy(col, row)] = c;
    FastLED.show();
    delay(perPixelDelayMs);
  }
}

void scrollOnVertical() {
  FastLED.clear(true);

  for (uint8_t col = 0; col < WIDTH; col++) {
    wipeColumnVertical(col, PER_PIXEL_DELAY_MS);
    delay(PER_COLUMN_PAUSE_MS);
  }

  // Hold full bars, then clear before repeating
  delay(HOLD_COMPLETE_MS);
  FastLED.clear(true);
  delay(LOOP_CLEAR_PAUSE_MS);
}

void setup() {
  FastLED.addLeds<CHIPSET, PIXEL_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(dispBrightness);
  FastLED.clear(true);
}

void loop() {
  scrollOnVertical();
}
