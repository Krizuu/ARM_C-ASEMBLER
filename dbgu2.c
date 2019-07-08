#include "AT91SAM9263.h"


void dbgu_print_ascii (const char *st)	{

int i=0;  
while ( st[i] != '\\0' ) {
  while ( !(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY) ){}; /* wait intil Tx buffer busy – check TXRDY flag */
    AT91C_BASE_DBGU->DBGU_THR = st[i]; /* write a single char to Transmitter Holding Register */
    i++;
  }
}


void dbgu_read_ascii (const char *Buffer, unsigned int Size){

int i=0;  
  
do {
  while ( !(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY) ){} /* wait until data available */
    Buffer[i] = AT91C_BASE_DBGU->DBGU_RHR; /* read data from Receiver Holding Register */
    i++;
  } while ( iDBGU_IDR = AT91C_US_RXRDY | AT91C_US_TXRDY | AT91C_US_ENDRX | AT91C_US_ENDTX;
  AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RXDIS;
  AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTTX | AT91C_US_TXEN;
  AT91C_BASE_PIOC->PIO_ASR = 1<<30 | 1<<31;
  AT91C_BASE_PIOC->PIO_PDR = 1<<30 | 1<<31;
  AT91C_BASE_DBGU->DBGU_BRGR = 54;
  AT91C_BASE_DBGU->DBGU_MR = AT91C_US_CHMODE_NORMAL | AT91C_US_PAR_NONE;
  AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN;  
  AT91C_BASE_DBGU->DBGU_CR = AT91C_US_TXEN;

  char *string = "as";
  //dbgu_read_ascii(&string, 20);
  dbgu_print_ascii("jeste");
  return 0;
}