#include "p18F4520.h"
#include "shuriken.h"
#include "adxl345.h"
#include <spi.h>
#include <delays.h>
#include <pwm.h>
#include <timers.h>

/****************************************
 *   
 *                Global
 *               Variables
 *    
 ****************************************/


// Timer overflow variables
char t1_ovf_att = 0;     //For attack cooldown
char t1_ovf_rld = 0;     //For reload cooldown
char t3_ovf = 0;         //For strength gauge tracking
// Attack variables
char action = 1;         //records the state of reload switch upon motion
char att_rdy = 1;
char rld_rdy = 1;
char current_ammo = MAX_AMMO;
char att_strength = 0;
// Visual variables
char gauge_index = 0;
char toggle_enable = 1;    // Set to 0 for real functionality
char LED_toggle = 0;
// Accelerometer variables
char X0_buff;
char X1_buff;
char Y0_buff;
char Y1_buff;
char Z0_buff;
char Z1_buff;
int X_axis[NUM_AXIS_DATA];
int Y_axis[NUM_AXIS_DATA];
int Z_axis[NUM_AXIS_DATA];
// Serial comm. variables
char didReceive_uart = 0;
char uart_rx_data = 0;
char spi_rx_data = 0;
// IR variables
char isReceiving = 0;			// Receive Trigger (RB2)
char expected_PB2 = 1;			// Previous state of PB2
char IR_data_ready = 0;			// IR packet has been read in and ready to be transmitted to Xbee
char low_ovf = 0;				// Number of low overflows
char high_ovf = 0;				// Number of high overflows
char lowTRUE = 0;				// S-o-D ON signal boolean
char highTRUE = 0;				// S-o-D OFF signal boolean
char IR_Data[24];				// Array for storing packet data
char IR_Packet_rx = 0;
char IR_Packet_tx[3];

/****************************************
 *   
 *                 Main
 *               Routine
 *   
 ****************************************/

void main() {
   init_all();

   //test_IR_tx();
   //test_strength_gauge();
   test_7S();
   //test_cooldown();
   //test_SPI();
   //test_UART();
   //test_accelerometer();

   //-->  Main program loop
   while(1) {
      if( (RELOAD==1) && (rld_rdy==1) ) {
         READY = 1;
         action = 0;
         monitor_interrupt();
      }
      else if( (RELOAD==1) && (rld_rdy==0) )
         READY = 0;
      else if( (RELOAD==0) && (att_rdy==1) ) {
         READY = 1;
         action = 1;
         monitor_interrupt();
      }
      else if( (RELOAD==0) && (att_rdy==0) )
         READY = 0;
   }
}

/****************************************
 *   
 *         Accelerometer Diagnostic
 *                Routines
 *   
 ****************************************/

/*
 *  Polls the accelerometer's interrupt pin.  If it detects a signal,
 *  it reads the axes involved in the motion.  It then results in a 
 *  bad_motion() or reads more data  from grab_axis()
*/
void monitor_interrupt() {
   char X_str;
   char Y_str;
   char Z_str;
   int  X_mag;
   int  Y_mag;
   int  Z_mag;
   char single_tap;
   char double_tap;
   char reading[16] = "Reading fifo..."; 
   char header[17] = "Axis  Mag  Str";
   char line[17]   = "--------------";
   // Monitor a single or double tap flag
   spi_rx_data = acc_read(INT_SOURCE);
   single_tap = (spi_rx_data & 0x40);
   double_tap = (spi_rx_data & 0x20);
   // Act on a tap event
//***   if(spi_rx_data == 0xC1 || spi_rx_data == 0xE1) {
//***   if(single_tap==0x40 || double_tap==0x20) {
//***   if(double_tap==0x20) {
   if(single_tap==0x40) {
      UART_tx_str(reading);
      UART_tx('\r');
      read_fifo();
      X_mag = find_maximum(X_axis);
      Y_mag = find_maximum(Y_axis);
      Z_mag = find_maximum(Z_axis);
      X_str = calculate_strength(X_mag);
      Y_str = calculate_strength(Y_mag);
      Z_str = calculate_strength(Z_mag);
      print_axis(X_axis);
      UART_tx('\r');
      print_axis(Y_axis);
      UART_tx('\r');
      print_axis(Z_axis);
      UART_tx('\r');
      //-->  Attack Handler
      if (action==1) {
         //Print data to UART
         UART_tx_str(line);
         UART_tx('\r');
         UART_tx_str(header);
         UART_tx('\r');
         UART_tx_str(line);
         UART_tx('\r');
         UART_tx('X');
         UART_tx(' ');
         UART_tx(' ');
         UART_tx(' ');
         UART_tx(' ');
         display_decimal(X_mag);
         UART_tx(' ');
         UART_tx(' ');
         UART_tx(' ');
         UART_tx(' ');
         display_decimal(X_str);
         UART_tx('\r');
         UART_tx('Y');
         UART_tx(' ');
         UART_tx(' ');
         UART_tx(' ');
         UART_tx(' ');
         display_decimal(Y_mag);
         UART_tx(' ');
         UART_tx(' ');
         UART_tx(' ');
         UART_tx(' ');
         display_decimal(Y_str);
         UART_tx('\r');
         UART_tx('Z');
         UART_tx(' ');
         UART_tx(' ');
         UART_tx(' ');
         UART_tx(' ');
         display_decimal(Z_mag);
         UART_tx(' ');
         UART_tx(' ');
         UART_tx(' ');
         UART_tx(' ');
         display_decimal(Z_str);
         UART_tx('\r');
         UART_tx('\r');
         //Analyze threshold breaches
         if( (Z_str<2) && ((X_str!=0)||(Y_str!=0)) ) {
            if( X_str >= Y_str)
               att_strength = X_str;
            else
               att_strength = Y_str;
               update_ammo(-1);
               set_IR_strength();        // Begin IR transmit packet
               CLR_AUD_3;
               delay_ms(10);
               SET_AUD_A;
         }
         else
            //**bad_motion();
         att_rdy = 0;
         gauge_index = 0;
      }
      //-->  Reload Handler
      else if (action==0) {
         if( Z_str!=0 ) {
            CLR_AUD_4;
            delay_ms(10);
            SET_AUD_A;
            update_ammo(1);
            
         }
         rld_rdy = 0;
      }
   }
}

/*
 *  Called when the threshold strengths do not meet
 *  the defined specifications for a motion
*/
void bad_motion() {
   action = 0;
   att_strength = 0;
   gauge_index = 0;
   update_ammo(-1);
   SET_AUD_E;
   CLR_AUD_2;
   delay_ms(10);
   SET_AUD_E;
}

/*
 *  Analyzes data retrieved from axis to find the sample of
 *  largest magnitude.  Sets a throw strength based on that ratio
*/
int find_maximum(int * axis) {
   int i;
   int min = 0;
   int max = 0;
   float ratio = 0;
   // Find the largest magnitude
   for( i=0 ; i<NUM_AXIS_DATA ; i++ ) {
      if( axis[i] < min )
         min = axis[i];
      if( axis[i] > max )
         max = axis[i];
   }
   if ( max<0 )
      max = ~max + 1;
   if ( min<0 )
      min = ~min + 1; 
   // Return the larger of magnitudes
   if( max >= min )
      return max;
   return min;
}

/*
 *  Takes a 10-bit 2's complement int value and
 *  computes a ratio of the maximum (511) to ultimately
 *  find an acceleration strength [1-4]
*/
char calculate_strength(int point) {
   float ratio;
   char strength;
   // Convert to positive
   if (point < 0)
      point = ~point + 1;
   // Compute ratio
   ratio = ((float)(point))/MAX_RATIO;
   // Compute axis strength
   if( ratio < .30 )
      strength = 0;
   else if( ratio < .45 )
      strength = 1;
   else if( ratio < .60 )
      strength = 2;
   else if( ratio < .75 )
      strength = 3;
   else
      strength = 4;
   return strength;
}

/*
 *  Takes the upper and lower bit of an axis sample and
 *  condenses it into an integer
*/
int convert_to_int(int high, int low) {
   int temp;
   int result;
   temp = (high << 8) & 0xFF00;
   result = temp + (0x00FF & low);
   return result;
}

/*
 * Polls the interrupt pin.  If it is high, reads the
 * data of all 6 axis data registers and displays them
 * via UART.
*/
void test_accelerometer() {
   char i;
   char int_status;
   char int_ack[20] = "Detected interrupt.";
   char source[17] = "Source register:";
   char status[17] = "Status register:";
   while(1) {
       if(PORTDbits.RD0 == 1) {
         // Collect interrupt data
         acc_read(INT_SOURCE);
         UART_tx_str(source);
         UART_tx('\r');
         UART_tx(spi_rx_data);
         UART_tx('\r');
         acc_read(ACT_TAP_STATUS);
         UART_tx_str(status);
         UART_tx('\r');
         UART_tx(spi_rx_data);
         UART_tx('\r');
         UART_tx('\r');
         // Collect data
         UART_tx_str(int_ack);
         UART_tx('\r');
         int_status = acc_read(ACT_TAP_STATUS);
         X0_buff = acc_read(DATAX1);
         X1_buff = acc_read(DATAX0);
         Y0_buff = acc_read(DATAY1);
         Y1_buff = acc_read(DATAY0);
         Z0_buff = acc_read(DATAZ1);
         Z1_buff = acc_read(DATAZ0);
         // Display
         display_readings();
         UART_tx('\r');
         // Delay
         for( i=0 ; i<10 ; i++ )
            delay_ms(100);
      }
   }
}

/*
 *  Calculates a throw strength based on the values
 *  stored in the data buffer variables.  Prints to UART
*/
void display_readings() {
   char X_mag[13] = "X magnitude:";
   char Y_mag[13] = "Y magnitude:";
   char Z_mag[13] = "Z magnitude:";
   char X_rat[10] = "X ratio:";
   char Y_rat[10] = "Y ratio:";
   char Z_rat[10] = "Z ratio:";
   int X_reading;
   int Y_reading;
   int Z_reading;
   char X_str;
   char Y_str;
   char Z_str;
   // Condense data
   X_reading = convert_to_int( X1_buff, X0_buff );
   Y_reading = convert_to_int( Y1_buff, Y0_buff );
   Z_reading = convert_to_int( Z1_buff, Z0_buff );
   // Calculate strength
   X_str = calculate_strength(X_reading);
   Y_str = calculate_strength(Y_reading);
   Z_str = calculate_strength(Z_reading);
   // Display magnitudes
   UART_tx_str(X_mag);
   UART_tx('\r');
   display_decimal(X_reading);
   UART_tx('\r');
   UART_tx_str(Y_mag);
   UART_tx('\r');
   display_decimal(Y_reading);
   UART_tx('\r');
   UART_tx_str(Z_mag);
   UART_tx('\r');
   display_decimal(Z_reading);
   UART_tx('\r');
   // Display ratios
   UART_tx_str(X_rat);
   UART_tx('\r');
   display_decimal(X_str);
   UART_tx('\r');
   UART_tx_str(Y_rat);
   UART_tx('\r');
   display_decimal(Y_str);
   UART_tx('\r');
   UART_tx_str(Z_rat);
   UART_tx('\r');
   display_decimal(Z_str);
   UART_tx('\r');
}

void print_axis(int * axis) {
   int i;
   for( i=0 ; i<32 ; i++ )
      display_decimal(axis[i]);
}

/****************************************
 *   
 *                  SPI
 *                Routines
 *   
 ****************************************/

/* 
 * Sends two packets of data to the accelerometer to
 * write to a configuration register.
 * 1.  reg address
 * 2.  reg data
 */
void acc_config(char reg, char input) {
   CHIP_SEL = 0;
   WriteSPI(reg);
   Delay10TCYx(10);
   WriteSPI(input);
   Delay10TCYx(10);
   CHIP_SEL = 1;
}

/* 
 * Sends a register address to the accelerometer and
 * subsequently receives its contents
*/
char acc_read(char reg) {
   CHIP_SEL = 0;
   reg |= 0x80;
   WriteSPI(reg);
   Delay10TCYx(10);
   spi_rx_data = ReadSPI();
   Delay10TCYx(10);
   CHIP_SEL = 1;
   return spi_rx_data;
}

/*
 *  Conducts a multiple-byte read of every axis
 *  
*/
void read_fifo() {
   char i;
   int temp;
   for( i=0 ; i<NUM_AXIS_DATA ; i++ ) {
      // Grab each axis byte
      CHIP_SEL = 0;
      WriteSPI(DATAX0|0xC0);
      X0_buff = ReadSPI();
      Delay10TCYx(10);
      X1_buff = ReadSPI();
      Delay10TCYx(10);
      Y0_buff = ReadSPI();
      Delay10TCYx(10);
      Y1_buff = ReadSPI();
      Delay10TCYx(10);
      Z0_buff = ReadSPI();
      Delay10TCYx(10);
      Z1_buff = ReadSPI();
      Delay10TCYx(10);
      CHIP_SEL = 1;
      // Store into axis array
      X_axis[i] = convert_to_int(X1_buff, X0_buff);
      Y_axis[i] = convert_to_int(Y1_buff, Y0_buff);
      temp = convert_to_int(Z1_buff, Z0_buff);
      Z_axis[i] = temp - GRAVITY;
   }
}

/****************************************
 *   
 *                  UART
 *                Routines
 *   
 ****************************************/

/*
 *  Sends a byte through UART
*/
void UART_tx(char dat) {
   while (PIR1bits.TXIF == 0) ;
   TXREG = dat;
}

/*
 *  Transmits a string through UART
*/
void UART_tx_str(char message[50]) {
   int index = 0;
   while( message[index] != '\0' ) {
      UART_tx( message[index] );
      index++;
   }
   UART_tx(' ');
   UART_tx(' ');
}

/*
 *  ISR:  Receives a byte through UART
*/
void UART_rx() {
   uart_rx_data = RCREG;
   didReceive_uart = 1;
}


/*
 *  Can be configured to test Tx or Rx with UART.
 *  Tx transmits "Hello World" and Rx parrots a character back
*/
void test_UART() {
   char message[12] = "Hello World";
   char method = 0;  // 0=Tx , 1=Rx
   int i;
   while(1) {
      if(method == 1 && didReceive_uart == 1) {
         for ( i=0 ; i<5 ; i++)
            UART_tx(uart_rx_data);
         UART_tx('\r');
         didReceive_uart = 0;
      }
      else if (method == 0) { 
         for( i=0 ; i<11 ; i++ )
            UART_tx( message[i] );
         UART_tx('\r');
      }
   }
}

/****************************************
 *   
 *              IR  Receive
 *               Routines
 *   
 ****************************************/

void start_of_data()
{
	read_data(IR_RX);						// Read status of PB2, if 1 IR Rx is receving
	
	if(IR_RX == 1 && expected_PB2 == 0)		// Transistion from low to high
	{		
		expected_PB2 = 1;					// PB2 is now high
		if(low_ovf > NUM_OVF_START)			// Checks if the # of low overflows is equal to "OFF" signal for S-o-D
		{
			lowTRUE = 1;					// S-o-D has been partially received
			low_ovf = 0;					// Resets Counter
			dataDecode();					
		}
		else
		{
			//noise?
		}
	}
	else if(IR_RX == 0 && expected_PB2 == 1)	// Transistion from high to low
	{
		expected_PB2 = 0;					// PB2 is now low
		if(high_ovf > NUM_OVF_START)		// Checks if the # of high overflows is equal to the "ON" time for S-o-D 
		{
			highTRUE = 1;					// S-o-d has been partially received
			high_ovf = 0;					// Resets counter
			dataDecode();
		}
	}	
	else
	{
		//noise?
	}	

}

void dataDecode()
{
	int i;
	
	if(lowTRUE == 1 && highTRUE == 1)			// Start-of-Data has occured, Read in data bits, LSB first
	{
		for(i = 0; i < 24; i++)
		{	
			read_data(IR_RX);
			
			if(IR_RX == 1 && expected_PB2 == 0)	// Transistion from low to high
			{
				expected_PB2 = 1;
				if(low_ovf > NUM_OVF_ZERO && high_ovf > NUM_OVF_ON)				// Logic Zero 
				{
					IR_Data[i] = 0;
					low_ovf = 0;
					high_ovf = 0;
				}
				else 
				{
					// noise?
				}
			}
			else if(IR_RX == 0 && expected_PB2 == 1)	// Transistion from high to low
			{
				expected_PB2 = 0;
				if(high_ovf > NUM_OVF_ONE && high_ovf > NUM_OVF_ON)				// Logic One 
				{
					IR_Data[i] = 1;
					lowTRUE = 0;
					highTRUE = 0;
				}
				else
				{
					// noise?
				}
			}
		}
		end_of_data();
	}
}

void read_data(char PB2)
{
	if(IR_RX == 0 && expected_PB2 == 0)		// Low Input
	{
		low_ovf++;							// Increment number of low overflows 
	}
	else if(IR_RX == 1 && expected_PB2 == 1)	// High Input
	{
		high_ovf++;							// Increment number of high overflows
	}
}

void end_of_data()							// Checks to make sure packet has End of Data signal
{
	read_data(IR_RX );
	
	if(IR_RX == 1 && expected_PB2 == 0)		// Transistion from low to high
	{		
		expected_PB2 = 1;					// PB2 is now high
		if(high_ovf > NUM_OVF_ON)			// Checks for End of Data Signal
		{	
			array_shift();
			IR_data_ready = 1;				// Packet ready to be transmitted to Xbee
			high_ovf = 0;					// Clears high counter
			low_ovf = 0;					// Clears low counter
			isReceiving = 0;				// Clears receving flag
			T0CONbits.TMR0ON = 0;			// Disables Timer0
			INTCON3bits.INT2IE = 1;   		// Reenables external interrupt on RB2
		}
		else
		{
			// noise?
		}
	}
	else
	{
		// noise?
	}
}

void array_shift()
{	
	int n;
	for(n = 0; n < 12; n++)
	{
		IR_Packet_rx = (IR_Data[n] << n);
	}
}

/****************************************
 *   
 *             IR Transmit
 *               Routines
 *   
 ****************************************/
 

void set_IR_strength()
{	
	//UART_tx(1);
	// Take in throw strength from Accel. 
	// Turn on selctor bits
	if(att_strength == 1) {					// Lowest strength from accelerometer 100ohm			CHECK
		IR_SEL_LO = 0;						// Ties LED to ground							CHECK
		IR_SEL_HI = 0;
    }   
	else if(att_strength == 2) {			// Medium low strength from accel. 22ohm
		IR_SEL_LO = 1;						// 
		IR_SEL_HI = 0; 
    }
	else if(att_strength == 3) {			// Medium low strength from accel.56ohm
		IR_SEL_LO = 0;						// 
		IR_SEL_HI = 1;
    }
	else if(att_strength == 4) {			// Medium low strength from accel. 12ohm
		IR_SEL_LO = 1;						// 
		IR_SEL_HI = 1;
    }
	encode_data();
}
  
void encode_data()							// Encodes binary packet excluding start of data and end of data
{
   	
	start_tx();								// start of data routine
	
	//Binary Packet - Hard coded
	
	// Upper 12 bits - regular
	IR_tx_0();
	IR_tx_1();
	IR_tx_0();
	IR_tx_1();
	IR_tx_0();
	IR_tx_1();
	IR_tx_0();
	IR_tx_1();
	IR_tx_0();
	IR_tx_1();
	IR_tx_0();
	IR_tx_1();
	// Lower 12 bits - inverted
	IR_tx_1();
	IR_tx_0();
	IR_tx_1();
	IR_tx_0();
	IR_tx_1();
	IR_tx_0();
	IR_tx_1();
	IR_tx_0();
	IR_tx_1();
	IR_tx_0();
	IR_tx_1();
	IR_tx_0();


	end_tx();
}

void start_tx()								// Start of Data protocol routine
{
	SetDCPWM2(0b100000000);								
	Delay100TCYx(35);						// Delays for 2ms
	Delay100TCYx(35);						// Delays for 2ms
	SetDCPWM2(0b000000000);
	Delay100TCYx(35);						// Delays for 2ms
	Delay100TCYx(35);						// Delays for 2ms
}

void IR_tx_0()								// IR transmit routine for logic zero 0.5ms ON 1ms OFF 
{
	SetDCPWM2(0b100000000);
	Delay100TCYx(7);						// Delays for 0.5ms
	SetDCPWM2(0b000000000);
	Delay100TCYx(17);						// Delays for 1ms

}
 
void IR_tx_1()								// IR transmit routine for logic one 0.5ms ON 2ms OFF 
{
	SetDCPWM2(0b100000000);
	Delay100TCYx(7);						// Delays for 0.5ms
	SetDCPWM2(0b000000000);
	Delay100TCYx(35);						// Delays for 2ms

}

void end_tx()								
{	
	SetDCPWM2(0b100000000);
	Delay100TCYx(7);						// Delays for 0.5ms
	SetDCPWM2(0b000000000);
}

void test_IR_tx() {
   att_strength = 4;
   while(1)
      set_IR_strength();
}

/****************************************
 *   
 *             Ammo & Visual
 *               Routines
 *   
 ****************************************/

//--> Timer3 overflow ISR to scroll the 4 strength gauge LEDs
void update_strength_gauge() {
   //--> Handle a gauge overflow if latency has passed
   if ( t3_ovf == STRENGTH_GAUGE_LATENCY ) {
      // Reset overflow counter
      t3_ovf = 0;
     // Update strength LEDs
      if ( gauge_index <= att_strength ) {
         switch (gauge_index) {
            case 0:
               PORTA &= ~STREN; break;
            case 1:
               PORTA |= 0x01;   break;
            case 2:
               PORTA |= 0x09;   break;
            case 3:
               PORTA |= 0x0B;   break;
            case 4:
               PORTA |= 0x0F;   break;
         }
         gauge_index++;  //Attacking should reset index
      }
   }
   //--> Increase number of overflows otherwise
   else
      t3_ovf++;
}

//--> Timer1 overflow ISR to track time since successful action
void update_cooldown() {
   // Attack cooldown
   if ( !att_rdy ) {
      if ( t1_ovf_att==COOLDOWN ) {
       // Reset attack variables
         att_rdy = 1;
         t1_ovf_att = 0;
       //**att_strength = 0;
       gauge_index = 0;
      }
      else
         t1_ovf_att++;
   }
   // Reload cooldown
   if ( !rld_rdy) {
      if ( t1_ovf_rld==COOLDOWN ) {
         rld_rdy = 1;
         t1_ovf_rld = 0;
      }
      else
         t1_ovf_rld++;
   }
}

//--> Routine to increase/decrease ammo and write to 7-S display
void update_ammo(char amount) {
   char code;
   // Numeric handling
   if ( (amount==-1) && (current_ammo>0) )
      current_ammo--;
   else if ( (amount==1) && (current_ammo<MAX_AMMO) )
      current_ammo++;
   else if (amount == MAX_AMMO)
      current_ammo = MAX_AMMO;
   else if (amount == 0)
      current_ammo = 0;
   // 7-Segment handling
   code = (current_ammo << 4);   //Move binary ammo to upple nibble
   PORTD &= ~SEGMENT;            //Clear 7S display
   PORTD |= code;                //Output binary number
}

//--> Method to read external switches that activate RLD and ATT cooldowns
void test_cooldown() {
   // Attack:  check RD2 for a pressed switch
   if ( DUMB2==1 && att_rdy && current_ammo>0 ) {
      att_rdy = 0;
      PORTDbits.RD0 = 0;   //Turn off an LED on R0
     update_ammo(-1);
   }
   else if (att_rdy)
      PORTDbits.RD0 = 1;   //Turn on RD0 LED
   // Reload: check RD3 for a pressed switch
   if ( DUMB3==1 && rld_rdy && current_ammo<=MAX_AMMO ) {
      rld_rdy = 0;
      PORTDbits.RD1 = 0;   //Turn off an LED on R1
     update_ammo(1);
   }
   else if (rld_rdy)
      PORTDbits.RD1 = 1;   //Turn on RD1 LED
}

void test_7S() {
   int i;
   update_ammo(MAX_AMMO);
   while(1) {
      update_ammo(-1);
      for( i=0 ; i<500 ; i++)
         delay_ms(1);
      update_ammo(-1);
      for( i=0 ; i<500 ; i++)
         delay_ms(1);
      update_ammo(-1);
      for( i=0 ; i<500 ; i++)
         delay_ms(1);
      update_ammo(-1);
      for( i=0 ; i<500 ; i++)
         delay_ms(1);
      update_ammo(-1);
      for( i=0 ; i<500 ; i++)
         delay_ms(1);
      update_ammo(1);
      for( i=0 ; i<500 ; i++)
         delay_ms(1);
      update_ammo(1);
      for( i=0 ; i<500 ; i++)
         delay_ms(1);
      update_ammo(1);
      for( i=0 ; i<500 ; i++)
         delay_ms(1);
      update_ammo(1);
      for( i=0 ; i<500 ; i++)
         delay_ms(1);
      update_ammo(1);
      for( i=0 ; i<500 ; i++)
         delay_ms(1);
   }
}

void test_strength_gauge() {
   gauge_index = 0;
   att_strength = 4;
   while(1)
      if( gauge_index > att_strength )
         gauge_index = 0;
}
/****************************************
 *   
 *              Auxilliary
 *               Routines
 *   
 ****************************************/

//--> Halts program execution for a number of milliseconds
void delay_ms(int milli) {
   Delay1KTCYx(2* milli);
}

//--> Changes a binary number to ascii so it can be printed with UART
void display_decimal(int number) {
	int j=0;
    int value[5];
    char isNegative = 0;

    if(number < 0) {
        number = ~number + 1;
        isNegative = 1;
    }

    if(number>=10000) {          //Ten-Thousand's place
        j = number / 10000;
        value[0] = j + 48;
        number = number % 10000;
    }
    else
        value[0] = '0';
    if(number>=1000) {           //Thousand's place
        j = number / 1000;
        value[1] = j + 48;
        number = number % 1000;
    }
    else
        value[1] = '0';
	if(number>=100) {             //Hundred's place
		j = number / 100;
		value[2]= j + 48;         //ASCII offset
		number = number % 100;
	}
	else
		value[2]= '0';
	if(number>=10) {             //Ten's place
		j = number / 10;
		value[3]= j + 48;         //ASCII offset
		number 	= number % 10;
	}
	else
		value[3]= '0';
	value[4] 	= number + 48;     //One's place

   //Print to UART
   if(isNegative)
      UART_tx('-');
   if(value[0] != '0')
      UART_tx(value[1]);
   if(value[1] != '0')
      UART_tx(value[1]);
   if(value[2] != '0')
      UART_tx(value[2]);
   if(value[3] != '0')
      UART_tx(value[3]);
   UART_tx(value[4]);
   UART_tx(' ');
}


/****************************************
 *   
 *           Initialization
 *              Methods
 *   
 ****************************************/
void init_all() {
   init_port_directions();
   init_oscillator();
   init_interrupts();
   init_timers();
   init_UART();
   init_accelerometer();
   init_PWM();
   init_audio();
   update_ammo(MAX_AMMO);

}

void init_port_directions() {
   // 0 = output //
   // 1 = input  //

   //--> Strength LEDs
   TRISAbits.TRISA0 = 0;
   TRISAbits.TRISA1 = 0;
   TRISAbits.TRISA2 = 0;
   TRISAbits.TRISA3 = 0;
   //--> Audio 3 and 4
   TRISAbits.TRISA4 = 0;
   TRISAbits.TRISA5 = 0;
   TRISAbits.TRISA6 = 0;   //Unconnected
   TRISAbits.TRISA7 = 0;   //Unconnected

   //--> IR Selectors
   TRISBbits.TRISB0 = 0;
   TRISBbits.TRISB1 = 0;
   //--> IR-Tx and IR-Rx
   TRISBbits.TRISB2 = 1;
   TRISBbits.TRISB3 = 0;
   //**TRISBbits.TRISB4 = 0;
   //--> Programming pins (arbitrary)
   TRISBbits.TRISB5 = 1;
   TRISBbits.TRISB6 = 1;
   TRISBbits.TRISB7 = 1;

   //--> Reload switch
   TRISCbits.TRISC0 = 1;
   //--> Dummy pin
   TRISCbits.TRISC1 = 1;
   //--> Chip select
   TRISCbits.TRISC2 = 0;
   //--> SPI pins
   TRISCbits.TRISC3 = 0;  //SCK
   TRISCbits.TRISC4 = 1;  //SDI
   TRISCbits.TRISC5 = 0;  //SD0
   //--> UART
   TRISCbits.TRISC6 = 1;  //Tx
   TRISCbits.TRISC7 = 1;  //Rx specified in datasheet

   //--> Dummy Pins
   TRISDbits.TRISD0 = 0;  //Attack LED
   TRISDbits.TRISD1 = 0;  //Reload LED
   TRISDbits.TRISD2 = 1;  //Attack switch
   TRISDbits.TRISD3 = 1;  //Reload switch
   //--> 7-Segment
   TRISDbits.TRISD4 = 0;
   TRISDbits.TRISD5 = 0;
   TRISDbits.TRISD6 = 0;
   TRISDbits.TRISD7 = 0;


   //-->Ready LED
   TRISEbits.TRISE2 = 0;
}

void init_oscillator() {
   OSCCON &= ~0x03;               //Choose primary oscillator
   OSCTUNEbits.PLLEN = 1;         //Enable PLL multiplier
   OSCCON |= 0x70;                //Set to 8 MHz
}

void init_audio()
{
   //-->Audio 1 and 2
   TRISEbits.TRISE0 = 0;
   TRISEbits.TRISE1 = 0;
   //--> Audio 3 and 4
   TRISAbits.TRISA4 = 0;
   TRISAbits.TRISA5 = 0;
   ADCON0bits.ADON = 0;
   SET_AUD_A;
   SET_AUD_E;
}
void init_interrupts() {
   //--> Enable all interrupt handling
   RCONbits.IPEN = 1;       //Enable interrupt priorities
   INTCONbits.GIEH = 1;     //Enable high-priority interrupts
   INTCONbits.PEIE = 1;     //Enable low-priority interrupts
   //--> IR-Rx external interrupt pin
   INTCON3bits.INT2IE = 1;   //Enables external on RB2
   INTCON3bits.INT2IP = 1;   //Sets to High priority
   INTCON2bits.INTEDG2 = 0;  //Falling edge triggered
}

void init_timers() {
   //--> Timer 1
   T1CONbits.TMR1ON = 1;    //Enable Timer 1
   T1CONbits.RD16 = 1;      //Choose 16-bit
   T1CON &= ~0x30;          //Set a 1:8 prescalar
   T1CONbits.TMR1CS = 0;    //Uses clock of Fosc/4
   PIE1bits.TMR1IE = 1;     //Enables overflow interrupt
   IPR1bits.TMR1IP = 0;     //Set to low priority
   //--> Timer 3
   T3CONbits.TMR3ON = 1;    //Enable Timer 3
   T3CONbits.RD16 = 1;      //Choose 16-bit
   T3CON &= ~0xF0;           //Set a 1:1 prescalar
   T3CONbits.TMR3CS = 0;    //Uses clock of Fosc/4
   PIE2bits.TMR3IE = 1;     //Enables overflow interrupt
   IPR2bits.TMR3IP = 0;     //Set to low priority
}

void init_SPI() {
   PORTCbits.RC2 = 1;        //Set Chip Select pin high
   SSPCON1bits.SSPEN = 1;    //Enable serial ports
   SSPCON1 &= ~0x0F;       //Set SCK at Fosc/4
   SSPCON1bits.CKP = 1;      //SCK idles high
   SSPSTATbits.CKE = 0;      //Data transmitted on rising edge
   SSPSTATbits.SMP = 1;      //Data sampled on falling edge
   //**PIE1bits.SSPIE = 1;       //Enable SSPBUF interrupt
   //**IPR1bits.SSPIP = 0;       //Set low priority
}

void init_UART() {
   RCSTAbits.SPEN = 1;      //Enable RC6:7 as serial ports
   RCSTAbits.RX9 = 0;       //Set 8-bit packets
   TXSTAbits.TXEN = 1;      //Enable Tx
   RCSTAbits.CREN = 1;      //Enable Rx
   TXSTAbits.SYNC = 0;      //Set asychronous
   BAUDCONbits.TXCKP = 0;   //Tx idle (1=high)
   BAUDCONbits.RXDTP = 0;   //Rx idle (1=high)
   SPBRG = 12;              //Set a 9600 baud rate
   SPBRGH = 0;              //  --
   PIE1bits.RCIE = 1;       //Enable interrupt on receive
   IPR1bits.RCIP = 0;       //Set low priority
}

void init_accelerometer() {
   CHIP_SEL = 1;
   OpenSPI(SPI_FOSC_4, MODE_11, SMPMID);
   acc_config(DATA_FORMAT, 0x00);           // 2g max
   acc_config(INT_MAP, 0x9F);               // Single, double to INT1
   acc_config(TAP_AXES, 0x07);              // X, Y, Z tap
   acc_config(THRESH_TAP, GRAVITY);         // 511 thresh maximum
//**   acc_config(OFXZ, -128);              // account for 1g
   acc_config(DUR, 0x10);
   acc_config(LATENT, 0x50);
   acc_config(WINDOW, 0xFF);
   acc_config(BW_RATE, 0x0F);               // 800 Hz sample rate
   acc_config(FIFO_CTL, 0x1F);              // Hold 32 samples
   acc_config(INT_ENABLE, 0x60);            // Enable single, double
   acc_config(POWER_CTL, 0x08);             // Set to monitor activity
}

void init_PWM() {
	OpenTimer2(TIMER_INT_OFF & T2_PS_1_1 & T2_POST_1_1);
    OpenPWM2(140);
    SetDCPWM2(0b000000000);
    //SetOutputPWM1(SINGLE_OUT, PWM_MODE_1);
}

/****************************************
 *   
 *              Interrupt
 *              Routines
 *   
 ****************************************/

#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh (void) {
  _asm
     goto InterruptServiceHigh  //jump to interrupt routine
  _endasm
}

#pragma code InterruptVectorLow = 0x18
void InterruptVectorLow (void) {
  _asm
     goto InterruptServiceLow  //jump to interrupt routine
  _endasm
}

#pragma interrupt InterruptServiceHigh
void InterruptServiceHigh() 
{
	//--> Check Timer0 Interrupt Flag
	if (INTCONbits.TMR0IF)     	
	{
		if(isReceiving == 1)
			start_of_data();
		INTCONbits.TMR0IF = 0;		// Clear Interrupt Flag
	}
	//--> Check RB2 Interrupt Pin
	if(INTCON3bits.INT2IF)
	{
		isReceiving = 1;
		T0CONbits.TMR0ON = 1;		// Enables Timer0
		TMR0L &= 0x00;				// Clears Timer0 register				CHECK!!!
		INTCON3bits.INT2IE = 0;		// Disables external interrupt on RB2
		INTCON3bits.INT2IF = 0; 	// Clear Interrupt Flag
	}
}

#pragma interrupt InterruptServiceLow
void InterruptServiceLow() {
   //--> Check Timer 1 interrupt flag
   if (PIR1bits.TMR1IF) {
      update_cooldown();
      PIR1bits.TMR1IF = 0;      //Clear interrupt flag
      // Ready LED toggle for testing
      if (toggle_enable) {
         if (LED_toggle == 1) {
            READY = 1;
            LED_toggle = 0;
         }
         else {
            READY = 0;
            LED_toggle = 1;
         }
      }
   }
   //--> Check Timer 3 interrupt flag
   if (PIR2bits.TMR3IF) {     
      update_strength_gauge();
      PIR2bits.TMR3IF = 0;      //Clear interrupt flag
   }
   //--> Check for UART receive
   if (PIR1bits.RCIF) {
      UART_rx();
   }
}
