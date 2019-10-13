#!/usr/bin/env python
# Alexander Powers (alexander-powers@uiowa.edu)

import numpy as np
import time

def generate_sin(magnitude=10, noise=True, offset=0):
    sin = np.sin(time.time())
    if noise:
        sin += np.random.rand()
    sin *= magnitude
    sin += offset
    return sin

def generate_cos(magnitude=10, noise=True, offset=0):
    cos = np.cos(time.time())
    if noise:
        cos += np.random.rand()
    cos *= magnitude
    cos += offset
    return cos

def generate_jumpy_sin(magnitude=10, noise=True, time_count=0, jump_every=50, jump_offset=20):
    time_count = time_count // jump_every
    if (time_count %  2 == 0):
        offset = jump_offset
    else:
        offset = 0
        
    sin = np.sin(time.time())
    if noise:
        sin += np.random.rand()
    sin *= magnitude
    sin += offset
    return sin

def generate_jumpy_cos(magnitude=10, noise=True, time_count=0, jump_every=50, jump_offset=20):
    time_count = time_count // jump_every
    if (time_count %  2 == 0):
        offset = jump_offset
    else:
        offset = 0

    cos = np.cos(time.time())
    if noise:
        cos += np.random.rand()
    cos *= magnitude
    cos += offset
    return cos
