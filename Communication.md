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