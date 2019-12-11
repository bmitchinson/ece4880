
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <stdint.h> 
#include "TouchScreen.h"
#include <SdFat.h>                // SD card & FAT filesystem library
#include <Adafruit_SPIFlash.h>    // SPI / QSPI flash library
#include <Adafruit_ImageReader.h> // Image-reading functions

#define USE_SD_CARD

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
#define SD_CS   4 // SD card select pin

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

#if defined(USE_SD_CARD)
  SdFat                SD;         // SD card filesystem
  Adafruit_ImageReader reader(SD); // Image-reader object, pass in SD filesys
#else
  // SPI or QSPI flash filesystem (i.e. CIRCUITPY drive)
  #if defined(__SAMD51__) || defined(NRF52840_XXAA)
    Adafruit_FlashTransport_QSPI flashTransport(PIN_QSPI_SCK, PIN_QSPI_CS,
      PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3);
  #else
    #if (SPI_INTERFACES_COUNT == 1)
      Adafruit_FlashTransport_SPI flashTransport(SS, &SPI);
    #else
      Adafruit_FlashTransport_SPI flashTransport(SS1, &SPI1);
    #endif
  #endif
  Adafruit_SPIFlash    flash(&flashTransport);
  FatFileSystem        filesys;
  Adafruit_ImageReader reader(filesys); // Image-reader, pass in flash filesys
#endif

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

Adafruit_Image       img;        // An image loaded into RAM
int32_t              width  = 0, // BMP image dimensions
                     height = 0;

Rectangle recty  = {0, 40, 0, 60, ILI9341_RED};

// setup for program
void setup() {
  ImageReturnCode stat; // Status from image-reading functions
  Serial.begin(9600);
  #if !defined(ESP32)
  while(!Serial);       // Wait for Serial Monitor before continuing
#endif
  Serial.print(F("Initializing filesystem..."));
#if defined(USE_SD_CARD)
  // SD card is pretty straightforward, a single call...
  if(!SD.begin(SD_CS, SD_SCK_MHZ(25))) { // ESP32 requires 25 MHz limit
    Serial.println(F("SD begin() failed"));
    for(;;); // Fatal error, do not continue
  }
#else
  // SPI or QSPI flash requires two steps, one to access the bare flash
  // memory itself, then the second to access the filesystem within...
  if(!flash.begin()) {
    Serial.println(F("flash begin() failed"));
    for(;;);
  }
  if(!filesys.begin(&flash)) {
    Serial.println(F("filesys begin() failed"));
    for(;;);
  }
#endif
  Serial.println(F("OK!"));

  tft.begin(); 
  // Fill screen blue. Not a required step, this just shows that we're
  // successfully communicating with the screen.
  tft.fillScreen(ILI9341_BLUE);
  tft.setRotation(1);
  // drawRectangle(recty);

  // Load full-screen BMP file' at position (0,0) (top left).
  // Notice the 'reader' object performs this, with 'tft' as an argument.
  Serial.print(F("Loading demo.bmp to screen..."));
  stat = reader.drawBMP("/demo.bmp", tft, 0, 0);
  reader.printStatus(stat);   // How'd we do?

  delay(1000); // Pause 2 seconds before moving on to loop()
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
