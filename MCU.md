### MCU ###
Motion Detection:  These software routines are responsible for reading in accelerometer data from the X, Y, and Z axes as often as possible.  The data will be stored into arrays to store a recent history of each axis.  When any axis breaches a certain threshold (to be determined) and then once again lowers below the threshold, the software will jump into an analysis of the axis to find the maximum acceleration.  With that data, the characteristics of the motion can be determined and the rest of the program can decide how to react (audio, IR transmit, etc).
> This unit uses SPI to serially communicate with the accelerometer.  As such, only one axis may be read at a time.  Speeds of data acquisition with respect to the MCU’s clock will need to be tested in prototyping along with the diagnostic methods.
The accelerometer is connected to four pins using [SPI](http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus).:<br />
    * SDO (Serial Data Out) to request a specific axis reading<br />
    * SDI   (Serial Data In) to receive the axis reading<br />
    * SCK   (Serial Clock) to provide a synchronous means to communicate serially<br />
    * CS  (Chip Select) which is necessary to notify the accelerometer of incoming data<br />
> Visual Control:  This block is a relatively simple group of methods to handle the Ready [LED](http://en.wikipedia.org/wiki/Light-emitting_diode)., Throw Strength LEDs, and 7-Segment display.  The ratio of the accelerometer data to the maximum axis reading will determine the number of throw strength LEDs to be lit up, and the program will make use of a timer so that the LEDs appear to light up one at a time.  The Ready LED code will make use of another timer to signify when the shuriken is ready to throw or reload.  The LED ‘on’ status will be related to either the reload or throw cool-down based on whether or not the reload switch is pushed.
The visual block uses the following pins:<br />
    * Ready [LED](http://en.wikipedia.org/wiki/Light-emitting_diode).:  1 simple output pin<br />
    * Throw Strength LEDs:  4 pins for 4 LEDs<br />
> 7-Segment display:  4 pins to transmit digital data (1 through 16) into a one-hot decoder that will light up the corresponding segments.  That is, transmitting 0011 should light up five segments making the number 3.

http://team-endeavor.googlecode.com/hg/Images/Software%20Flowchart.PNG

http://team-endeavor.googlecode.com/hg/Images/MCU%20Pin%20Layout.PNG

Click [here](http://team-endeavor.googlecode.com/hg/MCU/PIC%2018F4520%20Datasheet.pdf) for MCU datasheet.