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

# Pins
import RPi.GPIO as GPIO

# Process stuff
import schedule
import threading

cred = credentials.Certificate("./lab1-firebase-admin-sdk-key.json")
firebase_admin.initialize_app(cred)
SLEEP_DURATION = 1

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

def read_temp_process(state):
    state['update_display'] = True
    sensor = state['sensor']
    try:
        new_temp = Temperature(sensor.get_temperature())
        state["current_temp"] = new_temp
        print("Saved: {0}".format(str(new_temp)))
        state['isDisconnected'] = False
    except Exception as e:
        state['isDisconnected'] = True
        print('sensor unpluged')
    if not state['isDisconnected']:
        try:
            send_temp_to_firebase(state)
        except Exception as e:
            print('problem pushing to firebase:')
            print(e)
    try:
        send_connection_status_to_firebase(state)
    except Exception as e:
        print('problem pushing isDisconnected')
        print(e)

    # TODO:
    #if pi_should_text
        # send_text()

def send_connection_status_to_firebase(state):
    state['sensor_document'].set({u'isDisconnected': state['isDisconnected']});
    if state['isDisconnected']:
        print('"sent is disconnected"')
    else:
        print('sent "sensor is not disconnected"')
    


def send_temp_to_firebase(state):
    new_doc = state['temps_collection'].document()
    new_temp = state["current_temp"]
    new_doc.set(new_temp.to_dict(firestore_timestamp=True))
    print("Pushed: {0}".format(str(new_temp)))

#TODO: pi_should_text()

#TODO: send_text()

#TODO: pull_firebase_to_state()
# pull everything, only thing that needs interpretation is the button
# print('texting preferences updated')
# print('display turrned on remotely')
# print('display turned off remotely')

# TODO: case: button is pressed when website is loaded
#                 ...(is there an initial get for it's opening state)


def update_display_process(state):
    if state['update_display'] and state['isPressed']:
        state['update_display'] = False
        disp = state['disp']
        font = state['font']
        disp.clear()
        disp.display()

        def write_unpluged_to_lcd():
              image = Image.new('1', (LCD.LCDWIDTH, LCD.LCDHEIGHT))
              draw = ImageDraw.Draw(image)
              draw.rectangle((0,0,LCD.LCDWIDTH,LCD.LCDHEIGHT), outline=255, fill=255)
              formatted_str = "! DISCONNECTED !"
              draw.text((8,LCD.LCDHEIGHT/2), formatted_str, font=font)
              disp.image(image)
              disp.display()

        def write_temp_to_lcd(temperature : Temperature):
              image = Image.new('1', (LCD.LCDWIDTH, LCD.LCDHEIGHT))
              draw = ImageDraw.Draw(image)
              draw.rectangle((0,0,LCD.LCDWIDTH,LCD.LCDHEIGHT), outline=255, fill=255)
              formatted_str = "Temp: %.2f." % temperature.temp
              draw.text((8,LCD.LCDHEIGHT/2), formatted_str, font=font)
              disp.image(image)
              disp.display()


        current_temp = state["current_temp"]
        if state['isDisconnected']:
            write_unpluged_to_lcd()
        else:
            write_temp_to_lcd(current_temp)

def main():

    # Pins
    GPIO.setmode(GPIO.BCM)
    BACKLIGHT_PIN = 26
    PUSH_BUTTON_PIN = 17
    SWITCH_PIN = 27

    GPIO.setup(BACKLIGHT_PIN, GPIO.OUT)
    GPIO.setup(PUSH_BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    
    GPIO.output(BACKLIGHT_PIN, 1)

    disp = LCD.PCD8544(DC, RST, spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE, max_speed_hz=4000000))
    font = ImageFont.load_default()
    disp.begin(contrast=60)
    disp.clear()
    disp.display()
    sensor = W1ThermSensor()
    db = firestore.client()
    temps_collection = db.collection(u"temps")
    sensor_document = db.collection(u"toggles").document(u"sensor")
    
    GPIO.output(BACKLIGHT_PIN, 0)

    state = {}
    state['isDisconnected'] = False
    state['font'] = font
    state['disp'] = disp
    state['current_temp'] = Temperature(0.0)
    state['update_display'] = False
    state['isPressed'] = False
    state['sensor'] = sensor
    state['temps_collection'] = temps_collection
    state['sensor_document'] = sensor_document

    def button_pressed(pin):
        if GPIO.input(PUSH_BUTTON_PIN):
            state['isPressed'] = True
            GPIO.output(BACKLIGHT_PIN, 1)
        else:
            state['isPressed'] = False
            disp.clear()
            disp.display()
            GPIO.output(BACKLIGHT_PIN, 0)

    #TODO:
    #def switch_flipped

    # Interrupts:
    GPIO.add_event_detect(PUSH_BUTTON_PIN, edge=GPIO.BOTH, callback=button_pressed, bouncetime=50)
    # TODO: GPIO.add_event_detect(SWITCH_PIN, edge=GPIO.BOTH, callback=switch_flipped, bouncetime=50)

    def run_threaded(job_function, state):
        job_thread = threading.Thread(target=job_function, kwargs=dict(state=state))
        job_thread.start()

    schedule.every(1).seconds.do(run_threaded, job_function=read_temp_process, state=state)
    schedule.every(0.01).seconds.do(run_threaded, job_function=update_display_process, state=state)

    while True:
        schedule.run_pending()
        sleep(0.01)

if __name__ == "__main__":
    main()
