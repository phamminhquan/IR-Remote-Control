/*
 * 2-NEC IR REMOTE CONTROL.c
 *
 * Created: 8/8/2017 10:18:03 AM
 * Author : QUAN1
 */ 


// INCLUDES
#include <avr/io.h>
#include <avr/interrupt.h>

// FUNCTION PROTOTYPES
void set_clock(void);
void ext_init(void);
void tc_init(void);
void tc_disable(void);
void ext_disable(void);
void ext_disable(void);


// GLOBAL VARIABLES
int kaylee[] = {1,345,171, 21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21, 21,64,21,64,21,64,21,64,21,64,21,64,21,64,21,64, 21,21,21,64,21,64,21,21,21,21,21,21,21,21,21,21, 21,64,21,21,21,21,21,64,21,64,21,64,21,64,21,64, 21,1000};
int samsung[] = {1,171,171, 21,64,21,64,21,64,21,21,21,21,21,21,21,21,21,21, 21,64,21,64,21,64,21,21,21,21,21,21,21,21,21,21, 21,21,21,64,21,21,21,21,21,21,21,21,21,21,21,21, 21,64,21,21,21,64,21,64,21,64,21,64,21,64,21,64, 21,1000};
int select=0;
int i=1;


//////////////////////////// MAIN ////////////////////////////////
int main(void)
{
	//IR initialization
	PORTC_DIRSET = 0x01;
	PORTC_OUTCLR = 0x01;
	
	//initializes peripherals
	set_clock();
	ext_init();
	
	PMIC_CTRL = 0x04;
	sei();
	
	while (1)
	{
	}
}
//////////////////////////////////////////////////////////////////


//////////////////////////// ISR /////////////////////////////////
// EXTERNAL TRIGGER S1 **********************************************
ISR(PORTA_INT0_vect)
{
	ext_disable();
	tc_init();
}
//****************************************************************



// TCC0 OVERFLOW *************************************************
ISR(TCC0_OVF_vect)
{
	if(i == samsung[select])
	{
		PORTC_OUTTGL = 0x01;
		i=1;
		select++;
	}
	else if(i < samsung[select])
	{
		i++;
	}
	
	if(select >= 69)
	{
		PORTC_OUTCLR = 0x01;
		select=0;
		i=1;
		tc_disable();
		PORTA_INTFLAGS = 0x01;
		ext_init();
		PORTA_INTFLAGS = 0x01;
	}
}
// ***************************************************************





////////////////////////// FUNCTIONS /////////////////////////////


// EXTERNAL INTERRUPT INIT ********************************
// This function initializes external interrupt for button connected to PORT A PIN 0
// Input: N/A
// Output: N/A
void ext_init(void)
{
	PORTA_DIRCLR = 0x01;				//make interrupt pin as input
	PORTA_INTCTRL = 0x03;				//set high level interrupt
	PORTA_INT0MASK = 0x01;				//set pin 2 as trigger source
	PORTA_PIN0CTRL = 0x03;				//set low level trigger
}
//***************************************************************


// EXTERNAL INTERRUPT DISABLE ***********************************
// This function disable external interrupt on PORT F PIN 2
// Input: N/A
// Output: N/A
void ext_disable(void)
{
	PORTA_INTCTRL = 0x00;				//disable interrupt
	PORTA_INTFLAGS = 0x01;				//clear flag
}
// **************************************************************


// TC INIT *******************************************************
// This function initializes TCC0
// Input: N/A
// Output: N/A
void tc_init(void)
{
	TCC0_PER = 850;							//init period to approximately 1 second
	TCC0_INTCTRLA = 0x03;					//Overflow interrupt high level
	TCC0_CTRLA = TC_CLKSEL_DIV1_gc;			//timer counter clock select division group config
	return;
}
//***************************************************************


// TC DISABLE ***************************************************
// This function disable TCC0
// Input: N/A
// Output: N/A
void tc_disable(void)
{
	TCC0_CTRLA = 0x00;				//timer counter clock select division group config
	TCC0_PER = 0x0000;				//init period to approximately 1 second
	TCC0_INTCTRLA = 0x00;			//Overflow interrupt high level
}
//***************************************************************




// SET CLOCK *****************************************************************************************
// This function initialize system clock to 32MHz
// Input : N/A
// Output: N/A
void set_clock(void)
{
	OSC_CTRL = 0x02;						//enable 32MHz oscilaltor
	while (1)
	{
		if ((OSC_STATUS & 0x02) == 0x02)				//check if oscillator is ready
		{break;}
	}
	
	CPU_CCP = CCP_IOREG_gc;					//protect IO registers
	CLK_CTRL = 0x01;						//select 32MHz oscillator as system clock
	
	return;
}
//****************************************************************************************************

