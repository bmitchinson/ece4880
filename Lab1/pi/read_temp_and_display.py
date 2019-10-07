#!/usr/bin/env python
# Alexander Powers (alexander-powers@uiowa.edu)

from random import random
from time import sleep
from Temperature import Temperature

import Adafruit_Nokia_LCD as LCD
import Adafruit_GPIO.SPI as SPI

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

from w1thermsensor import W1ThermSensor
sensor = W1ThermSensor()

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
print(LCD.LCDWIDTH, LCD.LCDHEIGHT)

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


print('Press Ctrl-C to quit.')
while True:
    try:
      new_temp = Temperature(sensor.get_temperature())
      write_temp_to_lcd(new_temp.temp)
      print("Saved: {0}".format(str(new_temp)))
    except Exception as e:
      write_unpluged_to_lcd()
      print('lcd unpluged')
    sleep(1)



