/*
 *  shuriken.h - Connor Ledgerwood
 *
 *  Provides aliases for pin assignments
 *
*/

/****************************************
 *   
 *              Constants
 *    
 ****************************************/

#define STRENGTH_GAUGE_LATENCY 1    // 100ms at Fosc=8MHz and 1:1 prescale
#define COOLDOWN 61                 // 2s at Fosc=8MHz and 1:8 prescale
#define MAX_AMMO 5

#define NUM_OVF_START	16		// 4ms Start of Data signal
#define NUM_OVF_ZERO	4		// 1ms OFF signal for logic Zero
#define NUM_OVF_ONE		8		// 2ms OFF signal for logic One
#define NUM_OVF_ON		2		// 0.5ms ON signal 

#define NUM_AXIS_DATA 32
#define GRAVITY       255       // At 4g resolution
#define MAX_RATIO     255.0

/****************************************
 *   
 *             Bitmasks
 *    
 ****************************************/

#define  ACC_INT   PORTDbits.RD0
#define  CHIP_SEL  PORTCbits.RC2  //PORTDbits.RD1 

#define  CLR_AUD_1  LATE &= 0xFD
#define  CLR_AUD_2  LATE &= 0xFE
#define  CLR_AUD_3  LATA &= 0xDF
#define  CLR_AUD_4  LATA &= 0xEF
#define  SET_AUD_E  PORTE |= 0x03; LATE |= 0x03
#define  SET_AUD_A  PORTA |= 0x30; LATA |- 0x30

#define  READY   PORTBbits.RB4
#define  RELOAD  PORTCbits.RC0

#define  IR_SEL_LO  PORTBbits.RB0
#define  IR_SEL_HI  PORTBbits.RB1
#define  IR_RX      PORTBbits.RB2
#define  IR_TX      PORTBbits.RB3

#define  SEGMENT  0xF0  //RD 4:7
#define	 STREN	  0x0F  //RA 0:3

#define  DUMB1  PORTDbits.RD1
#define  DUMB2  PORTDbits.RD2
#define  DUMB3  PORTDbits.RD3
#define  DUMB4  PORTCbits.RC1
#define  DUMB5  PORTEbits.RE2

/****************************************
 *   
 *                Function
 *        Definitions & Prototypes
 *    
 ****************************************/

//Initialization routines
extern void init_all();
extern void init_port_directions();
extern void init_oscillator();
extern void init_SPI();
extern void init_UART();
extern void init_interrupts();
extern void init_timers();
extern void init_PWM();
extern void init_accelerometer();
// UART routines
extern void UART_tx(char);
extern void UART_tx_str(char message[50]);
extern void UART_rx();
// Accelerometer routines
extern void acc_config(char, char);
extern char acc_read(char);
extern void read_fifo();
extern void monitor_interrupt();
extern int  convert_to_int(int high, int low);
extern int  find_maximum(int*);
extern char calculate_strength(int point);
extern void bad_motion();
extern void display_readings();
extern void print_axis(int * axis);
// IR Receive routines
extern void start_of_data();
extern void read_data(char);
extern void dataDecode();
extern void end_of_data();
extern void array_shift();
// IR Transmit routines
extern void set_IR_strength();
extern void encode_data();
extern void start_tx();
extern void IR_tx_0();
extern void IR_tx_1();
extern void end_tx();
// Ammo and cooldown routines
extern void update_strength_gauge();
extern void update_cooldown();
extern void update_ammo(char);
// Auxilliary routines
extern void delay_ms(int);
extern void display_decimal(int);
// Test routines
extern void test_strength_gauge();
extern void test_cooldown();
extern void test_7S();
extern void test_UART();
extern void test_SPI();
extern void test_accelerometer();
extern void test_throw_strength();
extern void test_IR_tx();
// Interrupt routines
extern void InterruptServiceHigh();
extern void InterruptServiceLow();

//Initialization routines
void init_all();
void init_port_directions();
void init_oscillator();
void init_SPI();
void init_SPI_std();
void init_UART();
void init_interrupts();
void init_timers();
void init_PWM();
void init_accelerometer();
// UART routines
void UART_tx(char);                  //works
void UART_tx_str(char message[50]);  //works
void UART_rx();                      //works
// Accelerometer routines
void acc_config(char, char);             //
char acc_read(char);                     //
void read_fifo();                        //
void monitor_interrupt();                //
int  convert_to_int(int high, int low);  //
int  find_maximum(int*);                 //
char calculate_strength(int point);      //
void bad_motion();                       //
void display_readings();                 //
void print_axis(int * axis);             //
// IR Receive routines
void start_of_data();                //
void read_data(char);                //
void dataDecode();                   //
void end_of_data();                  //
void array_shift();                  //
// IR Transmit routines
void set_IR_strength();              //
void encode_data();                  //
void start_tx();                     //
void IR_tx_0();                      //
void IR_tx_1();                      //
void end_tx();                       //
// Ammo and cooldown routines
void update_strength_gauge();        //works
void update_cooldown();              //works
void update_ammo(char);              //works
// Auxilliary routines
void delay_ms(int);                  //works
void display_decimal(int);           //works
// Test routines
void test_strength_gauge();          //
void test_cooldown();                //works
void test_7S();                      //works
void test_UART();                    //works
void test_SPI();                     //works
void test_throw_strength();          //works
void test_accelerometer();           //
void test_IR_tx();                   //
// Interrupt routines
void InterruptServiceHigh();         //
void InterruptServiceLow();          //works
