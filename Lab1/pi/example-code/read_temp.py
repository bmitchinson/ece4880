#!/usr/bin/env python
# Alexander Powers (alexander-powers@uiowa.edu)

import time
from w1thermsensor import W1ThermSensor
sensor = W1ThermSensor()

while True:
  temp = sensor.get_temperature()
  print("the temp is {}".format(temp))
  time.sleep(1)
