#include "AT91SAM9263.h"

#define CD 54
#define ODBIORNIK (1<<30)
#define NADAJNIK (1<<31)
#define BUFFERSIZE 30

char znak[20];
int licznik = 0; //Licznik znakow

struct FIFO
{
	char buffer [BUFFERSIZE+1];
	int head;
	int tail;
}STACK, *wsk=&STACK;

void FIFO_Init (struct FIFO *Fifo);
void FIFO_Empty (struct FIFO *Fifo);
int FIFO_Put (struct FIFO *Fifo, char Data);
int FIFO_Get (struct FIFO *Fifo, char *Data);
static void Open_DBGU(void);
void dbgu_print_ascii();
void dbgu_read_ascii();

static void Open_DBGU(void)
{
	AT91C_BASE_DBGU->DBGU_IDR = AT91C_US_TXRDY|AT91C_US_RXRDY; // wylacza przerwania od portu DBGU
	
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX|AT91C_US_RSTTX; // Resetuje odbiornik i nadajnik
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXDIS|AT91C_US_RXDIS; // Wlacza odbniornik i nadajnik
	
	AT91C_BASE_PIOC->PIO_ASR = NADAJNIK|ODBIORNIK; // Select A
	AT91C_BASE_PIOC->PIO_PDR = NADAJNIK|ODBIORNIK; // PIO Disable Register
	// Konfiguracja portow wejscia-wyjscia jako porty RxD i TxD DBGU
	
	AT91C_BASE_DBGU->DBGU_BRGR = CD; // Konfiguracja szybkosci transmisji portu szeregowego 
	
	AT91C_BASE_DBGU->DBGU_MR = AT91C_US_CHMODE_NORMAL|AT91C_US_PAR_NONE; 
	// Konfiguracja trybu pracy, tryb normalny bez przystosci (8N1), rejestr  flagi AT91C_US_CHMODE_NORMAL, AT91C_US_PAR_NONE

	
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN|AT91C_US_RXEN; // Wlaczam odbiornik i nadajnik
}

void dbgu_print_ascii()
{
	int i;
	for(i=0; i<20; ++i)
	{
		while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY)) {};
		AT91C_BASE_DBGU->DBGU_THR = znak[i];
		FIFO_Get(wsk,znak);
	}
	
}

void dbgu_read_ascii()
{

	if(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY)  // Channel Status Register & RXRDY Interrupt
	{
		znak = AT91C_BASE_DBGU->DBGU_RHR;
	}
	FIFO_Put(wsk,&znak);
}

void FIFO_Init (struct FIFO *Fifo) // tworzy FIFO
{
	Fifo->head=0;
	Fifo->tail=0;
    /* optional: initialize data in buffer with 0 */
}

void FIFO_Empty (struct FIFO *Fifo) // sprawdza czy FIFO jest puste, jesli tak to glowa jest ogonem
{
	Fifo->head = Fifo->tail;
}

int FIFO_Put (struct FIFO *Fifo, char Data) // wstawia element do FIFO
{
	if ((Fifo->tail-Fifo->head)==1 || (Fifo->tail-Fifo->head)==BUFFERSIZE) // sprawdza czy FIFO nie jest pelne
	{
        	return -1;
	}; /* FIFO overflow */

	Fifo->buffer[Fifo->head] = Data; // wstawia element
	Fifo->head = Fifo->head++ & BUFFERSIZE; // zwieksza wskaznik glowy

	return 1; /* Put 1 byte successfully */
}

int FIFO_Get (struct FIFO *Fifo, char *Data) // pobiera element z FIFO
{
	if ((Fifo->head!=Fifo->tail)) // sprawdza czy FIFO nie jest puste
	{
		*Data = Fifo->buffer[Fifo->tail]; // pobiera element
		Fifo->tail = Fifo->tail++ & BUFFERSIZE; // zwieksza wskaznik ogona
		return 1; /* Get 1 byte successfully */
    	}
    	
	else return -1; /* No data in FIFO */
}

int main(void)
{
	int i;

	FIFO_Init(wsk); // inicjalizuje stos
	
	for(i=0; i<20; ++i) znak[i]=' '; // wypelnia tablica pustymi polami
	Open_DBGU();

	dbgu_read_ascii();
    
	if(licznik==20)
	{
        	dbgu_print_ascii();
        	licznik=0;
	}
	
	return 0;
}