
#define GPIOCPID 	(unsigned int) 	4

#define LED1 		8
#define LED2 		29

#define BUTTON1 	4
#define BUTTON2 	5


#define SEGMENTS_LENGTH 		8
#define MAIN_DISPLAY_SWITCH 	31
#define DISPLAY_A_SWITCH 		30
#define DISPLAY_B_SWITCH 		28



#define UP_SEGMENT 				1U << 25
#define CENTER_SEGMENT 			1U << 26
#define LEFT_UP_SEGMENT 		1U << 27
#define RIGHT_UP_SEGMENT 		1U << 24

#define LEFT_DOWN_SEGMENT 		1U << 20

#define RIGHT_DOWN_SEGMENT 	1U << 22
#define DOWN_SEGMENT 			1U << 21
#define DOT_DISPLAY_SEGMENT 	1U << 23

#define MAX_TIMMER_VALUE	2097151


//AIC 
#define 	PRIORITY_HIGH 		(uint32_t)						7
#define 	IT_RISING			(uint32_t)						(1U << 5)	
#define 	TIMER_IT_ENABLE	(uint32_t)						(1U << 25)
#define 	TIMER_SWITCH		(uint32_t)						(1U << 24)


#include <stdio.h>
#include <stdint.h>
#include "AT91SAM9263.h"
#include "AT91SAM9263-EK.h"

uint32_t displayNumber = 0;
uint16_t timer_spins = 0;


void dbgu_print_ascii(){}

void delay(unsigned int mils)
{
	volatile int i;
	for(i = 0; i < mils * 2500; i++){}

}

const uint32_t segments[][10] = 
{
	{LEFT_DOWN_SEGMENT,	DOWN_SEGMENT,	RIGHT_DOWN_SEGMENT,	0,	RIGHT_UP_SEGMENT,	UP_SEGMENT,	0,		LEFT_UP_SEGMENT}, //0 OK
	{0,			0,		RIGHT_DOWN_SEGMENT,	0,	RIGHT_UP_SEGMENT,	0,		0, 		0}, //1 OK
	{LEFT_DOWN_SEGMENT,	DOWN_SEGMENT,	0,			0,	RIGHT_UP_SEGMENT,	UP_SEGMENT,	CENTER_SEGMENT, 0}, //2 OK
	{0,			DOWN_SEGMENT,	RIGHT_DOWN_SEGMENT,	0,	RIGHT_UP_SEGMENT,	UP_SEGMENT,	CENTER_SEGMENT, 0}, //3 OK
	{0,			0,		RIGHT_DOWN_SEGMENT,	0,	RIGHT_UP_SEGMENT,	0,		CENTER_SEGMENT, LEFT_UP_SEGMENT}, //4 OK
	{0,			DOWN_SEGMENT,	RIGHT_DOWN_SEGMENT,	0,	0,			UP_SEGMENT,	CENTER_SEGMENT, LEFT_UP_SEGMENT}, //5 OK
	{LEFT_DOWN_SEGMENT,	DOWN_SEGMENT,	RIGHT_DOWN_SEGMENT,	0,	0,			UP_SEGMENT,	CENTER_SEGMENT, LEFT_UP_SEGMENT}, //6 OK
	{0,			0,		RIGHT_DOWN_SEGMENT,	0,	RIGHT_UP_SEGMENT,	UP_SEGMENT,	0, 		0}, //7 OK
	{LEFT_DOWN_SEGMENT,	DOWN_SEGMENT,	RIGHT_DOWN_SEGMENT,	0,	RIGHT_UP_SEGMENT,	UP_SEGMENT,	CENTER_SEGMENT, LEFT_UP_SEGMENT}, //8 OK
	{0,			DOWN_SEGMENT,	RIGHT_DOWN_SEGMENT,	0,	RIGHT_UP_SEGMENT,	UP_SEGMENT,	CENTER_SEGMENT, LEFT_UP_SEGMENT}, //9 OK
	{LEFT_DOWN_SEGMENT,	DOWN_SEGMENT,	RIGHT_DOWN_SEGMENT,	DOT_DISPLAY_SEGMENT,		RIGHT_UP_SEGMENT,	UP_SEGMENT,	CENTER_SEGMENT, LEFT_UP_SEGMENT} //all
};



void display_init() 
{
	AT91C_BASE_PIOB->PIO_PER = 1U << MAIN_DISPLAY_SWITCH;
	AT91C_BASE_PIOB->PIO_OER = 1U << MAIN_DISPLAY_SWITCH;

	AT91C_BASE_PIOB->PIO_PER = 1U << DISPLAY_A_SWITCH;
	AT91C_BASE_PIOB->PIO_OER = 1U << DISPLAY_A_SWITCH;

	AT91C_BASE_PIOB->PIO_PER = 1U << DISPLAY_B_SWITCH;
	AT91C_BASE_PIOB->PIO_OER = 1U << DISPLAY_B_SWITCH;

	AT91C_BASE_PIOB->PIO_SODR = 1U << MAIN_DISPLAY_SWITCH;


	int i;
	for(i = 0; i < SEGMENTS_LENGTH; i++)
	{
		AT91C_BASE_PIOB->PIO_PER = segments[10][i];
		AT91C_BASE_PIOB->PIO_OER = segments[10][i];
		AT91C_BASE_PIOB->PIO_CODR = segments[10][i];
	}
}


void init_buttons()
{
	AT91C_BASE_PIOC->PIO_PER = (1U << BUTTON2) | (1U << BUTTON1);
	AT91C_BASE_PIOC->PIO_ODR = (1U << BUTTON2) | (1U << BUTTON1);
	AT91C_BASE_PIOC->PIO_PPUER = (1U << BUTTON2) | (1U << BUTTON1);
}



void GPIO_init()
{
	AT91C_BASE_PMC->PMC_PCER =  (1U << GPIOCPID);
	init_buttons();
}

void timer_setup(uint32_t time)
{
	if(time > MAX_TIMMER_VALUE)
	{
		time = MAX_TIMMER_VALUE;
	}
	AT91C_BASE_PITC->PITC_PIMR = (uint32_t)time;
	AT91C_BASE_PITC->PITC_PIMR |= (uint32_t)TIMER_IT_ENABLE;

}

void start_timer()
{
	AT91C_BASE_PITC->PITC_PIMR |= (uint32_t)TIMER_SWITCH;
	AT91C_BASE_PITC->PITC_PIMR |= (uint32_t)TIMER_IT_ENABLE;
}

void stop_timer()
{
	AT91C_BASE_PITC->PITC_PIMR &= ~((uint32_t)1 << 24);
}

uint32_t read_timer_value()
{
	return (AT91C_BASE_PITC->PITC_PIIR & (~((uint32_t)4095 << 20)));
}

uint32_t read_timer_spins_value()
{
	return (AT91C_BASE_PITC->PITC_PIIR >> 20);
}

uint32_t check_timer_state()
{
	return ((uint32_t)1 & AT91C_BASE_PITC->PITC_PISR);
}

void reset_timer()
{
	volatile uint32_t tmp = AT91C_BASE_PITC->PITC_PIVR;
}



int read_left_buton()
{
	return (!(AT91C_BASE_PIOC->PIO_PDSR & (1U << BUTTON2)));
}

int read_right_buton()
{
	return (!(AT91C_BASE_PIOC->PIO_PDSR & (1U << BUTTON1)));
}


void clear_display_buffer()
{
	int i;
	for(i = 0; i < SEGMENTS_LENGTH; i++)
	{
		AT91C_BASE_PIOB->PIO_CODR = segments[10][i];
	}
}


void shut_down_displays()
{
// #######Turns off both (A,B) switches #######

	AT91C_BASE_PIOB->PIO_SODR = 1U << DISPLAY_A_SWITCH;
	AT91C_BASE_PIOB->PIO_SODR = 1U << DISPLAY_B_SWITCH;
}


void turn_on_left_display()
{
// #######Turns on left display switch #######
	AT91C_BASE_PIOB->PIO_CODR = 1U << DISPLAY_B_SWITCH;
}

void turn_on_right_display()
{
// #######Turns on left display switch #######
	AT91C_BASE_PIOB->PIO_CODR = 1U << DISPLAY_A_SWITCH;
}


void display_number(uint32_t number)
{
	int i;

	uint32_t left_display = (uint32_t)number / 10;
	uint32_t right_display = (uint32_t)number % 10;

	shut_down_displays();
	clear_display_buffer();


//########Left display management #######


	for(i = 0; i < SEGMENTS_LENGTH; i++)
	{
		AT91C_BASE_PIOB->PIO_SODR = segments[left_display][i];
	}

	turn_on_left_display();		

	delay(5);

	shut_down_displays();

	clear_display_buffer();

//########Right display management #######

	for(i = 0; i < SEGMENTS_LENGTH; i++)
	{
		AT91C_BASE_PIOB->PIO_SODR = segments[right_display][i];
	}

	turn_on_right_display();		

	delay(5);

	shut_down_displays();
	
}

void AIC_timmer_it_enable()
{
		AT91C_BASE_AIC->AIC_IECR = 1U << 1;
}

void AIC_timmer_it_disable()
{
		AT91C_BASE_AIC->AIC_IDCR = 1U << 1;
}



void timer_callback()
{

	stop_timer();
	if(check_timer_state())
	{
		AIC_timmer_it_disable();
		reset_timer();
		++timer_spins;		
		AIC_timmer_it_enable();
	}
	AT91C_BASE_AIC->AIC_ICCR = 1U << 1;
	start_timer();


}

void AIC_setup()
{
		AIC_timmer_it_disable();
	 	AT91C_BASE_AIC->AIC_SMR[1] = (uint32_t)(PRIORITY_HIGH | IT_RISING);
		AT91C_BASE_AIC->AIC_SVR[1] = (uint32_t)timer_callback;
		AT91C_BASE_AIC->AIC_ICCR = 0xFFFFFFFF;

}

int main(void)
{
	GPIO_init();
	display_init();
	AIC_setup();
	timer_setup(100000);
	reset_timer();
	AIC_timmer_it_enable();
	start_timer();
	
	while(1)
	{
		if(timer_spins > 62)
		{
			if(read_left_buton())
			  	{
					if(displayNumber == 0)
						{
							displayNumber = 100;
						}
						--displayNumber;
			  	}
				else
				{
					++displayNumber;
				}
				if(displayNumber > 99)
				{
					displayNumber = 0;
				}
			timer_spins = 0;
		}
		display_number(displayNumber);
		if(read_right_buton())
		{
			displayNumber = 0;
		}
	}


}



