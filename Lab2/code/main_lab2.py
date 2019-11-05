#!/usr/bin/env python

# for delaying each individual process ( so they take turns better )
from datetime import datetime
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

    print('Waiting for texts')
    while(True):
        sleep(.3)
        if "sendtext" in str(ser.readline()):
            sendText()

def sendText():
    print('sending text!')
    client = Client(TwilioCreds().sid(), TwilioCreds().auth_token())
    hours = datetime.now().hour
    minutes = datetime.now().minute
    if minutes < 10:
        minutes = 0 + str(minutes) 

    msg = f"Critical saftey event at {hours}:{minutes}"

    try:
        message = client.messages.create(
            body=msg,
            from_='+12054984327',
            to='+16307404172'
            )
        print(message.sid)

    except Exception as e:
        print('error:')
    

if __name__ == "__main__":
    main()