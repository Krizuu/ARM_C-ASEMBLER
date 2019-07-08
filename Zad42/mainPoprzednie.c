//#define PMC_PCER (volatile unsigned int * const) 0xFFFFFC10

#define LEWY_SEG (1U << 28)
#define PRAWY_SEG (1U << 30)
#define WYSWIETLACZ (1U << 29)

#define GORNY_SEG (1U << 25)
#define PRAWY_GORNY_SEG (1U << 24)
#define PRAWY_DOLNY_SEG (1U << 22)
#define DOLNY_SEG (1U << 21)
#define LEWY_DOLNY_SEG (1U << 20)
#define LEWY_GORNY_SEG (1U << 27)
#define SRODKOWY_SEG (1U << 26)
#define KROPKA_SEG (1U << 23)
#define CALY_SEGMENT (GORNY_SEG | PRAWY_GORNY_SEG | PRAWY_DOLNY_SEG | DOLNY_SEG | LEWY_DOLNY_SEG | LEWY_GORNY_SEG | SRODKOWY_SEG | KROPKA_SEG)

#define PRZYCISK2 4
#define PRZYCISK1 5

#include <stdint.h>
#define TIME (uint32_t) 2097151
#include "AT91SAM9263.h"
#include "AT91SAM9263-EK.h"
//#define PERIOD 10000;
#define IT_RISING	(uint32_t)	(1U << 5)

uint16_t  timer_spins = 0;
uint32_t  licznik = 0;

//liczby od zera do 9
const unsigned int DZIESIEC_LICZB[10] = {
	GORNY_SEG | PRAWY_GORNY_SEG | PRAWY_DOLNY_SEG | DOLNY_SEG | LEWY_DOLNY_SEG | LEWY_GORNY_SEG,
	PRAWY_GORNY_SEG | PRAWY_DOLNY_SEG,
	GORNY_SEG | PRAWY_GORNY_SEG | DOLNY_SEG | LEWY_DOLNY_SEG | SRODKOWY_SEG,
	GORNY_SEG | PRAWY_GORNY_SEG | PRAWY_DOLNY_SEG | DOLNY_SEG | SRODKOWY_SEG,
	PRAWY_GORNY_SEG | PRAWY_DOLNY_SEG | LEWY_GORNY_SEG | SRODKOWY_SEG,
	GORNY_SEG | PRAWY_DOLNY_SEG | DOLNY_SEG | LEWY_GORNY_SEG | SRODKOWY_SEG,
	GORNY_SEG | PRAWY_DOLNY_SEG | DOLNY_SEG | LEWY_DOLNY_SEG | LEWY_GORNY_SEG | SRODKOWY_SEG,
	GORNY_SEG | PRAWY_GORNY_SEG | PRAWY_DOLNY_SEG,
	GORNY_SEG | PRAWY_GORNY_SEG | PRAWY_DOLNY_SEG | DOLNY_SEG | LEWY_DOLNY_SEG | LEWY_GORNY_SEG | SRODKOWY_SEG,
	GORNY_SEG | PRAWY_GORNY_SEG | PRAWY_DOLNY_SEG | DOLNY_SEG | LEWY_GORNY_SEG | SRODKOWY_SEG,
};

//void dbgu_print_ascii(const char* string){}

void initWYSWIETLACZ()
{
	AT91C_BASE_PIOB->PIO_PER = CALY_SEGMENT | PRAWY_SEG | LEWY_SEG | WYSWIETLACZ;
	AT91C_BASE_PIOB->PIO_OER = CALY_SEGMENT | PRAWY_SEG | LEWY_SEG | WYSWIETLACZ;
	AT91C_BASE_PIOB->PIO_CODR = LEWY_SEG | CALY_SEGMENT;	
	AT91C_BASE_PIOB->PIO_SODR = PRAWY_SEG | WYSWIETLACZ; 
}

void delay(int delay_ms)
{
	volatile int i;
	for(i=0;i<2900*delay_ms;++i){};
	
}

void rozdziel(unsigned int* SEGMENT1, unsigned int* SEGMENT2, int liczba) {
  int jednosci = liczba % 10;  //reszta z dzielenia przez 10
  liczba /= 10;
  int dziesiatki = liczba % 10;
  *SEGMENT1 = DZIESIEC_LICZB[dziesiatki];
  *SEGMENT2 = DZIESIEC_LICZB[jednosci];
}

void initPRZYCISKI()
{
        AT91C_BASE_PIOC->PIO_PER = (1U << PRZYCISK2) | (1U << PRZYCISK1);
        AT91C_BASE_PIOC->PIO_PPUER = (1U << PRZYCISK2) | (1U << PRZYCISK1);
        AT91C_BASE_PIOC->PIO_PDSR = (1U << PRZYCISK2) | (1U << PRZYCISK1);
        AT91C_BASE_PIOC->PIO_ODR = (1U << PRZYCISK2) | (1U << PRZYCISK1);
	//*PIOC_PER = (1U << PRZYCISK2) | (1U << PRZYCISK1);
        //*PIOC_PUER = (1U << PRZYCISK2) | (1U << PRZYCISK1);
        //*PIOC_PDSR = (1U << PRZYCISK2) | (1U << PRZYCISK1);
        //*PIOC_ODR = (1U << PRZYCISK2) | (1U << PRZYCISK1);
}

int read_button()
{
        return(!(AT91C_BASE_PIOC->PIO_PDSR & (1U << PRZYCISK1)));
}

int read_button2()
{
        return(!(AT91C_BASE_PIOC->PIO_PDSR & (1U << PRZYCISK2)));
}

void liczbaMin(int licznik)
{       
        volatile int SEGMENT2 = 0, SEGMENT1 = 0;
	rozdziel(&SEGMENT2,&SEGMENT1,licznik);
	
	AT91C_BASE_PIOB->PIO_SODR = SEGMENT2 | PRAWY_SEG;
	delay(5);			
	AT91C_BASE_PIOB->PIO_CODR = CALY_SEGMENT | PRAWY_SEG;
	AT91C_BASE_PIOB->PIO_SODR = SEGMENT1 | LEWY_SEG;
	delay(5);
	AT91C_BASE_PIOB->PIO_CODR = CALY_SEGMENT | LEWY_SEG;
          
}


void timer(uint32_t value)
{
        if(TIME < value)    //21 bitów
        {
                value = TIME;
        }
        AT91C_BASE_PITC->PITC_PIMR = value;    //wlacza timer
        AT91C_BASE_PITC->PITC_PIMR = (uint32_t)1 << 25;
}

void start()
{
        AT91C_BASE_PITC->PITC_PIMR |= (1U << 24);    //wlacza timer
}

uint32_t read_timer()
{
        return (AT91C_BASE_PITC->PITC_PIIR >> 20); 
}

uint32_t check_timer_state()
{
	return ((uint32_t)1 & AT91C_BASE_PITC->PITC_PISR);
}

void reset()
{
        AT91C_BASE_PITC->PITC_PIVR;      //reset
        //AT91C_BASE_PITC->PITC_PIMR &= ~(1U << 24);  //wylacza timer
}

void initPIT()
{
        start();
        reset();
}

//void pit_int();

void timerPowrot()
{
        AT91C_BASE_PITC->PITC_PIMR &= ~((uint32_t)1 << 24);     //stop timer
  	if(check_timer_state())
	{
		AT91C_BASE_AIC->AIC_IDCR = 1U << 1;     //wylaczenie przerwania
		reset();
		++timer_spins;		
		AT91C_BASE_AIC->AIC_IECR = 1U << 1;     //wlaczenie przerwania
	}              
        AT91C_BASE_AIC->AIC_ICCR = 1U << 1;     //kasowane jedno przerwanie
        start();
}

void wlaczeniePrzerwania()
{
        AT91C_BASE_AIC->AIC_IECR = 1U << 1;
}

void initAIC()
{
        AT91C_BASE_AIC->AIC_IDCR = 1U << 1;     //wylaczenie przerwania
        AT91C_BASE_AIC->AIC_SMR[1] = (uint32_t) (7 | IT_RISING);
        AT91C_BASE_AIC->AIC_SVR[1] = (uint32_t) timerPowrot;
        AT91C_BASE_AIC->AIC_ICCR = 0xFFFFFFFF; //jeśli wpiszemy F to czyścimy flagi

}

int main(void)
{
        initPRZYCISKI();
	initWYSWIETLACZ();
	initAIC();
	timer(10000);
	//volatile int licznik = 0;
	wlaczeniePrzerwania();
	initPIT();
	while(1){
	        
	        //if(read_button2())
	        //{
	                //uint32_t  steps = read_timer();
	                if(timer_spins > 10)
	                {
	                        //reset();
	                        //steps = read_timer();
	                        if(read_button())
	                        {
	                                if(--licznik < 0)
	                                        licznik = 99;           
	                        }
	                        else
	                        {
	                                if(++licznik > 99)
	                                        licznik = 0;       
	                        }
	                        timer_spins = 0;
	                }
	                liczbaMin(licznik);
	     /*}
	     else
	     {
	                //uint32_t steps = read_timer();
	                if(timer_spins > 624)
	                {
	     	        //reset();fghdfgh
	                if(read_button())
	                {
	                        if(--licznik < 0)
	                                licznik = 99;
	                                
	                }
	                else
	                {
	                        if(++licznik > 99)
	                                licznik = 0;
	                        
	                }
	                timer_spins = 0;
	                }
	                liczbaMin(licznik);
	     }*/
	     
        }
}
//funkcja rozdziel rozdziela liczby bo przjmuje wskazniki, i rozbija liczby na dziesiatki i jednosci

//return(*PIT_PIIR &= (~(4095 << 20)));

