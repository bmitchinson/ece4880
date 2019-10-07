#!/usr/bin/env python
# Alexander Powers (alexander-powers@uiowa.edu)

import time
from w1thermsensor import W1ThermSensor
sensor = W1ThermSensor()

while True:
  try:
    temp = sensor.get_temperature()
    print("the temp is {}".format(temp))
  except Exception as e:
    print(e)
    print('Sensor is unpluged')
  time.sleep(1)
