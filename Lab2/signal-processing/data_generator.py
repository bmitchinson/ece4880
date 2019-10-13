#!/usr/bin/env python
# Alexander Powers (alexander-powers@uiowa.edu)

import numpy as np
import time

def generate_sin(magnitude=10, noise=True):
    sin = np.sin(time.time())
    if noise:
        sin += np.random.rand()
    return magnitude * sin

def generate_cos(magnitude=10, noise=True):
    cos = np.cos(time.time())
    if noise:
        cos += np.random.rand()
    return magnitude * cos
