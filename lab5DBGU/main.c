#include "AT91SAM9263.h"
#include "AT91SAM9263-EK.h"

#define DRXD (1U << 30) //receiver
#define DTXD (1U << 31) //transmitter

#define BUFFERSIZE 0x6

/* FIFO buffer size and mask */
typedef struct FIFO {
	char buffer[BUFFERSIZE+1];
	int head;
	int tail;
}struktura;

void FIFO_Init(struct FIFO *Fifo){
	Fifo->head=0;
	Fifo->tail=0;
	/* optional: initialize data in buffer with 0 */
}

void FIFO_Empty(struct FIFO *Fifo){
	Fifo->head = Fifo->tail;
	/* now FIFO is empty*/
}

int FIFO_Full(struct FIFO *Fifo)
{
if((Fifo->head-Fifo->tail)==BUFFERSIZE){
return 1;}
else return 0;
}

int FIFO_Put (struct FIFO *Fifo, char Data){
	if((Fifo->tail-Fifo->head)==1 || (Fifo->tail-Fifo->head)==BUFFERSIZE){
		return -1; 
	};
	/* FIFO overflow */
	Fifo->buffer[Fifo->head] = Data;
	Fifo->head = Fifo->head++ & BUFFERSIZE;
	return 1;
	/* Put 1 byte successfully */
}

int FIFO_Get (struct FIFO *Fifo, char *Data){
	if((Fifo->head!=Fifo->tail)){
		*Data = Fifo->buffer[Fifo->tail];
		Fifo->tail = Fifo->tail++ & BUFFERSIZE;
	return 1; /* Get 1 byte successfully */
	} else return -1; /* No data in FIFO */
} 

volatile char sign;

char swapLetter(char letter)
{
                        if((letter>=(int)('A')) && (letter<=(int)('Z')))
                        {
                                letter = letter + 32;
                        }
                        else if((letter>=(int)('a')) && (letter <=(int)('z')))
                        {
                                letter = letter - 32;
                        }
        return letter;
}

void dbgu_print_ascii (char Data)
{
while (!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY)){};
AT91C_BASE_DBGU->DBGU_THR = Data;
}

/*void dbgu_print_ascii2 (char *Data)
{
//if(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY)){};
AT91C_BASE_DBGU->DBGU_THR = *Data;
}*/

void dbgu_read_ascii (char *Data){
while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY)){};
if(((int)AT91C_BASE_DBGU->DBGU_RHR)==13)
{
*Data=((int)AT91C_BASE_DBGU->DBGU_RHR);
}
else
{
*Data=swapLetter((char)AT91C_BASE_DBGU->DBGU_RHR);
} 	
}

void initDBGU()
{
	AT91C_BASE_DBGU->DBGU_IDR = AT91C_US_TXRDY | AT91C_US_RXRDY; //disable receiver and transmitter interrupt
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTTX | AT91C_US_TXDIS; //reset and stop transmitter
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RXDIS; //reset and stop receiver
	AT91C_BASE_PIOC->PIO_PDR = DRXD | DTXD;
	AT91C_BASE_PIOC->PIO_ASR = DRXD | DTXD;
	AT91C_BASE_DBGU->DBGU_BRGR = 54;  //MCK/(CD*16); 100MHZ/(115200*16) = 54;
	AT91C_BASE_DBGU->DBGU_MR = AT91C_US_CHMODE_NORMAL | AT91C_US_PAR_NONE; // chmode_normal = (0 << 14)|(0 << 15) , par_none = (1<<11)
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN | AT91C_US_TXEN; // enable receiver and transmitter
	
}

int main(void)
{
	
	struct FIFO Fifo;
	char putFifo;
	char getFifo;
	//char blad[] = "Przepelnienie";
	initDBGU();
	FIFO_Init(&Fifo);
	while(1){
	dbgu_read_ascii(&putFifo);
	FIFO_Put(&Fifo, putFifo);
	if(putFifo==13)
	{
	        //dbgu_print_ascii2(blad);
        	if(FIFO_Full(&Fifo))
        	{
	        	while(FIFO_Get(&Fifo, &getFifo) == 1)
	        	{
	        		dbgu_print_ascii(getFifo);		
	        	}	
	        }
	}
	}
	return 0;
}






