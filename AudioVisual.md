### Audio ###

> The audio block is capable of producing four different sounds, which are played back when told to do so by the [PIC](http://en.wikipedia.org/wiki/PIC_microcontroller)..  The input of the system consists of four digital inputs.  The only output from the system is the four different types of sound.  The sounds are recorded to the playback device before actual operation by flipping a switch that controls whether the device is recording or outputting audio.  To record you simply connect an audio cable and hold the corresponding button down for the duration of the sound you are recording.

http://team-endeavor.googlecode.com/hg/Images/Audio.PNG


---

### Visual ###

> The visual indication block has three parts: the Blade Counter, Blade Ready/Reload Available, and the Force Indicator. The Blade counter consists of a 7 Segment Decoder IC that takes the number of blades in 4 bits binary and separates it into 7 individual bits that turn on the 7 separate LEDs of the 7 segment display to display the binary number received from the PIC.  The Blade Ready/Reload Available consists of a [FET](http://en.wikipedia.org/wiki/Field-effect_transistor). Switch and an LED that lights up when a blade can be fired and when a reload is requested, it lights up when the reload is available, otherwise the [LED](http://en.wikipedia.org/wiki/Light-emitting_diode). turns off. The [FET](http://en.wikipedia.org/wiki/Field-effect_transistor). is attached to the power source and lets the power flow when it receives a High input signal from the [PIC](http://en.wikipedia.org/wiki/PIC_microcontroller).. Force Indication consists of 4 FET Switches and 4 LEDs each of a different color each connected in series to four different pins of the PIC.  The PIC will turn on the FETs in correlation to the throw force read from the accelerometer and display a red [LED](http://en.wikipedia.org/wiki/Light-emitting_diode). for a bad or too low force throw, a red and an orange for a good but low strength throw, red, orange and yellow for a good and medium strength throw, and the three previous and a green [LED](http://en.wikipedia.org/wiki/Light-emitting_diode). for a good and full strength throw.


http://team-endeavor.googlecode.com/hg/Images/Visual.PNG

Click [here](http://code.google.com/p/team-endeavor/source/browse/Audio-Visual/Visual%20Control%20Block.docx) to better understand the Audio Block.