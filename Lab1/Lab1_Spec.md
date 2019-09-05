## Lab 1 Spec (Shorthand)

Purpose: You are to design a thermometer with a web interface. The “design” is a
set of documents that describe how to make this device. Accompanying this design
is a prototype that demonstrates that the design works.


The primary challenge of this project is to analyze the requirements carefully 
and create and implement a design that satisfies all of them.


This assignment should be completed in “rapid prototyping” fashion, with tangible 
results expected early on.

### Requirements

#### 1) General Description

- [ ] A computer used for UI, display, and control.

- [ ] A thermometer sensor, at the end of a 2.0 +-0.1 meter cable. This should be 
a nice mechanical construction, capable of bouncing around without breaking. 
The sensor should not be damaged when placed in ice water.

- [ ] A "third box" containing, at minimum, a display (e.g. a set of 7-segment LEDs 
or equivalent), a button, a battery, and a power switch. It is intended that the
box, together with the sensor, can act as a battery-operated thermometer. 
The temperature data is then available on the internet.

- [ ] A cellphone that can receive text messages.

#### 2) Requirements for the afore mentioned "third box"

- [ ] This "third box” should be enclosed in some way, and physically robust (can 
stand being dropped from the workbench to the floor), and can be turned upside 
down, with the circuit, connectors, and switches still working.

- [ ] All cable connections to the third box should have terminating connectors, 
securely mounted to the third box. These connectors should be the kind meant to 
be easily connected/disconnected by a casual user.

- [ ] When dropped to the floor with cables connected, the connectors or cables 
should not break (although it is OK if they become disconnected). 

- [ ] If the sensor has been unplugged and is then plugged in, the third box should 
begin normal operation without user intervention. 

<p align="center">
  <img src="https://i.imgur.com/DjwLPn4.png">
  </br>
  <b>Fig 1. General Physical description of the temperature measurement 
  system.</b>
</p>

**3)** The switch on the third box functions as an on/off switch. When the switch is 
“off”, the thermometer system cannot display temperatures and temperature data 
is not available from the internet. 

**4)** When the switch on the third box is “on”, the following features are available 
locally at the third box. When the button is pressed on the third box, a display
on the box shows the temperature of the thermometer sensor in degrees C. There 
are no requirements on the resolution or size of the display. However, it must 
have the following features: 

- [ ] The correct temperature should appear on the display when the button is 
pressed, with no noticeable delay. Delays are noticeable if they are longer than 
about 20 milliseconds.

- [ ] The display should be clearly readable under normal indoor lighting 
conditions and all temperatures within the normal range of operation of this 
device (as specified below) should be displayed correctly.

- [ ] The button is to be “momentary contact”: When pressed, the display is on, 
when not pressed, the display is off. The display should go dark when the button
is released with no noticeable delay. 

- [ ] If the temperature sensor is not plugged into the third box, or is not 
working in some way, the display should notify the user that there is an error 
condition. 

**5)** When the power switch on the third box is “on”, the following features are 
available from an internet connected computer when appropriate software is run 
on the computer:

- [ ] The real-time temperature, in degrees C or degrees F, (controlled by the 
computer user), is displayed prominently (in a large font) on the computer 
screen, and updated once a second.

- If the temperature sensor is unplugged from the third box, an “unplugged sensor” 
message should appear instead of the real time temperature.

- if the third box switch is off, a message “no data available” should appear 
instead of the real-time temperature. 

- [ ] By user action on the computer, the temperature display on the third box can be 
turned on or turned off (in other words, the computer can virtually “press the 
button” on the third box.) The button response me in this situation shall be 
less than 1 second. 

- [ ] When the computer is connected to the internet, and the switch on the third box 
is on, a graph of the past temperature readings from the third box can be 
displayed on the computer screen. The graph of the past 300 seconds of data 
should be available within 10 seconds of the start of the software on the 
computer. 

- The graph is the temperature in degrees C. The top of the graph corresponds to 50 
degrees C, and the bottom, 10 degrees C. The graph should always have these limits, 
and is always in degrees C, irrespective of the real-time temperature display format. 

- This graph scrolls horizontally, with the latest temperature at the right side of 
the screen, and past temperature values on the left side of the screen. Once a second,
a new temperature value is added to the graph on the right side, and the graph 
scrolls from right to left . (The look is similar to a “chart recorder”). Older 
temperature values scroll off the graph on the left. The chart should have x-axis 
labels as described in (iv), below. 

- The physical size of the graph should be scalable with the mouse. 

- The total time record displayed on the graph is 300 seconds. The horizontal graph 
should correspond to, and be labeled in, “seconds ago from the current time”. (this 
means the c marks should be in the range 300->0). 

- If there is data missing (perhaps the switch on the third box was off, or perhaps 
the temperature sensor is not plugged in), this should be obvious on the graph 
display. Missing data should be clearly discernable from data that is off-scale (too 
large or too small). 

- This also applies to the present time display of data.  If the third box is off or 
the temperature sensor is not plugged in, the graph should continue to scroll and the 
graph data should be shown as missing. When the error is corrected, the graphing 
and real time display of data should resume. 

**6)** If the computer is on and the third box is off, the graph and real time 
display of data should appear on the computer screen within 10 seconds of the third 
box being turned on. 

<p align="center">
  <img src="https://i.imgur.com/n2OgvBX.png">
  </br>
  <b>Fig 2. Possible look for the computer graph display. 
  New data appears at the right and the graph scrolls to the left.  
  Here, something went wrong 180 seconds ago, for 20 seconds or so.</b>
</p>

**7)** When the computer is on and the third box is on, a text message will be sent to a 
specified phone number whenever the real-time temperature exceeds a certain value or 
is lower than a certain value. The two text messages, the max temperature, the min 
temperature, and the phone number can all be altered with the computer user 
interface.

#### 8) Total Range of Operation

- [ ] The design range of the possible temperature displayed should be at least from 
minus 10 to +63 degrees Celsius. This does not have to be verified by testing, 
(simply because it is not feasible to verify this in this class) but should be 
addressed by the design. 

- [ ] When someone holds the temperature sensor in their hand, the heat from their 
fingers should make the temperature go up after a few seconds. Holding a soldering 
iron close to or briefly touching the sensor should do the same, even more quickly. 

- [ ] In the lab, at room temperature, the output of the thermometer should be 
approximately 22 degrees C, +-4 degrees C. 

- [ ] When placed in a water-ice mixture, the output of the thermometer should be 0 
degrees C, +- 2 degrees C
