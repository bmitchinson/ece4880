
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
#define XP 7   // can be a digital pin

// Touch screen variables defining maximum and minum inputs to 
// convert between touch screen and display coordinates
#define TS_MAXX 942
#define TS_MAXY 890 
#define TS_MINX 122 
#define TS_MINY 111

// view min and max
#define VIEW_MAX_X 240
#define VIEW_MAX_Y 320

//
// #define LINECOLOR1 0xEBD5  

// structs
struct Rectangle {
  int minX;
  int maxX;
  int minY;
  int maxY;
  unsigned int fillColor;
};

// function prototypes
void drawRectangle(Rectangle rectangle);
bool tsPointInRectangle(TSPoint p, Rectangle rectangle);
void drawBackground();

// initialize global object for touch screen
# define UNKNOWN_TOUCHSCREEN_PARAM 300
TouchScreen ts = TouchScreen(XP, YP, XM, YM, UNKNOWN_TOUCHSCREEN_PARAM);

// Use hardware SPI (on Uno, #13, #12, #11) and the above  CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

Rectangle recty  = {0, 40, 0, 60, ILI9341_RED};

// setup for program
void setup() {
  Serial.begin(9600);
  tft.begin(); 
  tft.fillScreen(ILI9341_WHITE);
  // drawRectangle(recty);
}

// main loop
bool run = false;
void loop(void){
  TSPoint p = ts.getPoint();
  if (p.z > ts.pressureThreshhold) {
     p.x = map(p.x, TS_MAXX, TS_MINX, 320, 0);
     p.y = map(p.y, TS_MAXY, TS_MINY, 480, 0);

     Serial.print("X = "); Serial.print(p.x);
     Serial.print("\tY = "); Serial.print(p.y);
     Serial.print("\tPressure = "); Serial.println(p.z);
  }
  // if(tsPointInRectangle(p, recty)) {
  //   tft.fillScreen(ILI9341_RED);
  //   run = true;
  // } else if (run) {
  //   tft.fillScreen(ILI9341_WHITE);
  //   drawRectangle(recty);
  //   run = false;
  // }
  delay(500);
}


void drawRectangle(Rectangle rectangle) {
  tft.fillRect(rectangle.minX, rectangle.minY, rectangle.maxX, rectangle.maxY, rectangle.fillColor);
}

void drawBackground() {
  tft.fillScreen(ILI9341_WHITE);
}

bool tsPointInRectangle(TSPoint p, Rectangle rectangle) {
  if((p.x < rectangle.maxX) and (p.x > rectangle.minX)){
    if((p.y < rectangle.maxY) and (p.y > rectangle.minY)) {
      return true;
    }
  }
  return false;
}


// old code commented below


// implementation of prototypes
//bool buttonPress(TSPoint p) {
//  if((p.x < x2) and (p.x > x1)){
//    if((p.y < y2) and (p.y > y1)) {
//      return true;
//    }
//  }
//  return false;
//}


//unsigned long testFillScreen() {
//  unsigned long start = micros();
//  tft.fillScreen(ILI9341_BLACK);
//  yield();
//  tft.fillScreen(ILI9341_RED);
//  yield();
//  tft.fillScreen(ILI9341_GREEN);
//  yield();
//  tft.fillScreen(ILI9341_BLUE);
//  yield();
//  tft.fillScreen(ILI9341_BLACK);
//  yield();
//  return micros() - start;
//}
