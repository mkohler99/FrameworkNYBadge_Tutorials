//TUTORIAL 02 Simple Scrolling Text
//Uses the FastLED_NeoMatrix Library to handle drawing and scrolling text to a grid of WS2812 LEDs.

#include <Arduino.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

//Your Text Here
String storedName = "Hello, my name is...";


//Pin Definitions
#define PIXEL_PIN 4

//Screen Definitions
#define WIDTH 15
#define HEIGHT 7
#define NUM_LEDS (WIDTH * HEIGHT)
#define COLOR_ORDER GRB
#define CHIPSET WS2812
uint8_t dispBrightness = 15;
int scrollX = WIDTH;   // start text just off the right edge

//Define Text Color
CRGB textColor = CRGB(255, 0, 128);  // default solid Yellow

//Fastled/Neomatrix config
CRGB leds[NUM_LEDS];
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(
  leds, WIDTH, HEIGHT, 
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE
);




// Handles displaying text in a variety of fun ways
void scrollName() {
  matrix->fillScreen(0);
  int xPos = scrollX;
  matrix->setTextColor(matrix->Color(textColor.r, textColor.g, textColor.b));
  matrix->setCursor(xPos, 0);
  matrix->print(storedName);
  FastLED.show();
  //Handle Scrolling
  scrollX--;
  int totalWidth = storedName.length() * 6; //I Think changing this to +2 added the padd
  if (scrollX < -totalWidth) {
    scrollX = WIDTH;
  }
}




// Initializes Hardware (This only runs at startup once.)
void setup() {
  // ---- LED MATRIX ---- This sets up the display to work with the badge's unique hardware layout
  FastLED.addLeds<CHIPSET, PIXEL_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000); // FastLED attempts to manage power draw dyanically for us. We ABSOLUTELY do not want the LED's to draw too much power
  FastLED.setBrightness(dispBrightness);
  FastLED.clear(true);
  FastLED.show();
  delay(10);
  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setTextSize(1);
  matrix->setRotation(0);
}




// Main Program Loop (This runs every frame.)
void loop() {
  scrollName();
  delay(100);
}