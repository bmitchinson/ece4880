#!/usr/bin/env python

# for delaying each individual process ( so they take turns better )
from time import sleep


# Pins
import RPi.GPIO as GPIO

# Twilio Texting
# from TwilioCreds import TwilioCreds
# from twilio.rest import Client

# Process stuff? If we even need it.
from multiprocessing import Manager, Process
import schedule
import threading

INPUT_PIN = 27

def main():
    
    sleep(.1)

    # Pins
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(SWITCH_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)

    while(True):
        sleep(.33)
        print(GPIO.input(SWITCH_PIN))


if __name__ = "__main__":
    main()