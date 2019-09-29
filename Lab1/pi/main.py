#!/usr/bin/env python
# Alexander Powers (alexander-powers@uiowa.edu)

# for delaying each individual process ( so they take turns better )
from time import sleep

# thermometer library
from w1thermsensor import W1ThermSensor

# database library
import firebase_admin
from firebase_admin import credentials
from firebase_admin import firestore

# custom Temperature class
from Temperature import Temperature

# LETS MULTI-PROCESS THIS BITCH
from multiprocessing import Manager, Process

# Display Libraries
import Adafruit_Nokia_LCD as LCD
import Adafruit_GPIO.SPI as SPI

# really image processing library, but used for LCD
from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

cred = credentials.Certificate("./lab1-firebase-admin-sdk-key.json")
firebase_admin.initialize_app(cred)
sleep_duration = 1

DC = 23
RST = 24
SPI_PORT = 0
SPI_DEVICE = 0

disp = LCD.PCD8544(DC, RST, spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE, max_speed_hz=4000000))
font = ImageFont.load_default()

# Initialize library.
disp.begin(contrast=60)

# Clear display.
disp.clear()
disp.display()


def button_is_pressed():
    # replace this with some logic that determines if the button is pressed
    return True


def switch_is_on():
    # replace this with some logic that determines if the switch is on
    return True


def read_and_send_temp_process(state, individual_run=False):
    db = firestore.client()
    temps = db.collection(u"temps")
    sensor = W1ThermSensor()
    while True:
        sleep(SLEEP_DURATION)
        send_temps = switch_is_on() and state['a']
        if send_temps:
            new_doc = temps.document()
            current_temp = senesor.get_temperature
            state["current_temp"] = current_temp
            new_temp = Temperature(current_temp)
            new_doc.set(new_temp.to_dict(firestore_timestamp=True))
            print("Saved: {0}, to firestore".format(str(new_temp)))


def update_state_process(state, individual_run=False):
    def write_unpluged_to_lcd():
          image = Image.new('1', (LCD.LCDWIDTH, LCD.LCDHEIGHT))
          draw = ImageDraw.Draw(image)
          draw.rectangle((0,0,LCD.LCDWIDTH,LCD.LCDHEIGHT), outline=255, fill=255)
          formatted_str = "! DISCONNECTED !"
          draw.text((8,LCD.LCDHEIGHT/2), formatted_str, font=font)
          disp.image(image)
          disp.display()

    def write_temp_to_lcd(temperature : int):
          image = Image.new('1', (LCD.LCDWIDTH, LCD.LCDHEIGHT))
          draw = ImageDraw.Draw(image)
          draw.rectangle((0,0,LCD.LCDWIDTH,LCD.LCDHEIGHT), outline=255, fill=255)
          formatted_str = "Temp: %.2f." % temperature
          draw.text((8,LCD.LCDHEIGHT/2), formatted_str, font=font)
          disp.image(image)
          disp.display()

    db = firestore.client()
    prefs = db.collection(u"preferences")
    while True:
        sleep(SLEEP_DURATION)
        # read state from firestore
        state["b"] = not state["b"]


def update_display_process(state, individual_run=False):
    disp = LCD.PCD8544(DC, RST, spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE, max_speed_hz=4000000))
    font = ImageFont.load_default()
    disp.begin(contrast=60)
    disp.clear()
    disp.display()

    while True:
        sleep(SLEEP_DURATION)
        # check some condition

        print("idk what to do here yet")
        current_temp = state["current_temp"]
        # display the current temperature

def hardware_io_process(state, individual_run=False):
    # lol we should figure this out....
    while True:
        print('hardware sucks')
        sleep(SLEEP_DURATION)


def main():
    with Manager() as manager:
        state_dict = manager.dict()
        # Read initial state from firestore
        db = firestore.client()
        prefs = db.collection(u"preferences")
        state_dict["firestore_state"] = {}  # put state here
        state_dict["local_state"] = {}
        state_dict["current_temp"] = 0.0;
        try:
            processes = [
                Process(target=read_and_send_temp_process, args=(state_dict,)),
                Process(target=update_state_process, args=(state_dict,)),
                Process(target=update_display_process, args=(state_dict,)),
                Process(target=hardware_io_process, args=(state_dict,)),
            ]
            [p.start() for p in processes]
            [p.join() for p in processes]
        finally:
            print("BYE BYE")
            [p.join() for p in processes]


if __name__ == "__main__":
    main()

