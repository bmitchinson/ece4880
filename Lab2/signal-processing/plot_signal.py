#!/usr/bin/env python
# Alexander Powers (alexander-powers@uiowa.edu)

# https://learn.sparkfun.com/tutorials/graph-sensor-data-with-python-and-matplotlib/update-a-graph-in-real-time
import datetime as dt
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from data_generator import generate_sin, generate_cos


# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
ys = []

n_elements = 50
# This function is called periodically from FuncAnimation
def animate(i, xs, ys):

    # Read temperature (Celsius) from TMP102
    value = generate_cos(noise=True)
    print(i, value)

    # Add x and y to lists
    xs.append(dt.datetime.now().strftime("%H:%M:%S.%f"))
    ys.append(value)

    # Limit x and y lists to 20 items
    xs = xs[-n_elements:]
    ys = ys[-n_elements:]

    # Draw x and y lists
    ax.clear()
    ax.plot(xs, ys, 'r.')
    ax.plot(xs, ys, 'b-')

    # Format plot
    plt.xticks(rotation=50, ha="right")
    plt.subplots_adjust(bottom=0.30)
    plt.title("Signal over time")
    plt.ylabel("Signal Intensity")


# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=100)
plt.show()
