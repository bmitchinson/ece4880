import RPi.GPIO as GPIO
import time


GPIO.setmode(GPIO.BCM)

SWITCH_PIN = 27

GPIO.setup(SWITCH_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

def handle_change(pin):
  print("Rising Edge ", pin )

GPIO.add_event_detect(SWITCH_PIN, GPIO.BOTH, callback=handle_change)

while(True):
  print(GPIO.input(SWITCH_PIN))
  time.sleep(1)
