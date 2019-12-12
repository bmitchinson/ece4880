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
#include <OneWire.h>              // Temp Sensor Protocol
#include <DallasTemperature.h>    // Temp Sensor Lib
#include "RTClib.h"               // Real Time Clock Lib
// *****************************
// defs
#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 4 // SD card select pin

// Temp data wire
#define ONE_WIRE_BUS 2

// RTC
RTC_DS1307 rtc;

// Touchscreen definitions
#define YP A2 // must be an analog pin, use "An" notation!
#define XM A3 // must be an analog pin, use "An" notation!
#define YM 8  // can be a digital pin
#define XP 7  // can be a digital pin

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
#define UNKNOWN_TOUCHSCREEN_PARAM 300

// *****************************
// configure sd card
#define USE_SD_CARD
#if defined(USE_SD_CARD)
SdFat SD;                        // SD card filesystem
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
Adafruit_SPIFlash flash(&flashTransport);
FatFileSystem filesys;
Adafruit_ImageReader reader(filesys); // Image-reader, pass in flash filesys
#endif

// *****************************

// screen and display objects
TouchScreen ts = TouchScreen(XP, YP, XM, YM, UNKNOWN_TOUCHSCREEN_PARAM);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Configure Temp Sensor:
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int currentTemp;

// structs
class Component
{
public:
  // constructor
  Component(int mx, int my, int Mx, int My, char *fname, TouchScreen *ts, Adafruit_ILI9341 *tft)
  {
    this->filename = fname;
    this->minX = mx;
    this->maxX = Mx;
    this->minY = my;
    this->maxY = My;
    this->tsPtr = ts;
    this->tftPtr = tft;
  }
  // state
  // position data
  int minX;
  int maxX;
  int minY;
  int maxY;
  // pointers to global variables
  TouchScreen *tsPtr;
  Adafruit_ILI9341 *tftPtr;
  // iamge status
  ImageReturnCode stat;
  // filename to read from
  char *filename;

  // functions
  void render()
  {
    Serial.print(F("Loading..."));
    Serial.print(this->filename);
    Serial.print("\nsize:");
    Serial.print(this->minX);
    Serial.print(",");
    Serial.print(this->maxX);
    Serial.print(",");
    Serial.print(this->minY);
    Serial.print(",");
    Serial.print(this->maxY);
    Serial.print("\n");

    Serial.print(F("..DONE"));
    this->stat = reader.drawBMP(this->filename, *(this->tftPtr), this->minX, this->minY);
    reader.printStatus(this->stat);
  }

  bool containsPoint(TSPoint p)
  {
    if ((p.x < this->maxX) and (p.x > this->minX))
    {
      if ((p.y < this->maxY) and (p.y > this->minY))
      {
        return true;
      }
    }
    return false;
  }
};

// *****************************

// *****************************
// LIST OUT ALL COMPONENTS
// ///////////////////////

// set preset
Component swchoff = Component(149, 214, 225, 242, "swchoff.bmp", &ts, &tft);
Component swchonn = Component(149, 214, 225, 242, "swchonn.bmp", &ts, &tft);

Component time_div = Component(0, 52, 240, 164, "time_div.bmp", &ts, &tft);

Component tabfour = Component(0, 290, 240, 320, "tabfour.bmp", &ts, &tft);
Component tabthree = Component(0, 290, 240, 320, "tabthree.bmp", &ts, &tft);
Component tabtwo = Component(0, 290, 240, 320, "tabtwo.bmp", &ts, &tft);
Component tabone = Component(0, 290, 240, 320, "tabone.bmp", &ts, &tft);

Component endtitle = Component(58, 20, 187, 41, "endtitle.bmp", &ts, &tft);
Component daytitle = Component(58, 20, 187, 41, "daytitle.bmp", &ts, &tft);

Component pm_butt = Component(180, 94, 230, 127, "pm_butt.bmp", &ts, &tft);
Component am_butt = Component(180, 94, 230, 127, "am_butt.bmp", &ts, &tft);

Component min_up = Component(99, 63, 172, 81, "min_up.bmp", &ts, &tft);
Component min_down = Component(99, 135, 172, 153, "min_down.bmp", &ts, &tft);
Component tmpleft = Component(11, 189, 29, 262, "tmpleft.bmp", &ts, &tft);
Component tmpright = Component(111, 189, 129, 262, "tmpright.bmp", &ts, &tft);
Component h_up = Component(11, 63, 84, 81, "h_up.bmp", &ts, &tft);
Component h_down = Component(11, 135, 84, 153, "h_down.bmp", &ts, &tft);

// main
Component setlock = Component(12, 11, 74, 75, "/setlock.bmp", &ts, &tft);
Component setpre = Component(89, 11, 151, 75, "/setpre.bmp", &ts, &tft);
Component toghoff = Component(166, 11, 228, 75, "/toghoff.bmp", &ts, &tft);
Component toghon = Component(166, 11, 228, 75, "/toghon.bmp", &ts, &tft);
Component upbutt = Component(148, 112, 221, 130, "/upbutt.bmp", &ts, &tft);
Component downbutt = Component(148, 189, 221, 207, "/downbutt.bmp", &ts, &tft);
Component offbar = Component(24, 226, 217, 305, "/offbar.bmp", &ts, &tft);
Component autobar = Component(24, 226, 217, 305, "/autobar.bmp", &ts, &tft);
Component heatbar = Component(24, 226, 217, 305, "/heatbar.bmp", &ts, &tft);
Component acbar = Component(24, 226, 217, 305, "/acbar.bmp", &ts, &tft);
Component tempspot = Component(17, 105, 129, 217, "/tempspot.bmp", &ts, &tft);

// state
enum ControlSetting
{
  OFF,
  AUTO,
  HEAT,
  ON
};

enum ScreenSetting
{
  MAIN,
  PRESET_SELECT_SET,
  PRESET_SET,
  CLOCK_SET
};
enum PresetSelectSetting
{
  WEEKEND,
  WEEKDAY,
};

bool hold = false;
ControlSetting controlSetting = ControlSetting::OFF;
ScreenSetting screenSetting = ScreenSetting::MAIN;
PresetSelectSetting presetSelectSetting = PresetSelectSetting::WEEKEND;

void renderHoldButton()
{
  if (hold)
  {
    toghon.render();
  }
  else
  {
    toghoff.render();
  }
}

void renderControlBar()
{
  switch (controlSetting)
  {
  case ControlSetting::OFF:
    offbar.render();
    break;

  case ControlSetting::AUTO:
    autobar.render();
    break;

  case ControlSetting::HEAT:
    heatbar.render();
    break;

  case ControlSetting::ON:
    acbar.render();
    break;
  }
}

void renderMainScreen()
{
  tft.fillScreen(ILI9341_WHITE);
  setlock.render();
  setpre.render();
  renderHoldButton();
  upbutt.render();
  downbutt.render();
  renderControlBar();
  tempspot.render();
}

void callbackMainScreen(TSPoint p)
{
  if (toghoff.containsPoint(p))
  {
    Serial.print("contained\n");

    hold = !hold;
    renderHoldButton();
  }
  if (offbar.containsPoint(p))
  {
    int lenOfSubBox = (offbar.maxX - offbar.minX) / 4;
    if (p.x < offbar.minX + (1 * lenOfSubBox))
    {
      controlSetting = ControlSetting::HEAT;
    }
    else if (p.x < offbar.minX + (2 * lenOfSubBox))
    {
      controlSetting = ControlSetting::ON;
    }
    else if (p.x < offbar.minX + (3 * lenOfSubBox))
    {
      controlSetting = ControlSetting::AUTO;
    }
    else
    {
      controlSetting = ControlSetting::OFF;
    }
    renderControlBar();
  }
}

// *****************************
// program setup
void setup()
{
  // Setup serial
  Serial.begin(9600);
#if !defined(ESP32)
  while (!Serial)
    ;
#endif
  // If the SD is messed up talk about it
  Serial.print(F("Initializing filesystem..."));
#if defined(USE_SD_CARD)
  if (!SD.begin(SD_CS, SD_SCK_MHZ(25)))
  {
    Serial.println(F("SD begin() failed"));
    for (;;)
      ;
  }
#else
  if (!flash.begin())
  {
    Serial.println(F("flash begin() failed"));
    for (;;)
      ;
  }
  if (!filesys.begin(&flash))
  {
    Serial.println(F("filesys begin() failed"));
    for (;;)
      ;
  }
#endif

  // by this point sd is good to go
  Serial.println(F("OK!"));

  // Set Up Real Time Clock if not already set up
  
  if (! rtc.begin()) {
    while (1);
  }
  
  if (! rtc.isrunning()) {
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  tft.begin();
  // Fill screen blue. Not a required step, this just shows that we're
  // successfully communicating with the screen.
  tft.fillScreen(ILI9341_WHITE);
  //tft.setRotation(1);
  renderMainScreen();

  delay(1000); // Pause 1 second before moving on to loop()
}

// *****************************

// program loop
void loop(void)
{
  // get and print touch
  TSPoint p = ts.getPoint();
  if (p.z > ts.pressureThreshhold)
  {
    // button press occurred
    Serial.print("X1 = ");
    Serial.print(p.x);
    Serial.print("\tY1 = ");
    Serial.print(p.y);
    Serial.print("\t");
    p.x = map(p.x, TS_MAXX, TS_MINX, 240, 0);
    p.y = map(p.y, TS_MAXY, TS_MINY, 320, 0);

    //    p.x = map(p.x, TS_MAXX, TS_MINX, 240, 0);
    //    p.y = map(p.y, TS_MAXY, TS_MINY, 320, 0);
    //    int newy = p.x;
    //    newx += 240;
    //    p.x = newx;
    //    p.y = newy;
    // p.y=320-p.y;
    Serial.print("X2 = ");
    Serial.print(p.x);
    Serial.print("\tY2 = ");
    Serial.print(p.y);
    Serial.print("\tPressure = ");
    Serial.println(p.z);

    switch (screenSetting)
    {
    case ScreenSetting::MAIN:
      Serial.print("main\n");
      callbackMainScreen(p);
      //renderMainScreen();
      break;
    case ScreenSetting::PRESET_SELECT_SET:
      break;
    case ScreenSetting::PRESET_SET:
      break;
    case ScreenSetting::CLOCK_SET:
      break;
    }
  }
  drawCurrentTemp();
  drawCurrentTime();
  drawSetTemp();
  drawRectangle(toghoff);
  delay(1000);
}

// *****************************
// helpter function defs

void drawCurrentTemp() {
  // clear the field
  tft.fillRect(30,135, 80, 75, ILI9341_WHITE);
  tft.setCursor(30, 135);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(7);
  // TODO replace with current temp val
  getTemp();
  tft.println(currentTemp);
}

void drawSetTemp(){
  tft.setCursor(157, 144);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(5);
  // TODO replace with current set val
  tft.println("68");
}

void drawCurrentTime(){
  // clear the field
  tft.fillRect(45, 280, 250, 25, ILI9341_WHITE);
  tft.setCursor(45, 280);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
  // TODO replace with current time
  DateTime now = rtc.now();
  getTemp();
  String time = String(now.month()) + "/" + String(now.day()) + "  " + String(now.hour()) + ":" + String(now.minute());
  tft.println(time);
}

void drawRectangle(Component c) {
    Serial.print("cmin = (");
    Serial.print(c.minX);
    Serial.print(",");
    Serial.print(c.minY);
    Serial.print(") cmax = ");
    Serial.print(c.maxX);
    Serial.print(",");
    Serial.print(c.maxY);
    Serial.print(")\n");
  tft.drawRect(c.minX, c.minY, c.maxX-c.minX, c.maxY-c.minY, ILI9341_RED);
}

// Temp Sensor Helper
void getTemp(){
  sensors.requestTemperatures(); // Send the command to get temperatures  
  currentTemp = (sensors.getTempCByIndex(0) * 1.8) + 32;
}

// *****************************
