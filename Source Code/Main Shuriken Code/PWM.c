// PWM setup for 56kHz using 8MHz internal clock

/*
 * PWM registers configuration
 * Fosc = 8000000 Hz
 * Fpwm = 55555.56 Hz (Requested : 56000 Hz)
 * Duty Cycle = 50 %
 * Resolution is 7 bits
 * Prescaler is 4
 * Ensure that your PWM pin is configured as digital output
 * see more details on http://www.micro-examples.com/
 * this source code is provided 'as is',
 * use it at your own risks
 
PR2 = 0b00001000 ;						// Sets period register
T2CON = 0b00000101 ;					// Setup Timer2 bits
CCPR1L = 0b00000100 ;
CCP1CON = 0b00011100 ;	*/				//


extern void PWM_init();
extern void timer2_init();

void PWM_init();
void timer2_init();

void init()
{
	timer2_init(); 							// Enable Timer 2
    PWM_init();								// Set up the PWM
}

void timer2_init()  						// Initialize Timer Two for 56kHz PWM
{
	T2CON = 0b00000101 ;					// Sets Timer2 bits, 1:1 postscale, prescaler of 4
	PR2 = 0b00001000 ;						// Sets period register, when TMR2 and PR2 are equal an interrupt occurs
}

void PWM_init()								// Together these two registers create a 10-bit resolution/ 50% duty cycle
{
	CCP1CON = 0b00011100 ;					// 2-LSB
	CCPR1L = 0b00000100 ;					// 8 MSB bits PWM duty cycle
}

