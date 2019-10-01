import RPi.GPIO as GPIO
import time


GPIO.setmode(GPIO.BCM)

PUSH_BUTTON = 17

GPIO.setup(PUSH_BUTTON, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

def push_button_change(pin):
  print("Rising Edge ", pin )

GPIO.add_event_detect(PUSH_BUTTON, GPIO.BOTH, callback=push_button_change)

while(True):
  time.sleep(1)
