#include "AT91SAM9263.h"

#define CD 54
#define ODBIORNIK (1<<30)
#define NADAJNIK (1<<31)

int main(void)
{ 
	int znak = 0;

	AT91C_BASE_DBGU->DBGU_IDR = AT91C_US_TXRDY|AT91C_US_RXRDY; // wylacza przerwania od portu DBGU

	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RSTTX; // Resetuje odbiornik i nadajnik
	AT91C_BASE_DBGU->DBGU_CR =AT91C_US_TXDIS|AT91C_US_RXDIS; // Wlacza odbniornik i nadajnik

	AT91C_BASE_PIOC->PIO_ASR = NADAJNIK|ODBIORNIK; // Select A
	AT91C_BASE_PIOC->PIO_PDR = NADAJNIK|ODBIORNIK; // PIO Disable Register
	// Konfiguracja portów wejocia-wyjocia jako porty RxD i TxD DBGU


	AT91C_BASE_DBGU->DBGU_BRGR = CD; // Konfiguracja szybkosci transmisji portu szeregowego 

	AT91C_BASE_DBGU->DBGU_MR = AT91C_US_CHMODE_NORMAL|AT91C_US_PAR_NONE; 
	// Konfiguracja trybu pracy, tryb normalny bez przystooci (8N1), rejestr  flagi AT91C_US_CHMODE_NORMAL, AT91C_US_PAR_NONE

	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN|AT91C_US_RXEN; // Wlaczam odbiornik i nadajnik
	
	while(1)
	{
		if(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY)  // Channel Status Register & RXRDY Interrupt - SPRAWDZANIE PRZEZ POOLING
		{
			znak = AT91C_BASE_DBGU->DBGU_RHR; // Receive Holding Register - zapis

			if((znak>=(int)('A')) && (znak<=(int)('Z')))
			{
				znak = znak + (int)('a'-'A');
			}
			else if((znak>=(int)('a')) && (znak <=(int)('z')))
			{
				znak = znak - (int)('a'-'A');
			}

			while(!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY)){}  // Channel Status Register & TXRDY Interrupt
			AT91C_BASE_DBGU->DBGU_THR = znak; // Transmit Holding Register - odczyt
		}
	}

	return 0;
}

void dbgu_print_ascii(const char* string){}