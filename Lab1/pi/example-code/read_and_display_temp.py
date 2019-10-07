#!/usr/bin/env python
# Alexander Powers (alexander-powers@uiowa.edu)

import time

# Pins
import RPi.GPIO as GPIO

# LCD Libraries
import Adafruit_Nokia_LCD as LCD
import Adafruit_GPIO.SPI as SPI

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

# Temp. Sensor Libraries
from w1thermsensor import W1ThermSensor
sensor = W1ThermSensor()

# Raspberry Pi hardware SPI config:
DC = 23
RST = 24
SPI_PORT = 0
SPI_DEVICE = 0

GPIO.setmode(GPIO.BCM)
DISPLAY_POWER_PIN = 26
GPIO.setup(DISPLAY_POWER_PIN, GPIO.OUT)
GPIO.output(DISPLAY_POWER_PIN, 1)

# Hardware SPI usage:
disp = LCD.PCD8544(DC, RST, spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE, max_speed_hz=4000000))


# Initialize library.
disp.begin(contrast=60)

# Clear display.
disp.clear()
disp.display()

# Load default font.
font = ImageFont.load_default()


# Create blank image for drawing.
# Make sure to create image with mode '1' for 1-bit color.
image = Image.new('1', (LCD.LCDWIDTH, LCD.LCDHEIGHT))

# Get drawing object to draw on image.
draw = ImageDraw.Draw(image)

# Draw a white filled box to clear the image.
draw.rectangle((0,0,LCD.LCDWIDTH,LCD.LCDHEIGHT), outline=255, fill=255)

while True:
  temp = sensor.get_temperature()
  print("the temp is {}".format(temp))
  # draw.text((horizontal_px, vertical_px), string, font)
  # Draw a white filled box to clear the image. #TODO, we could just clear the pixels with the changes
  draw.rectangle((0,0,LCD.LCDWIDTH,LCD.LCDHEIGHT), outline=255, fill=255)
  draw.text((8,10), "the temp is \n{}".format(temp), font=font)
  disp.image(image)
  disp.display()
  time.sleep(1)
