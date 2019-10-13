#!/usr/bin/env python
# Alexander Powers (alexander-powers@uiowa.edu)

# https://learn.sparkfun.com/tutorials/graph-sensor-data-with-python-and-matplotlib/update-a-graph-in-real-time\
# https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.lfilter.html#scipy.signal.lfilter
import datetime as dt
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from data_generator import generate_sin, generate_cos
from scipy.integrate import simps
from scipy import signal


# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
ys = []

n_elements = 200
# This function is called periodically from FuncAnimation
def animate(i, xs, ys):
    if(i==0):
        return

    # Read temperature (Celsius) from TMP102
    value = generate_cos(noise=True)
    print(i, value)

    # Add x and y to lists
    xs.append(i+1)
    ys.append(value)

    # Limit x and y lists to 20 items
    xs = xs[-n_elements:]
    ys = ys[-n_elements:]
    ax.clear()
    ax.plot(xs, ys, 'r.')

    if len(xs) > 5:
        b, a = signal.butter(3, 0.05)
        zi = signal.lfilter_zi(b, a)
        z, _ = signal.lfilter(b, a, ys, zi=zi*ys[0])
        z2, _ = signal.lfilter(b, a, z, zi=zi*z[0])
        y = signal.filtfilt(b, a, ys)
        ax.plot(xs, z, 'b-')
        ax.plot(xs, z2, 'g-')
        ax.plot(xs, y, 'm-')
        


    # Format plot
    plt.xticks(rotation=50, ha="right")
    plt.subplots_adjust(bottom=0.30)
    plt.title("Signal over time")
    plt.ylabel("Signal Intensity")


# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=100)
plt.show()
