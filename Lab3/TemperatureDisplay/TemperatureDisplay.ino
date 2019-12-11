// *****************************
// imports
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <stdint.h> 
#include "TouchScreen.h"
#include <SdFat.h>                // SD card & FAT filesystem library
#include <Adafruit_SPIFlash.h>    // SPI / QSPI flash library
#include <Adafruit_ImageReader.h> // Image-reading functions

// *****************************
// defs
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

// initialize global object for touch screen
# define UNKNOWN_TOUCHSCREEN_PARAM 300

// *****************************
// configure sd card
#define USE_SD_CARD
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

// *****************************
// structs
class Component {
public:
  Component(int mx, int Mx, int my, int My, char* fname, TouchScreen* ts, Adafruit_ILI9341* tft){
    this->minX = mx;
    this->maxX = Mx;
    this->minY = my;
    this->maxY = My;
    this->filename = fname;
    this->tsPtr = ts;
    this->tftPtr = tft;
  }
  // position data
  int minX;
  int maxX;
  int minY;
  int maxY;
  // pointers to global variables
  TouchScreen* tsPtr;
  Adafruit_ILI9341* tftPtr;
  // iamge status
  ImageReturnCode stat;
  // filename to read from
  char* filename;
  // draw the Image
  void render(){
      Serial.print(F("Loading...\n"));
      this->stat = reader.drawBMP(this->filename, *(this->tftPtr), this->minX, this->minY);
      reader.printStatus(this->stat);
  }

  bool containsPoint(TSPoint p) {
    if((p.x < this->maxX) and (p.x > this->minX)){
      if((p.y < this->maxY) and (p.y > this->minY)) {
        return true;
      }
    }
    return false;
  }
};


// *****************************


// *****************************
// screen and display objects
TouchScreen ts = TouchScreen(XP, YP, XM, YM, UNKNOWN_TOUCHSCREEN_PARAM);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
ImageReturnCode stat; // Status from image-reading functions
Adafruit_Image img; // An image loaded into RAM
// int32_t width  = 0, height = 0;

Component demoComponenet  = Component(0, VIEW_MAX_X, 0, VIEW_MAX_Y, "/demo.bmp", &ts, &tft);

// *****************************
// program setup
void setup() {
  // Setup serial
  Serial.begin(9600);
  #if !defined(ESP32)
    while(!Serial);
  #endif
  // If the SD is messed up talk about it
  Serial.print(F("Initializing filesystem..."));
  #if defined(USE_SD_CARD)
    if(!SD.begin(SD_CS, SD_SCK_MHZ(25))) {
      Serial.println(F("SD begin() failed"));
      for(;;);
    }
  #else
    if(!flash.begin()) {
      Serial.println(F("flash begin() failed"));
      for(;;);
    }
    if(!filesys.begin(&flash)) {
      Serial.println(F("filesys begin() failed"));
      for(;;);
    }
  #endif
  // by this point sd is good to go
  Serial.println(F("OK!"));

  tft.begin(); 
  // Fill screen blue. Not a required step, this just shows that we're
  // successfully communicating with the screen.
  tft.fillScreen(ILI9341_BLUE);
  tft.setRotation(1);

  // Serial.print(F("Loading demo.bmp to screen..."));
  // stat = reader.drawBMP("/demo.bmp", tft, 0, 0);
  // reader.printStatus(stat);   // How'd we do?
  demoComponenet.render();

  delay(1000); // Pause 1 second before moving on to loop()
}
// *****************************
// program loop state
bool run = false;
// *****************************
// program loop
void loop(void){
  // get and print touch
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
  delay(200);
}

// *****************************
// helpter function defs


// *****************************
