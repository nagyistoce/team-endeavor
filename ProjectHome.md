



# Project 2: The Shuriken #

# Team Endeavor #
**Introduction:** The goal and objective is to provide a central location for documentation and any pertinent information to be stored on this site for all Team members for Project 2: The Shuriken. [Project Specifications](http://ecen4013.okstate.edu/docs/project2/Shuriken.pdf).

#### Team Members ####

> Ben Lozano- Project Manager<br />     James Bellamy- Lead Technical Engineer<br />     Caitlin Guthrie- Design Engineer<br />     Aaron Humphries- Design Engineer<br />      Connor Ledgerwood- Technical Engineer<br />      Adam Zucker- Technical Engineer

**Team Contract:**
> //Link the Team Contract here//

---

# Block Diagram #

http://team-endeavor.googlecode.com/hg/Images/Block%20Diagram.PNG

# Gantt Chart #
http://team-endeavor.googlecode.com/hg/Images/Gantt%20Chart.PNG


---

## Functional Blocks ##

### Case ###
This will serve as the basis for the shuriken case design. Overall it is a flat case which will be easy to design and fabricate as well as provide a flat surface to have visible external devices such as the indicator LEDs and 7-segement display. This case design can easily be “thickened” to allow for multiple [PCB](http://en.wikipedia.org/wiki/Printed_circuit_board). layers to be placed inside without adding to much volume overall. This design will roughly be 6 inches square and just over 7 inches tip to tip across. It will measure around a ½ inch in height but can be adjusted to allow a multiple PCB layer setup however it can be no more than an inch.

#### Multisim and Ultiboard ####


---

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

---

### Power Pack ###

The power pack will consist of 6 C Cell batteries. Each battery is rated for 1.5Volts so the pack will provide a maximum of 9 Volts. The batteries take approximately 6hrs to discharge to a rating of 1 volt a piece, overall voltage of 6volts. Each battery is approximately 1inch wide and 2 inches tall, so the power pack will only be 6”x1”x2” (width x height x depth).
> The battery output voltage will be input into 3 Low Drop Out (LDO) voltage regulators which will be housed in the arm power pack unit as well. The reason LDO’s are being used is because they only require an input to output voltage difference of .9 volts as opposed to the standard linear regulators requiring a difference of 2 volts. The 3 LDO’s that are being used are two 5V LDO’s, and one 3.3V LDO. The reason we are using two 5V LDO’s is because each of the LDO’s can only provide a maximum current of 250mA. The MCU can draw 250mA (max) alone, so one LDO will be used to solely power the [MCU](http://en.wikipedia.org/wiki/MCU).. The remaining LDO’s will be used to provide power to the remaining parts.
> The power from the arm unit will be sent to the main shuriken via 20AWG wire. This wire is sturdy and rated for enough voltage to power the shuriken.



---

### IR Transmit/Receive ###

> The Shuriken will have at least one IR transmitter (LED), located in the front tip of the case (farthest from person) and two IR receivers located in the two tips perpendicular to the transmitter. The IR transmitter will be limited from 5 feet to 25 feet; this limitation will be determined by the throw strength.
> The [IR](http://en.wikipedia.org/wiki/Infrared). Handler (code inside the PIC) will take in a signal from Motion Handling; this signal will be interpreted by the IR Handler to determine the distance the IR packet will be transmitted. The IR Handler will then encode a binary packet and send it off to be transmitted through the IR [LED](http://en.wikipedia.org/wiki/Light-emitting_diode).. The packet will be a PWM (Pulse Width Modulation) signal at 56 kHz and consist of 1’s and 0’s.
To limit the physical distance of the packet, a PIC > a 2:4 demultiplexer > MOSFET > resistor > [LED](http://en.wikipedia.org/wiki/Light-emitting_diode). combo will be implemented. The PIC will output two selection bits and a PWM Vdd which will input into the demultiplexer, the demux will then select between 4 outputs, the corresponding output from the demux will then input into a MOSFET to amplify the current, the output from the [MOSFET](http://en.wikipedia.org/wiki/MOSFET). will then input to a certain resistor value limiting the current to the IR LED. By limiting the current to the LED effectively lowers the radiant intensity of the LED thus limiting the range of the transmission.
> The [IR](http://en.wikipedia.org/wiki/Infrared). receiver is an active LOW [IR](http://en.wikipedia.org/wiki/Infrared). detector; this means when it detects an IR packet the detector outputs a low signal to the PIC. The packet is then interpreted by the IR Handler; it is read in bit by bit (24 bits total) and checked to ensure a proper packet has been received. If the packet is correct it is transmitted to the HIU through the communication channel. The detector already filters out most ambient noise; this ensures that only a 56 kHz signal triggers the detector.



---

### Communication ###

> The Shuriken device will communicate from the PIC to [XBee](http://en.wikipedia.org/wiki/XBee).
> Pro radio then to HIU then to the server. The [XBee](http://en.wikipedia.org/wiki/XBee). module can be connected directly to the UART port on the PIC which will allow it through pins RX (receive) or TX (transmit) once given proper Destination ID from server unless we broadcast data packets to the entire PAN. The [XBee](http://en.wikipedia.org/wiki/XBee). module will incorporate the [IEEE 802.15.4](http://en.wikipedia.org/wiki/IEEE_802.15.4) is a point-point/point-multipoint communications protocol (similar to Bluetooth) designed for low-power devices will be used in the communication of the Shuriken.
> [XBee](http://en.wikipedia.org/wiki/XBee). specific identifiers:<br />
    * Source ID- The Source ID is the ID number of your particular radio<br />
    * Destination ID- The Destination ID is the ID of the radio that you want to send to.<br />
    * PAN (Personal Area Network) ID is the ID of the network. Your radio will only send to radios with the same PAN ID unless you set your own ID to 0xFFFF, which will make you broadcast across all networks on the same channel.<br />
    * Channel - This is the radio channel of your [XBee](http://en.wikipedia.org/wiki/XBee). radio. Radios must be on the same channel in order to communicate. You can reduce interference between different [XBee](http://en.wikipedia.org/wiki/XBee). networks by using a different channel.<br />
[XBee](http://en.wikipedia.org/wiki/XBee). has two source ID's:<br />
    * A unique 64-bit serial number that is set at the factory and cannot be changed.<br />
    * A 16-bit ID that you can change<br />
[XBee](http://en.wikipedia.org/wiki/XBee). Setup: Using [X-CTU](http://www.digi.com/support/kbase/kbaseresultdetl.jsp?kb=125). software  for the [XBee](http://en.wikipedia.org/wiki/XBee). will allow to select settings and test for range. Programming the module using AT commands will give the [XBee](http://en.wikipedia.org/wiki/XBee). radio a variety of functions to perform.  All parameter are in hexadecimal format.

**For more information on the XBee wireless radio please click [here](http://ftp1.digi.com/support/documentation/90000982_E.pdf) for datasheet.**


---

### Accelerometer (Motion Detection) ###

> The accelerometer block consists of the ADXL345 a digital tri-axial accelerometer that can read up to 16g’s of acceleration.  It is powered by 3.6 V, 140 mA max and has 3 connections to the PIC, Clock, Serial Data Input, Serial Data Output and Chip select. Chip Select is used in initiating SPI(the coding we are using) transactions, it goes low at the start of transmission and high at the end of transmission. The clock controls the timing in which data is sent and received, data is updated on the falling edge of the clock and is sampled on the rising edge. After data addressing occurs the each clock pulse the chip goes to the next register. To read non-sequential registers it has to bring CS to high and drop it back low and readdress the data to get the new register.  Through this process the ADXL will read off the sequential registers of –/+X, Y and Z and send the reading to the [PIC](http://en.wikipedia.org/wiki/PIC_microcontroller). as the chip is detecting acceleration. The [PIC](http://en.wikipedia.org/wiki/PIC_microcontroller). will then take that information and use it to cause the Force Indication LED’s to light up and cause our IR’s to transmit if the throw was strong enough and a correct throw( acceleration in the x&y plane with minimal movement in the Z direction).

http://team-endeavor.googlecode.com/hg/Images/Motion%20Detection.PNG

For more information on the Motion Detection system click [here](http://code.google.com/p/team-endeavor/source/browse/Prototyping/Accereometer/Prototype%20Datasheet%20for%20Acceleration_CG.docx).


---

### Audio ###

> The audio block is capable of producing four different sounds, which are played back when told to do so by the [PIC](http://en.wikipedia.org/wiki/PIC_microcontroller)..  The input of the system consists of four digital inputs.  The only output from the system is the four different types of sound.  The sounds are recorded to the playback device before actual operation by flipping a switch that controls whether the device is recording or outputting audio.  To record you simply connect an audio cable and hold the corresponding button down for the duration of the sound you are recording.

http://team-endeavor.googlecode.com/hg/Images/Audio.PNG


---

### Visual ###

> The visual indication block has three parts: the Blade Counter, Blade Ready/Reload Available, and the Force Indicator. The Blade counter consists of a 7 Segment Decoder IC that takes the number of blades in 4 bits binary and separates it into 7 individual bits that turn on the 7 separate LEDs of the 7 segment display to display the binary number received from the PIC.  The Blade Ready/Reload Available consists of a [FET](http://en.wikipedia.org/wiki/Field-effect_transistor). Switch and an LED that lights up when a blade can be fired and when a reload is requested, it lights up when the reload is available, otherwise the [LED](http://en.wikipedia.org/wiki/Light-emitting_diode). turns off. The [FET](http://en.wikipedia.org/wiki/Field-effect_transistor). is attached to the power source and lets the power flow when it receives a High input signal from the [PIC](http://en.wikipedia.org/wiki/PIC_microcontroller).. Force Indication consists of 4 FET Switches and 4 LEDs each of a different color each connected in series to four different pins of the PIC.  The PIC will turn on the FETs in correlation to the throw force read from the accelerometer and display a red [LED](http://en.wikipedia.org/wiki/Light-emitting_diode). for a bad or too low force throw, a red and an orange for a good but low strength throw, red, orange and yellow for a good and medium strength throw, and the three previous and a green [LED](http://en.wikipedia.org/wiki/Light-emitting_diode). for a good and full strength throw.


http://team-endeavor.googlecode.com/hg/Images/Visual.PNG

Click [here](http://code.google.com/p/team-endeavor/source/browse/Audio-Visual/Visual%20Control%20Block.docx) to better understand the Audio Block.


---

### Reload Unit ###

> The reload unit consists of a surface mount toggle switch which will be mounted in the shuriken. The switch will be normally open so that it only consumes power when engaged. During normal operation, a low signal (0V) will be sent to the MCU. When the switch is activated, a high signal (5V) will be sent to the MCU indicating that the reload actions are ready to be performed.
> The body of the toggle switch is only 4mm high so it should not be a major factor in determining the thickness of the Shuriken.


---

# Shuriken: Case & PCB #

//Insert picture of Case and PCB//


---

# Final Report & Video #

  * Insert PDF file of Final Report here

#### [YouTube](http://www.youtube.com) Video ####


---

# Sponsors #
> What a success! On behalf of Team Endeavor engineers, I want to extend a heart-felt thank you for your commitment to making this Senior Design so successful. It was the vision of this Team with research and dedication and your contribution made it a reality.

> Thank you again from all the engineers from Team Endeavor.

#### Stratasys ####
> Thank you to Jesse Roitenberg from [Stratasys](http://www.stratasys.com/Service.aspx) for his help and desire to make our vision a reality. They offer an extensive array of solutions, products and services to suit the needs of indivdual prototyping projects. Contact [Stratasys](http://www.stratasys.com/Service.aspx) and they will be more than happy to make your project come to reality from design to frabrication.

#### Sponsor 2 ####

#### Sponsor 3 ####

#### Sponsor 4 ####

