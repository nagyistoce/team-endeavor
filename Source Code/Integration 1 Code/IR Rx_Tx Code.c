// IR Receiver and Transmitter
#include "p18F4520.h"
#include "shuriken.h"
#include <string.h> 

#define NUM_OVF_START	16		// 4ms Start of Data signal
#define NUM_OVF_ZERO	4		// 1ms OFF signal for logic Zero
#define NUM_OVF_ONE		8		// 2ms OFF signal for logic One
#define NUM_OVF_ON		2		// 0.5ms ON signal 

/************************************** GLOBAL VARIABLES **********************/
char isReceiving = 0;			// Receive Trigger (RB2)
char expected_PB2 = 1;			// Previous state of PB2
char low_ovf = 0;				// Number of low overflows
char high_ovf = 0;				// Number of high overflows
char IR_data_ready = 0;			// IR packet has been read in and ready to be transmitted to Xbee
char IR_Data[24];				// Array for storing packet data
char IR_Packet_rx = 0;
char lowTRUE = 0;				// S-o-D ON signal boolean
char highTRUE = 0;				// S-o-D OFF signal boolean

/************************************** FUNCTION DECLARATIONS **********************/

extern void init_all();
	/***** IR Data Routines ****/
extern void start_of_data();
extern void read_data(char);
extern void dataDecode();
extern void end_of_data();
extern void array_shift();
	/***** Initialization ****/
extern void init_oscillator(); 
extern void init_port_directions();
extern void init_interrupts();
extern void init_timers();
extern void init_PWM();
extern void InterruptServiceHigh();
extern void init_UART();
extern void UART_tx(char);
extern void delay_ms(int);
extern void test_UART();
/************************************** FUNCTION PROTOTYPES **********************/

void init_all();
void start_of_data();
void read_data(char);
void dataDecode();
void end_of_data();
void array_shift();
void init_oscillator(); 
void init_port_directions();
void init_interrupts();
void init_timers();
void init_PWM();
void InterruptServiceHigh();
void init_UART();
void test_UART();
void UART_tx(char);
void delay_ms(int);

/************************************** MAIN *************************************/
void main()
{
	init_all();
	while(1);
	{
		UART_tx('F');
	}	
}


/************************************** Start of Data Routine *********************/
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


/************************************** Data Decode Routine **********************/
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
/************************************** Read Data Routine **********************/
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

/************************************** End of Data Routine **********************/
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

/************************************** Array Shift Routine **********************/
void array_shift()
{	
	int n;
	for(n = 0; n < 12; n++)
	{
		IR_Packet_rx = (IR_Data[n] << n);
	}
}


/********************
 *   
 *     UART Testing
 *     Routines
 *   
 ********************/

//--> Method to transmit a string through UART
void test_UART() 
{
	char message[12] = "Hello World";
	char IR_Packet_rx;
	char echo;
	char method = 0;  // 0=Tx , 1=Rx
	int i;

	if(method == 0)
	{
		int z;
		for(z = 0; z < 11; z++)
		{
			UART_tx(message[i]);
			UART_tx('\n');
			UART_tx('\r');
		}

		UART_tx(IR_Packet_rx);
		UART_tx('\n');
		UART_tx('\r');
	}
}

/********************
 *   
 *     Transmitter
 *     Routine
 *   
 ********************/
 
 /************************************** Throw Strength Routine **********************/
 
 void determine_str()
 {
	// Take in throw strength from Accel. 
	// Turn on selctor bits
	if(str == 0)							// Lowest strength from accelerometer						CHECK
		IR_SEL = 0x00;						// Ties LED to ground										CHECK
	else if(str == 1)						// Medium low strength from accel.
		IR_SEL == 0x01;						// 
	encode_data();
 }
  
  /************************************** Binary Packet Routine **********************/
  
 void encode_data()							// Encodes binary packet excluding start of data and end of data
{
	T0CONbits.TMR0ON = 1;					// Enables Timer0
	IR_Packet_tx[24] = 0xAAA555;   			// IR Packet to be transmitted
	
	start_tx();								// start of data routine
		
	for(a = 0; a < 24; a++)					// Encodes binary packet data
	{
		if(IR_Packet_tx[a] == 0)
		{
			IR_tx_0();						// Calls IR transmit for logic zero
		}
		else if(IR_Packet_tx[a] == 1)
		{
			IR_tx_1();						// Calls IR transmit for logic one
		}
	}
	
	end_tx();								// End of data routine
	
	PORTBbits.PB# = 0;						// Turns off IR LED
	T0CONbits.TMR0ON = 0;					// Disables Timer0
}
/************************************** Start of Data Routine **********************/

void start_tx()								// Start of Data protocol routine
{
	char start_on = 0;						// 'start_on' variable used to count to 4ms ON signal
	char start_off = 0;						// 'start_off' variable used to count to 4ms OFF signal
	while(start_on < NUM_OVF_START)
	{
		// PWM on							// 56kHz sqaure wave at whatever duty cycle
		PORTBbits.PB# = 1;					// Outputs to IR LED 
		start_on++;							// Increments till start of data bit (High) has been reached
		
	}
	while(start_off < NUM_OVF_START)
	{
		// PWM off							// Lack of PWM 56kHz signal
		start_off++;						// Increments till start of data bit(LOW) has been reached
	}
}

/************************************** Logic Zero Routine **********************/

void IR_tx_0()								// IR transmit routine for logic zero 0.5ms ON 1ms OFF 
{
	char ON_0 = 0;
	char OFF_0 = 0;
	while(ON_0 < NUM_OVF_ON)
	{
		// PWM ON
		PORTBbit.PB# = 1;					// Outputs to IR LED
		ON_0++;								// Increments till 0.5ms has been reached
	}
	while(OFF_0 < NUM_OVF_ZERO)
	{
		PORTBbit.PB# = 0;					// Turns off IR LED
		OFF_0++;							// Increments till 1ms has been reached
	}
}

/************************************** Logic One Routine **********************/

void IR_tx_1()								// IR transmit routine for logic one 0.5ms ON 2ms OFF 
{
	char ON_1 = 0;
	char OFF_1 = 0;
	while(ON_1 < NUM_OVF_ON)
	{
		//PWM ON
		PORTBbits.PB# = 1;					// Turns on IR LED
		ON_1++;								// Increments till 0.5ms has been reached
	}
	while(OFF_1 < NUM_OVF_ONE)
	{
		PORTBbit.PB# = 0;					// Turns off IR LED
		OFF_1++;							// Increments till 2ms has been reached
	}
}
 
 /************************************** End of Data Routine **********************/
 
 void end_tx()								
 {
	while(OFF < NUM_OVF_ON)
	{
		PORTBbits.PB# = 1;					// Turns on IR LED for 0.5ms
		OFF++;
	}
 }

 
/********************
 *   
 *      Serial
 *     Routines
 *   
 ********************/

//--> UART transmit method
void UART_tx(char dat) {
   if (PIR1bits.TXIF == 1)
      TXREG = dat;
   else {
      delay_ms(1);
      UART_tx(dat);
   }
}

/************************************** INITIALIZATION **********************/
void init_all()
{
	init_oscillator();
	init_port_directions();
	init_interrupts();
	init_timers();
	init_PWM();
	init_UART();
	
}

/************************************** INITIALIZATION METHODS **********************/
void init_oscillator() 
{
   OSCCON &= ~0x03;               //Choose primary oscillator
   OSCTUNEbits.PLLEN = 1;         //Enable PLL multiplier
   OSCCON |= 0x70;                //Set to 8 MHz
   //*** while (OSCCONbits.IOFS == 0);  //Wait for stability
}

void init_interrupts() 
{
   //--> Enable all interrupt handling
   RCONbits.IPEN = 1;       //Enable interrupt priorities
   INTCONbits.GIEH = 1;     //Enable high-priority interrupts
   INTCONbits.PEIE = 1;     //Enable low-priority interrupts
   //--> IR-Rx external interrupt pin
   INTCON3bits.INT2IE = 1;   //Enables external interrupt on RB2
   INTCON3bits.INT2IP = 1;   //Sets to High priority
   INTCON2bits.INTEDG2 = 0;  //Falling edge triggered				CHECK
}

void init_port_directions()
{
	//--> IR Selectors
   TRISBbits.TRISB0 = 0;
   TRISBbits.TRISB1 = 0;
   //--> IR-Rx and IR-Tx
   TRISBbits.TRISB2 = 1;
   TRISBbits.TRISB3 = 0;
   //--> UART
   TRISCbits.TRISC6 = 1;  //Tx
   TRISCbits.TRISC7 = 0;  //Rx specified in datasheet
}
void init_timers() 
{
	//--> Timer0
	//T0CONbits.TMR0ON = 1;			// Enables Timer0
	T0CONbits.T08BIT = 1;			// Sets Timer0 to 8bit (1) or 16bit (0)
	T0CONbits.T0CS = 0;				// Sets clock source to internal clock (0) or external (1)
	T0CONbits.PSA = 0;				// Enables Prescaler
	T0CONbits.T0PS2 &= 0;			// Prescaler values 1:2 (000)
	T0CONbits.T0PS1 = 0;
	T0CONbits.T0PS0 = 0;
	INTCON2bits.TMR0IP = 1;			// Enables Timer0 Interrupt Flag
	INTCONbits.TMR0IE = 1;			// Sets Interrupt to high priority
}	

void init_PWM()
{
	
}

void init_UART() 
{
   RCSTAbits.SPEN = 1;      //Enable RC6:7 as serial ports
   RCSTAbits.RX9 = 0;       //Set 8-bit packets
   TXSTAbits.TXEN = 1;      //Enable Tx
   TXSTAbits.SYNC = 0;      //Set asychronous
   BAUDCONbits.TXCKP = 0;   //Tx idle (1=high)
   BAUDCONbits.RXDTP = 0;   //Rx idle (1=high)
   SPBRG = 12;              //Set a 9600 baud rate
   SPBRGH = 0;              //  --
   //**BAUDCONbits.ABDEN = 1; //Or set auto-detect
   PIE1bits.RCIE = 1;       //Enable interrupt on receive
   IPR1bits.RCIP = 0;       //Set low priority
}
void delay_ms(int milli) {
   const int CYC_PER_MS = 8000;
   long delay = (CYC_PER_MS * milli);     // Number of instructions
   long i;
   for (i=0 ; i<delay ; i++);
}

/************************************** INTERRUPT ROUTINES **********************/
#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh (void) {
  _asm
     goto InterruptServiceHigh  //jump to interrupt routine
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