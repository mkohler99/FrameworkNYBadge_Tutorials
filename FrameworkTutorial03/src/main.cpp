//TUTORIAL 03 Fancy Scrolling Text with FX 
//Displays text with a variety of fun animations (like on a NYC hot dog cart!) Middle button changes FX 

#include <Arduino.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

//Your Text Here
String storedName = "BOTTOM TEXT";


//Pin Definitions
#define PIXEL_PIN 4
#define BTN2 6


//Screen Definitions
#define WIDTH 15
#define HEIGHT 7
#define NUM_LEDS (WIDTH * HEIGHT)
#define COLOR_ORDER GRB
#define CHIPSET WS2812
uint8_t dispBrightness = 15;


//Button Setup
bool lastBtn2State = HIGH;


//display
int scrollX = WIDTH;   // start text just off the right edge


//FX List
enum TextEffect {
  EFFECT_SOLID,
  EFFECT_RAINBOW,
  EFFECT_WAVE,
  EFFECT_FIRE,
  EFFECT_GLITCH,
  EFFECT_COUNT  // Because arduoino has no way to return the length of an enum
};


//Effect Setup Vars
TextEffect currentTextEffect = EFFECT_SOLID;
CRGB textColor = CRGB(255, 255, 0);  // default solid Yellow
uint8_t rainbowHue = 0;            // for rainbow chase animation


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

  // Apply effect-specific rendering
  switch (currentTextEffect) {

      case EFFECT_SOLID:
        matrix->setTextColor(matrix->Color(textColor.r, textColor.g, textColor.b));
        matrix->setCursor(xPos, 0);
        matrix->print(storedName);
        break;

      case EFFECT_RAINBOW:
        for (uint8_t i = 0; i < storedName.length(); i++) {
          char c = storedName.charAt(i);
          uint8_t hue = (i * 30) + rainbowHue;
          CRGB color = CHSV(hue, 255, 255);
          matrix->setTextColor(matrix->Color(color.r, color.g, color.b));
          matrix->setCursor(xPos, 0);
          matrix->print(c);
          xPos += 6;
        }
        rainbowHue += 3;
        break;

      case EFFECT_FIRE:
        for (int i = 0; i < NUM_LEDS; i++) {
          uint8_t flicker = random8(120);
          leds[i] = CRGB(127, (random8(100, 180) - flicker/2)/2 , 0); 
        }
        matrix->setTextColor(matrix->Color(255, 255, 255));
        matrix->setCursor(xPos, 0);
        matrix->print(storedName);
        break;

      case EFFECT_GLITCH: {
          bool burst = (random8() < 10);  
          int xPosGlitch = scrollX;
          int screenJitterX = burst ? random8(-1, 2) : 0;
          int screenJitterY = burst ? random8(-1, 2) : 0;

          for (uint8_t i = 0; i < storedName.length(); i++) {
              char c = storedName.charAt(i);

              int xJitter = (random8() < 50) ? random8(-1, 2) : 0;
              int yJitter = (random8() < 50) ? random8(-1, 2) : 0;

              CRGB color;
              if (burst) {
                  color = CRGB(random8(), random8(), random8());
              } else if (random8() < 40) {
                  color = CRGB(255, random8(80,255), 255);
              } else {
                  color = CRGB(180, 200, 255);
              }

              matrix->setTextColor(matrix->Color(color.r, color.g, color.b));
              matrix->setCursor(xPosGlitch + xJitter + screenJitterX, yJitter + screenJitterY);
              matrix->print(c);

              if (random8() < 40) {
                  matrix->setCursor(xPosGlitch + xJitter + random8(-1, 2),
                                    yJitter + random8(-1, 2));
                  matrix->print(c);
              }

              xPosGlitch += 6;
          }
      }
      break;

      case EFFECT_WAVE:
        for (uint8_t i = 0; i < storedName.length(); i++) {
          char c = storedName.charAt(i);
          int yOffset = (int)(sin8((scrollX + i * 10) * 4) / 42) - 3;
          matrix->setTextColor(matrix->Color(textColor.r, textColor.g, textColor.b));
          matrix->setCursor(xPos, yOffset);
          matrix->print(c);
          xPos += 6;
        }
        break;
  }

  FastLED.show();
  scrollX--;

  int totalWidth = storedName.length() * 6; //I Think changing this to +2 added the padd
  if (scrollX < -totalWidth) {
    scrollX = WIDTH;
  }
}




// Uses button 2 to loop through text display methods 
void handleButtons() {
  // --- Button 2: Text effect toggle ---
  // --- Button 2: cycle through effects ---
  bool btn2State = digitalRead(BTN2);
  if (btn2State == LOW && lastBtn2State == HIGH) {
    currentTextEffect = (TextEffect)((currentTextEffect + 1) % EFFECT_COUNT);

    switch (currentTextEffect) {
      case EFFECT_SOLID:   Serial.println("Text effect: SOLID"); break;
      case EFFECT_RAINBOW: Serial.println("Text effect: RAINBOW"); break;
      case EFFECT_FIRE:    Serial.println("Text effect: FIRE"); break;
      case EFFECT_GLITCH:  Serial.println("Text effect: GLITCH"); break;
      case EFFECT_WAVE:    Serial.println("Text effect: WAVE"); break;
    }
  }
}




// Initializes Hardware (This only runs at startup once.)
void setup() {
  Serial.begin(115200);
  
  //GPIO Config
  pinMode(BTN2, INPUT_PULLUP);

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
  handleButtons();
  scrollName();
      delay(100);
}