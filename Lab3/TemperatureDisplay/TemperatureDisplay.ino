
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <stdint.h> 
#include "TouchScreen.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

// Touchscreen definitions
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

#define TS_MAXX 942
#define TS_MAXY 890 
#define TS_MINX 122 
#define TS_MINY 111

#define LINECOLOR1 0xEBD5  


bool buttonPress();

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Use hardware SPI (on Uno, #13, #12, #11) and the above  CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

volatile int x1 = 0;
volatile int x2 = x1 + 96;
volatile int y1 = 0;
volatile int y2 = y1 + 96;

void setup() {
  Serial.begin(9600);
  tft.begin(); 
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRect(x1, y1, x2, y2, LINECOLOR1);
}


void loop(void){
  TSPoint p = ts.getPoint();
  if (p.z > ts.pressureThreshhold) {
     p.x = map(p.x, TS_MAXX, TS_MINX, 320, 0);
     p.y = map(p.y, TS_MAXY, TS_MINY, 480, 0);

     Serial.print("X = "); Serial.print(p.x);
     Serial.print("\tY = "); Serial.print(p.y);
     Serial.print("\tPressure = "); Serial.println(p.z);
  }
  if(buttonPress(p)) {
    tft.fillScreen(ILI9341_RED);
  }
  delay(500);
}

bool buttonPress(TSPoint p) {
  if((p.x < x2) and (p.x > x1)){
    if((p.y < y2) and (p.y > y1)) {
      return true;
    }
  }
  return false;
}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  tft.fillScreen(ILI9341_RED);
  yield();
  tft.fillScreen(ILI9341_GREEN);
  yield();
  tft.fillScreen(ILI9341_BLUE);
  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  return micros() - start;
}
