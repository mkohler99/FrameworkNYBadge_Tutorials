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








// Initializes Hardware (This only runs at startup once.)
void setup() {


  // ---- LED MATRIX ---- This sets up the display to work with the badge's unique hardware layout
  FastLED.addLeds<CHIPSET, PIXEL_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000); // FastLED attempts to manage power draw dyanically for us. We ABSOLUTELY do not want the LED's to draw too much power
  FastLED.setBrightness(dispBrightness);
  FastLED.clear(true);
  FastLED.show();
  
}





void colorWipe(CRGB color, int wait) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
    FastLED.show();
    delay(wait);
  }
  delay(500); // pause at the end before switching color
}

// Main Program Loop (This runs every frame.)
void loop() {
  colorWipe(CRGB::Red,   50);  // fill red
  FastLED.clear();
  FastLED.show();

  colorWipe(CRGB::Green, 50);  // fill green
  FastLED.clear();
  FastLED.show();

  colorWipe(CRGB::Blue,  50);  // fill blue
  FastLED.clear();
  FastLED.show();
}