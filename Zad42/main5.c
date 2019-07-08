#define PIOB_PER (volatile unsigned int * const)  0xFFFFF400
#define PIOB_OER (volatile unsigned int * const) 0xFFFFF410
#define PIOB_SODR (volatile unsigned int * const) 0xFFFFF430
#define PIOB_CODR (volatile unsigned int * const) 0xFFFFF434

#define PIOC_PER (volatile unsigned int * const)  0xFFFFF600
#define PIOC_OER (volatile unsigned int * const) 0xFFFFF610
#define PIOC_ODR (volatile unsigned int * const) 0xFFFFF614
#define PIOC_SODR (volatile unsigned int * const) 0xFFFFF630
#define PIOC_CODR (volatile unsigned int * const) 0xFFFFF634
#define PIOC_PUER (volatile unsigned int * const) 0xFFFFF664
#define PIOC_PDSR (volatile unsigned int * const) 0xFFFFF63C

#define PMC_PCER (volatile unsigned int * const) 0xFFFFFC10

#define PRAWY_SEG (1U << 28)
#define LEWY_SEG (1U << 30)
#define WYSWIETLACZ (1U << 29)

#define GORNY_SEG (1U << 25)
#define PRAWY_GORNY_SEG (1U << 24)
#define PRAWY_DOLNY_SEG (1U << 22)
#define DOLNY_SEG (1U << 21)
#define LEWY_DOLNY_SEG (1U << 20)
#define LEWY_GORNY_SEG (1U << 27)
#define SRODKOWY_SEG (1U << 26)
#define KROPKA_SEG (1U << 23)
#define CALY_SEGMENT (GORNY_SEG | PRAWY_GORNY_SEG | PRAWY_DOLNY_SEG | DOLNY_SEG | LEWY_DOLNY_SEG | LEWY_GORNY_SEGF | SRODKOWY_SEG | KROPKA_SEG)

#define PRZYCISK2 4
#define PRZYCISK1 5

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
	*PIOB_PER = CALY_SEGMENT | LEWY_SEG | PRAWY_SEG | WYSWIETLACZ;
	*PIOB_OER = CALY_SEGMENT | LEWY_SEG | PRAWY_SEG | WYSWIETLACZ;
	*PIOB_CODR = PRAWY_SEG | CALY_SEGMENT;	
	*PIOB_SODR = LEWY_SEG | WYSWIETLACZ; 

}

void delay(int delay_ms)
{
	volatile int i;
	for(i=0;i<2900*delay_ms;++i){};
	
}

void rozdziel(unsigned int* SEGMENT1, unsigned int* SEGMENT2, int liczba) {
  int i = liczba % 10;  //reszta z dzielenia przez 10
  liczba /= 10;
  int j = liczba % 10;
  *SEGMENT2 = DZIESIEC_LICZB[i];
  *SEGMENT1 = DZIESIEC_LICZB[j];
}

void initPRZYCISKI()
{
	*PIOC_PER = (1U << PRZYCISK2) | (1U << PRZYCISK1);
        *PIOC_PUER = (1U << PRZYCISK2) | (1U << PRZYCISK1);
        *PIOC_PDSR = (1U << PRZYCISK2) | (1U << PRZYCISK1);
        *PIOC_ODR = (1U << PRZYCISK2) | (1U << PRZYCISK1);
}

int read_button()
{
        return(!(*PIOC_PDSR & (1U << PRZYCISK1)));
}

void liczniki(int a, int licznik)
{       
        volatile int i = 0;
        volatile int SEGMENT1 = 0, SEGMENT2 = 0;
        if(licznik + a > 99)
        {
                licznik = -1;
        }
        if(licznik + a < 0)
        {
                licznik = 100;
        }
	licznik = licznik + a;
	rozdziel(&SEGMENT1,&SEGMENT2, licznik);
	for(i=0; i<50;++i)
		{
		*PIOB_SODR = SEGMENT1 | LEWY_SEG;
		delay(5);			
		*PIOB_CODR = CALY_SEGMENT | LEWY_SEG;	
		*PIOB_SODR = SEGMENT2 | PRAWY_SEG;
		delay(5);
		*PIOB_CODR = CALY_SEGMENT | PRAWY_SEG;
		}               
}

int main(void)
{
        initPRZYCISKI();
	initWYSWIETLACZ();
	volatile int licznik = 0;
	while(1){
	        if(read_button())
	        {
                        liczniki(-1, licznik);
                        if(licznik - 1 == -1)
                        {
                                licznik = 99;
                        }
                        else
                        {
                                licznik = licznik - 1;
                        }
	        }
	        else
	        {
	                liczniki(1, licznik);
                        if(licznik + 1 == 100)
                        {
                                licznik = 0;
                        }
                        else
                        {
                                licznik = licznik + 1;
                        }
	        }
	}
}
//funkcja rozdziel rozdziela liczby bo przjmuje wskazniki, i rozbija liczby na dziesiatki i jednosci

