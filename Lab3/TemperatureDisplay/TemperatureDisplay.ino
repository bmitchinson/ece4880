// *****************************
// library imports
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

// TODO
// -- store presets in EEPROM -> Zain
// -- preset setting --> Alex -- DONE
// -- preset storing --> Zain
// -- clock setting --> Alex
// -- clock storing --> Zain
// -- LED functionality --> Zain

// *****************************
// global constants
#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 4 // SD card select pin

// Temp data wire
#define ONE_WIRE_BUS 2

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
// temperature
#define MAX_SET_TEMP 99
#define MIN_SET_TEMP 50

// initialize global object for touch screen
#define UNKNOWN_TOUCHSCREEN_PARAM 300
// *****************************


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

// *****************************
// our imports
#include "Component.h"
#include "Preset.h"
// state enums
#include "EnumControlSetting.h"
#include "EnumScreenSetting.h"
#include "EnumPresetTabSelectSetting.h"
#include "EnumPresetSelectSetSetting.h"
// *****************************

// *****************************
// global variables

// RTC
RTC_DS1307 rtc;
// screen and display objects
TouchScreen ts = TouchScreen(XP, YP, XM, YM, UNKNOWN_TOUCHSCREEN_PARAM);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Configure Temp Sensor:
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

int currentTemp = 0;
int pastTemp = 0;
int setTemp = 51;
// *****************************



// *****************************
// LIST OUT ALL COMPONENTS

// set time
Component dayleft = Component(21, 199, 39, 248, "dayleft.bmp", &ts, &tft);
Component dayright = Component(201, 199, 219, 248, "dayright.bmp", &ts, &tft);
Component stimediv = Component(0, 71, 240, 183, "stimediv.bmp", &ts, &tft);
Component t_pm_but = Component(182, 110, 232, 143, "t_pm_but.bmp", &ts, &tft);
Component t_am_but = Component(182, 110, 232, 143, "t_am_but.bmp", &ts, &tft);
Component t_min_up = Component(102, 81, 175, 99, "t_min_up.bmp", &ts, &tft);
Component t_min_dn = Component(102, 153, 175, 171, "t_min_dn.bmp", &ts, &tft);
Component t_hr_up = Component(14, 81, 87, 99, "t_hr_up.bmp", &ts, &tft);
Component t_hr_dn = Component(14, 153, 87, 171, "t_hr_dn.bmp", &ts, &tft);
Component t_sv_but = Component(136, 278, 201, 311, "t_sv_but.bmp", &ts, &tft);
Component t_bc_but = Component(42, 278, 107, 311, "t_bc_but.bmp", &ts, &tft);
Component setclock = Component(64, 21, 177, 59, "setclock.bmp", &ts, &tft);

// select set preset
Component pretitle = Component(54, 22, 191, 47, "pretitle.bmp", &ts, &tft);
Component endbutt = Component(61, 72, 184, 143, "endbutt.bmp", &ts, &tft);
Component backbutt = Component(86, 265, 151, 298, "backbutt.bmp", &ts, &tft);
Component daybutt = Component(61, 159, 184, 230, "daybutt.bmp", &ts, &tft);

// set preset
Component swchoff = Component(149, 214, 225, 242, "swchoff.bmp", &ts, &tft);
Component swchon = Component(149, 214, 225, 242, "swchon.bmp", &ts, &tft);
Component time_div = Component(0, 52, 240, 164, "time_div.bmp", &ts, &tft);
Component tabfour = Component(0, 290, 240, 320, "tabfour.bmp", &ts, &tft);
Component tabthree = Component(0, 290, 240, 320, "tabthree.bmp", &ts, &tft);
Component tabtwo = Component(0, 290, 240, 320, "tabtwo.bmp", &ts, &tft);
Component tabone = Component(0, 290, 240, 320, "tabone.bmp", &ts, &tft);
Component endtitle = Component(88, 20, 217, 41, "endtitle.bmp", &ts, &tft);
Component daytitle = Component(88, 20, 217, 41, "daytitle.bmp", &ts, &tft);
Component pm_butt = Component(180, 94, 230, 127, "pm_butt.bmp", &ts, &tft);
Component am_butt = Component(180, 94, 230, 127, "am_butt.bmp", &ts, &tft);
Component min_up = Component(99, 63, 172, 81, "min_up.bmp", &ts, &tft);
Component min_down = Component(99, 135, 172, 153, "min_down.bmp", &ts, &tft);
Component tmpleft = Component(11, 189, 29, 262, "tmpleft.bmp", &ts, &tft);
Component tmpright = Component(111, 189, 129, 262, "tmpright.bmp", &ts, &tft);
Component h_up = Component(11, 63, 84, 81, "h_up.bmp", &ts, &tft);
Component h_down = Component(11, 135, 84, 153, "h_down.bmp", &ts, &tft);
Component set_preset_back_button = Component(0, 10, 65, 43, "t_bc_but.bmp", &ts, &tft);


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
// *****************************

// *****************************
// list of all state that will need to be stored in EEPROM
Preset preEnd1 = Preset(12, 0, true, false, 72);
Preset preEnd2 = Preset(12, 0, true, false, 72);
Preset preEnd3 = Preset(12, 0, true, false, 72);
Preset preEnd4 = Preset(12, 0, true, false, 72);
Preset preDay1 = Preset(12, 0, true, true, 72);
Preset preDay2 = Preset(12, 0, true, true, 72);
Preset preDay3 = Preset(12, 0, true, true, 72);
Preset preDay4 = Preset(12, 0, true, true, 72);
// *****************************



// *****************************
// all screen state variables
// global variables
ScreenSetting screenSetting = ScreenSetting::MAIN;

// main screen variables
ControlSetting controlSetting = ControlSetting::OFF;
bool hold = false;

// clock set screen variables
bool timeIsAm = true;

// preset select set screen variables
PresetSelectSetSetting presetSelectSetSetting = PresetSelectSetSetting::WEEKEND;

// preset set screen variables
PresetTabSelectSetting presetTabSelectSetting = PresetTabSelectSetting::ONE;
bool presetActive = true;
bool presetIsAm = true;
Preset *activePreset;
// *****************************


// *****************************
// RENDERING functions
// MAIN MODE -- DONE
void renderHoldButton() {
    if (hold) {
        toghon.render();
    } else {
        toghoff.render();
    }
}

void renderControlBar() {
    switch (controlSetting) {
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

void renderMainScreen() {
    tft.fillScreen(ILI9341_WHITE);
    renderHoldButton();
    renderControlBar();
    setlock.render();
    setpre.render();
    upbutt.render();
    downbutt.render();
    tempspot.render();
}


// PRESET SELECT SET -- DONE
void renderPresetSelectSetScreen() {
    tft.fillScreen(ILI9341_WHITE);
    pretitle.render();
    endbutt.render();
    backbutt.render();
    daybutt.render();
}

// PRESET SET MODE
void renderActivePresetActiveSwitch() {
    if (activePreset->isActive()) {
        swchon.render();
    } else {
        swchoff.render();
    }
}

void renderPresetTabs() {
    switch (presetTabSelectSetting) {
        case PresetTabSelectSetting::ONE:
            tabone.render();
            break;
        case PresetTabSelectSetting::TWO:
            tabtwo.render();
            break;
        case PresetTabSelectSetting::THREE:
            tabthree.render();
            break;
        case PresetTabSelectSetting::FOUR:
            tabfour.render();
            break;
    }
}


void renderWeekDayWeekendTitle() {
    if (presetSelectSetSetting == PresetSelectSetSetting::WEEKDAY) {
        daytitle.render();
    } else if (presetSelectSetSetting == PresetSelectSetSetting::WEEKEND) {
        endtitle.render();
    }
}

void setActivePreset() {
    switch (presetSelectSetSetting) {
        case PresetSelectSetSetting::WEEKEND:
            switch (presetTabSelectSetting) {
                case PresetTabSelectSetting::ONE:
                    activePreset = &preEnd1;
                    break;
                case PresetTabSelectSetting::TWO:
                    activePreset = &preEnd2;
                    break;
                case PresetTabSelectSetting::THREE:
                    activePreset = &preEnd3;
                    break;
                case PresetTabSelectSetting::FOUR:
                    activePreset = &preEnd4;
                    break;
            }
            break;
        case PresetSelectSetSetting::WEEKDAY:
            switch (presetTabSelectSetting) {
                case PresetTabSelectSetting::ONE:
                    activePreset = &preDay1;
                    break;
                case PresetTabSelectSetting::TWO:
                    activePreset = &preDay2;
                    break;
                case PresetTabSelectSetting::THREE:
                    activePreset = &preDay3;
                    break;
                case PresetTabSelectSetting::FOUR:
                    activePreset = &preDay4;
                    break;
            }
            break;
    }
}

void renderActivePresetMinute() {
    clearArea(min_up.minX, min_up.maxY + 3, min_up.maxX - min_up.minX, min_down.minY - min_up.maxY - 6);
    drawTextSetup(((int) (min_up.maxX - min_up.minX) / 5) + min_up.minX,
                  ((int) (min_down.minY - min_up.maxY) / 5) + min_up.maxY, 4);
    if (activePreset->getMinute() < 10) {
        tft.print(0);
    }
    tft.println(activePreset->getMinute());
}

void renderActivePresetHour() {
    clearArea(h_up.minX, h_up.maxY + 3, h_up.maxX - h_up.minX, h_down.minY - h_up.maxY - 6);
    drawTextSetup(((int) (h_up.maxX - h_up.minX) / 5) + h_up.minX,
                  ((int) (h_down.minY - h_up.maxY) / 5) + h_up.maxY, 4);
    if (activePreset->getHour() < 10) {
        tft.print(0);
    }
    tft.println(activePreset->getHour());
}

void renderActivePresetAmPm() {
    if (activePreset->isAm()) {
        am_butt.render();
    } else {
        pm_butt.render();
    }
}

void renderActivePresetTemp() {
    clearArea(tmpleft.maxX + 3, tmpleft.minY, tmpright.minX - tmpleft.maxX - 6, tmpleft.maxY - tmpleft.minY);
    drawTextSetup(((int) (tmpright.minX - tmpleft.maxX) / 5) + tmpleft.maxX,
                  ((int) (tmpright.maxY - tmpright.minY) / 4) + tmpright.minY, 4);
    tft.println(activePreset->getTemp());
}

void renderActivePreset() {
    setActivePreset();
    renderActivePresetTime();
    renderActivePresetActiveSwitch();
    renderActivePresetTemp();
}

void renderActivePresetTime() {
    renderActivePresetAmPm();
    renderActivePresetHour();
    renderActivePresetMinute();
}

void renderPresetSetScreen() {
    tft.fillScreen(ILI9341_WHITE);
    set_preset_back_button.render();
    renderPresetTabs();
    time_div.render();
    renderWeekDayWeekendTitle();
    min_up.render();
    min_down.render();
    tmpleft.render();
    tmpright.render();
    h_up.render();
    h_down.render();
    renderActivePreset();
}

// SET CLOCK RENDERING
void renderTimeAmPm() {
    if (timeIsAm) {
        t_am_but.render();
    } else {
        t_pm_but.render();
    }
}

void renderClockSetScreen() {
    tft.fillScreen(ILI9341_WHITE);
    dayleft.render();
    dayright.render();
    stimediv.render();
    renderTimeAmPm();
    t_min_up.render();
    t_min_dn.render();
    t_hr_up.render();
    t_hr_dn.render();
    t_sv_but.render();
    t_bc_but.render();
    setclock.render();
}
// *****************************


// *****************************
// CALLBACKS
// MAIN -- DONE
bool maxed_out = false;
bool mined_out = false;

void callbackMainScreen(TSPoint p) {
    // update setTemp
    if (upbutt.containsPoint(p)) {
        setTemp += 1;
        if (mined_out) {
            downbutt.render();
            mined_out = false;
        }
        if (setTemp > MAX_SET_TEMP) {
            maxed_out = true;
            setTemp = MAX_SET_TEMP;
        }
        drawSetTemp();
        if (maxed_out) {
            drawRectangle(upbutt);
        }
    } else if (downbutt.containsPoint(p)) {
        setTemp -= 1;
        if (maxed_out) {
            upbutt.render();
            maxed_out = false;
        }
        if (setTemp < MIN_SET_TEMP) {
            mined_out = true;
            setTemp = MIN_SET_TEMP;
        }
        drawSetTemp();
        if (mined_out) {
            drawRectangle(downbutt);
        }
    }

        // toggle hold
    else if (toghoff.containsPoint(p)) {
        hold = !hold;
        renderHoldButton();
    }
        // toggle contorl bar
    else if (offbar.containsPoint(p)) {
        int lenOfSubBox = (offbar.maxX - offbar.minX) / 4;
        if (p.x < offbar.minX + (1 * lenOfSubBox)) {
            controlSetting = ControlSetting::HEAT;
        } else if (p.x < offbar.minX + (2 * lenOfSubBox)) {
            controlSetting = ControlSetting::ON;
        } else if (p.x < offbar.minX + (3 * lenOfSubBox)) {
            controlSetting = ControlSetting::AUTO;
        } else {
            controlSetting = ControlSetting::OFF;
        }
        renderControlBar();
    }
        // Cross page navigation
    else if (setlock.containsPoint(p)) {
        Serial.println('to cs');
        screenSetting = ScreenSetting::CLOCK_SET;
        renderClockSetScreen();
    } else if (setpre.containsPoint(p)) {
        Serial.println('to pss');
        screenSetting = ScreenSetting::PRESET_SELECT_SET;
        renderPresetSelectSetScreen();
    }
}

// PRESET SELECT SET -- DONE
void callbackPresetSelectSet(TSPoint p) {
    // only cross page navigation
    if (endbutt.containsPoint(p)) {
        presetSelectSetSetting = PresetSelectSetSetting::WEEKEND;
        screenSetting = ScreenSetting::PRESET_SET;
        renderPresetSetScreen();
    } else if (daybutt.containsPoint(p)) {
        presetSelectSetSetting = PresetSelectSetSetting::WEEKDAY;
        screenSetting = ScreenSetting::PRESET_SET;
        renderPresetSetScreen();
    } else if (backbutt.containsPoint(p)) {
        Serial.println('to m');
        screenSetting = ScreenSetting::MAIN;
        renderMainScreen();
    }
}

// CLOCK SET
void callbackClockSet(TSPoint p) {
    // TODO: add time manipulation callbacks

    // only cross page navigation
    if (t_sv_but.containsPoint(p)) {
        // TODO: save new clock state to EEPORM
        Serial.println('to m');
        screenSetting = ScreenSetting::MAIN;
        renderMainScreen();
    } else if (t_bc_but.containsPoint(p)) {
        Serial.println('to m');
        screenSetting = ScreenSetting::MAIN;
        renderMainScreen();
    }
}

// PRESET SET -- DONE
void callbackPresetSet(TSPoint p) {
    if (min_down.containsPoint(p)) {
        activePreset->decMinute();
        renderActivePresetTime();
    } else if (min_up.containsPoint(p)) {
        activePreset->incMinute();
        renderActivePresetTime();
    } else if (h_down.containsPoint(p)) {
        activePreset->decHour();
        renderActivePresetTime();
    } else if (h_up.containsPoint(p)) {
        activePreset->incHour();
        renderActivePresetTime();
    } else if (am_butt.containsPoint(p)) {
        activePreset->toggleAmPm();
        renderActivePresetTime();
    } else if (swchon.containsPoint(p)) {
        activePreset->toggleActive();
        renderActivePresetActiveSwitch();
    } else if (tmpright.containsPoint(p)) {
        int tmp = activePreset->getTemp();
        tmp += 1;
        if (tmp > MAX_SET_TEMP) {
            tmp = MAX_SET_TEMP;
        }
        activePreset->setTemp(tmp);
        renderActivePresetTemp();
    } else if (tmpleft.containsPoint(p)) {
        int tmp = activePreset->getTemp();
        tmp -= 1;
        if (tmp < MIN_SET_TEMP) {
            tmp = MIN_SET_TEMP;
        }
        activePreset->setTemp(tmp);
        renderActivePresetTemp();
    } else if (tabone.containsPoint(p)) {
        int lenOfSubBox = (tabone.maxX - tabone.minX) / 4;
        if (p.x < tabone.minX + (1 * lenOfSubBox)) {
            presetTabSelectSetting = PresetTabSelectSetting::ONE;
            tabone.render();
        } else if (p.x < tabone.minX + (2 * lenOfSubBox)) {
            presetTabSelectSetting = PresetTabSelectSetting::TWO;
            tabtwo.render();
        } else if (p.x < tabone.minX + (3 * lenOfSubBox)) {
            presetTabSelectSetting = PresetTabSelectSetting::THREE;
            tabthree.render();
        } else {
            presetTabSelectSetting = PresetTabSelectSetting::FOUR;
            tabfour.render();
        }
        setActivePreset();
        renderActivePreset();
    } else if (set_preset_back_button.containsPoint(p)) {
        screenSetting = ScreenSetting::PRESET_SELECT_SET;
        renderPresetSelectSetScreen();
        // TODO: store presets in EEPROM
    }
}
// *****************************


// *****************************
// program setup
void setup() {
    // Setup serial
    Serial.begin(9600);
#if !defined(ESP32)
    while (!Serial);
#endif
    // If the SD is messed up talk about it
    Serial.print(F("Initializing filesystem..."));
#if defined(USE_SD_CARD)
    if (!SD.begin(SD_CS, SD_SCK_MHZ(25))) {
        Serial.println(F("SD begin() failed"));
        for (;;);
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

    if (!rtc.begin()) {
        while (1);
    }

    if (!rtc.isrunning()) {
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    tft.begin();
    // initial rendering
    renderMainScreen();
    //renderPresetSetScreen();
    // renderPresetSelectSetScreen();
    delay(1000); // Pause 1 second before moving on to loop()
}
// *****************************


// *****************************
// program loop
bool reDrawSetTemp = true;

void loop(void) {
    // get and print touch
    TSPoint p = ts.getPoint();
    if (p.z > ts.pressureThreshhold) {
        // button press occurred
        Serial.print("X1 = ");
        Serial.print(p.x);
        Serial.print("\tY1 = ");
        Serial.print(p.y);
        Serial.print("\t");
        p.x = map(p.x, TS_MAXX, TS_MINX, 240, 0);
        p.y = map(p.y, TS_MAXY, TS_MINY, 320, 0);
        Serial.print("X2 = ");
        Serial.print(p.x);
        Serial.print("\tY2 = ");
        Serial.print(p.y);
        Serial.print("\tPressure = ");
        Serial.println(p.z);

        // call the callback function neccesary to handle the touch
        Serial.println(screenSetting);

        switch (screenSetting) {
            case ScreenSetting::MAIN:
                callbackMainScreen(p);
                Serial.println('m');
                break;
            case ScreenSetting::PRESET_SELECT_SET:
                callbackPresetSelectSet(p);
                Serial.println('pss');
                break;
            case ScreenSetting::PRESET_SET:
                callbackPresetSet(p);
                Serial.println('ps');

                break;
            case ScreenSetting::CLOCK_SET:
                callbackClockSet(p);
                Serial.println('cs');

                break;
        }
    }

    // if on main draw this text
    if (screenSetting == ScreenSetting::MAIN) {
        getTemp();
        if (pastTemp != currentTemp) {
            drawCurrentTemp();
        }
        if (reDrawSetTemp) {
            drawSetTemp();
            reDrawSetTemp = false;
        }
        drawCurrentTime();
    } else {
        pastTemp = 0;
        reDrawSetTemp = true;
    }
    delay(1000);
}
// *****************************


// *****************************
// helper function defs
void drawTextSetup(int xCursor, int yCursor, int txtSize) {
    tft.setCursor(xCursor, yCursor);
    tft.setTextSize(txtSize);
    tft.setTextColor(ILI9341_BLACK);
}

void clearArea(int xStart, int yStart, int width, int height) {
    tft.fillRect(xStart, yStart, width, height, ILI9341_WHITE);
}

void drawCurrentTemp() {
    // clear the field
    clearArea(tempspot.minX + 2, tempspot.minY + 2, tempspot.maxX - tempspot.minX - 4,
              tempspot.maxY - tempspot.minY - 4);
    drawTextSetup(30, 135, 7);
    getTemp();
    tft.println(currentTemp);
}

void drawSetTemp() {
    clearArea(upbutt.minX, upbutt.maxY + 3, upbutt.maxX - upbutt.minX, downbutt.minY - upbutt.maxY - 6);
    drawTextSetup(157, 144, 5);
    tft.println(setTemp);
}

void drawCurrentTime() {
    // clear the field
    clearArea(45, 280, 250, 25);
    drawTextSetup(45, 280, 2);
    DateTime now = rtc.now();
    String time =
            String(now.month()) + "/" + String(now.day()) + "  " + String(now.hour()) + ":" + String(now.minute());
    tft.println(time);
}

void drawRectangle(Component c) {
    tft.drawRect(c.minX, c.minY, c.maxX - c.minX, c.maxY - c.minY, ILI9341_RED);
}

// Temp Sensor Helper
void getTemp() {
    sensors.requestTemperatures(); // Send the command to get temperatures
    pastTemp = currentTemp;
    currentTemp = (sensors.getTempCByIndex(0) * 1.8) + 32;
    if (currentTemp > MAX_SET_TEMP) {
        currentTemp = MAX_SET_TEMP;
    }
}
// *****************************
