// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "RTClib.h"
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

RTC_DS1307 rtc;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Use hardware SPI (on Uno, #13, #12, #11) and the above  CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  delay(3000);
  tft.begin(); 
  tft.fillScreen(ILI9341_BLACK);

  if (! rtc.begin()) {
    while (1);
  }

  if (! rtc.isrunning()) {
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  delay(1000);
}

void loop () {
    DateTime now = rtc.now();
/*
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);

    Serial.println();
    */
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
    String cur_hour = String(now.hour());
    String cur_min = String(now.minute());
    tft.println(cur_hour + " " + cur_min);    
    tft.println("Hello World!");    
    delay(3000);
}
