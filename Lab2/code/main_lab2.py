#!/usr/bin/env python

# for delaying each individual process ( so they take turns better )
from time import sleep
import serial

# Twilio Texting
from TwilioCreds import TwilioCreds
from twilio.rest import Client

def main():
    ser = serial.Serial()
    ser.baudrate = 9600
    ser.port = 'COM7'
    ser.open()
    sleep(3) # required when opening port

    print('Waiting for texts from...')
    while(True):
        sleep(.3)
        if "sendtext" in str(ser.readline()):
            sendText()

def sendText():
    print('send text')

if __name__ == "__main__":
    main()