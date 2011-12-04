#include <18F2580.h>

#device adc=8

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES HS                      //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES NOPROTECT                //Code not protected from reading
#FUSES BROWNOUT                 //Reset when brownout detected
#FUSES BORV21                   //Brownout reset at 2.1V
#FUSES NOPUT                    //No Power Up Timer
#FUSES NOCPD                    //No EE protection
#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES LVP                      //Low Voltage Programming on B3(PIC16) or B5(PIC18)
#FUSES NOWRT                    //Program memory not write protected
#FUSES NOWRTD                   //Data EEPROM not write protected
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES PBADEN                   //PORTB pins are configured as analog input channels on RESET
#FUSES BBSIZ2K                  //2K words Boot Block size
#FUSES NOWRTC                   //configuration not registers write protected
#FUSES NOWRTB                   //Boot block not write protected
#FUSES NOEBTR                   //Memory not protected from table reads
#FUSES NOEBTRB                  //Boot block not protected from table reads
#FUSES NOCPB                    //No Boot Block code protection
#FUSES LPT1OSC                  //Timer1 configured for low-power operation
#FUSES MCLR                     //Master Clear pin enabled
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)

#use delay(clock=20000000)
#use rs232(baud=57600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8, stream=USB)

// PIC pin defines
#define GREEN PIN_A0
#define YELLOW PIN_C2
#define RED PIN_A1
#define CAN_RS pin_c5

// CANbus baud rate settings, all are CCS PICC defaults except CAN_BRG_PRESCALAR
#define CAN_USE_EXTENDED_ID FALSE
#define CAN_BRG_PRESCALAR  9 // THIS ONE SETS CAN BAUD RATE TO 62500 kbps! 

#include <can-18xxx8.c>
#include <stdio.h>
#include <stdlib.h>
#include "packet.c"
#include "IR_rx.h"




// CAN global vars
int rx_len;
int32 rx_id;
int1 can_waiting;
struct rx_stat rxstat;
int1 CAN_data_ready;
int8 CAN_buffer[9];

// light flashing enable vars
int8 green_countdown;
int8 yellow_countdown;
int8 red_countdown;
   
// prototypes
void initialize();
void update_lights(float rat);
void change_lights(int16 process);

void initialize()
{
   can_waiting = 0;
   green_countdown = 0;
   yellow_countdown = 0;
   red_countdown = 0;
   
   can_init();
   
   output_low(CAN_RS);
   enable_interrupts(int_canrx0); 
   enable_interrupts(int_rda);
   enable_interrupts(int_TIMER0); 
   enable_interrupts(int_TIMER2);
   enable_interrupts(int_EXT);
   
   setup_adc_ports(NO_ANALOGS|VSS_VDD);
   setup_adc(ADC_OFF|ADC_TAD_MUL_0);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_16 );
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DIV_BY_16,141,4);
   setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   setup_vref(FALSE);
   
   enable_interrupts(GLOBAL);

   // Set low all unused pins
   output_low(pin_b4);
   output_low(pin_b5);
   output_low(pin_b1);
   output_low(pin_c3);
   output_low(pin_c4);
   output_low(pin_c0);
   output_low(pin_c1);
   output_low(pin_a2);
   output_low(pin_a3);
   output_low(pin_a4);
   output_low(pin_a5);
   
   //LED power on test sequence
   output_low(GREEN);
   output_low(YELLOW);
   output_low(RED);
   delay_ms(500);
   output_high(GREEN);
   delay_ms(100);
   output_high(YELLOW);
   delay_ms(100);
   output_high(RED);
   delay_ms(100);
   output_low(GREEN);
   delay_ms(100);
   output_low(YELLOW);
   delay_ms(100);
   output_low(RED);
   delay_ms(100);
   
   fprintf(USB,"Headband Initialized!@#");
}
