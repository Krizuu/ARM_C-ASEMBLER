
#define GPIOCPID 					(uint32_t) 			4

#define RECEIVER_RESET 			(uint32_t)			(1U << 2)
#define TRANSMITTER_RESET 		(uint32_t)			(1U << 3)

#define RECEIVER_ENABLE 		(uint32_t)			(1U << 4)
#define RECEIVER_DISABLE		(uint32_t)			(1U << 5)

#define TRANSMITTER_ENABLE 	(uint32_t)			(1U << 6)
#define TRANSMITTER_DISABLE 	(uint32_t)			(1U << 7)

#define RECEIVER_PIN				(uint32_t)			(1U << 30)
#define TRANSMITTER_PIN			(uint32_t)			(1U << 31)


#include <stdio.h>
#include <stdint.h>
#include "AT91SAM9263.h"
#include "AT91SAM9263-EK.h"


struct little_FIFO
{
	uint8_t data[255];
	uint16_t head;
	uint16_t tail;
}FIFO_Obj;

void FIFO_push()
{


}

void FIFO_pull()
{


}

void FIFO_Init()
{
	FIFO_Obj.head = 0;
	FIFO_Obj.tail = 0;
}

uint16_t FIFO_Size()
{
	return FIFO_Obj.head - FIFO_Obj.tail;
}





void dbgu_print_ascii()
{

}


uint32_t GET_transmitter_state()
{
	uint32_t var = (AT91C_BASE_DBGU->DBGU_CSR & (uint32_t)(1U << 1));
	return var;
}

uint32_t GET_receiver_state()
{
	return AT91C_BASE_DBGU->DBGU_CSR & 1U;
}


void GPIO_init()
{
	AT91C_BASE_PMC->PMC_PCER =  (1U << GPIOCPID);
}



void turn_receiver_on()
{
	AT91C_BASE_DBGU->DBGU_CR = RECEIVER_ENABLE;
}

void turn_receiver_off()
{
	AT91C_BASE_DBGU->DBGU_CR = RECEIVER_DISABLE;
}

void turn_transmitter_on()
{
	AT91C_BASE_DBGU->DBGU_CR = TRANSMITTER_ENABLE;
}

void turn_transmitter_off()
{
	AT91C_BASE_DBGU->DBGU_CR = TRANSMITTER_DISABLE;
}


void GDBU_transmit(uint8_t data)
{
	if(GET_transmitter_state() > 0)
	{
			AT91C_BASE_DBGU->DBGU_THR = data;
			AT91C_BASE_DBGU->DBGU_CR = TRANSMITTER_ENABLE;
			while(!GET_transmitter_state()){}
			AT91C_BASE_DBGU->DBGU_CR = TRANSMITTER_DISABLE;
	}
}


void DBGU_Init()
{
	AT91C_BASE_DBGU->DBGU_CR = RECEIVER_RESET | TRANSMITTER_RESET | (1U << 8) ;
	AT91C_BASE_DBGU->DBGU_IDR = (uint32_t) 0x1F;

	AT91C_BASE_PIOC->PIO_PDR = RECEIVER_PIN | TRANSMITTER_PIN;
	AT91C_BASE_PIOC->PIO_ASR = RECEIVER_PIN | TRANSMITTER_PIN;

	AT91C_BASE_DBGU->DBGU_BRGR = (uint16_t) 54;
	AT91C_BASE_DBGU->DBGU_MR = AT91C_US_CHMODE_NORMAL | AT91C_US_PAR_NONE;
}


//100 000 000 = 115200 (CD * 16)
//868 = 16CD


int main(void)
{
	void DBGU_Init();
	//void turn_receiver_on();
	volatile int i = 0;
	while(1)
	{
		uint8_t to_transmit = (uint8_t)'x';
		GDBU_transmit(to_transmit);
		for(i = 0; i < 1000000; i++){}

	}


}



