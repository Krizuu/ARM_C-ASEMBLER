#include "AT91SAM9263.h"

#define PIOB AT91C_BASE_PIOB
 
#define LED1 (1<<8)
#define LED2 (1<<29)
 
#define DIGIT1 (1<<28)
#define DIGIT2 (1<<30)

#define ALLSEGMENTS A|B|C|D|E|F|G|DP
 
#define A (1<<25)
#define B (1<<24)
#define C (1<<22)
#define D (1<<21)
#define E (1<<20)
#define F (1<<27)
#define G (1<<26)
#define DP (1<<23)

#define PITC AT91C_BASE_PITC

void prepare() {
	PIOB->PIO_MDDR = LED2|DIGIT1|DIGIT2|ALLSEGMENTS;
	PIOB->PIO_OER = LED2|DIGIT1|DIGIT2|ALLSEGMENTS;
	PIOB->PIO_PER = LED2|DIGIT1|DIGIT2|ALLSEGMENTS;
	PIOB->PIO_CODR = LED2|ALLSEGMENTS;
	timer();
}

void clear() {
	PIOB->PIO_CODR = A|B|C|D|E|F|G|DP;
}

void timer() {
		PITC->PITC_PIMR = 6249;
		PITC->PITC_PIMR |= 1 << 24;
}

void delay(volatile unsigned int m) {
		PITC->PITC_PIVR;
		while (m >= (PITC->PITC_PIIR)>>20){};
}

void setNumEntity(int num) {
	PIOB->PIO_SODR = DIGIT1;
	PIOB->PIO_CODR = DIGIT2;
	clear();
	switch (num) {
		case 0: PIOB->PIO_SODR = (A|B|C|D|E|F);break;
		case 1: PIOB->PIO_SODR = (B|C);break; 
		case 2: PIOB->PIO_SODR = (A|B|G|E|D);break;
		case 3: PIOB->PIO_SODR = (A|B|G|C|D);break;
		case 4: PIOB->PIO_SODR = (F|G|B|C);break;
		case 5: PIOB->PIO_SODR = (A|F|G|C|D);break;
		case 6: PIOB->PIO_SODR = (A|F|E|D|C|G);break;
		case 7: PIOB->PIO_SODR = (A|B|C);break;
		case 8: PIOB->PIO_SODR = (A|B|C|D|E|F|G);break;
		case 9: PIOB->PIO_SODR = (A|B|C|D|F|G);break;
	}
}

void setNumDec(int num) {
	PIOB->PIO_SODR = DIGIT2;
	PIOB->PIO_CODR = DIGIT1;
	clear();
	switch (num) {
		case 0: PIOB->PIO_SODR = (A|B|C|D|E|F);break;
		case 1: PIOB->PIO_SODR = (B|C);break; 
		case 2: PIOB->PIO_SODR = (A|B|G|E|D);break;
		case 3: PIOB->PIO_SODR = (A|B|G|C|D);break;
		case 4: PIOB->PIO_SODR = (F|G|B|C);break;
		case 5: PIOB->PIO_SODR = (A|F|G|C|D);break;
		case 6: PIOB->PIO_SODR = (A|F|E|D|C|G);break;
		case 7: PIOB->PIO_SODR = (A|B|C);break;
		case 8: PIOB->PIO_SODR = (A|B|C|D|E|F|G);break;
		case 9: PIOB->PIO_SODR = (A|B|C|D|F|G);break;
	}
}

void setNum(int i) {
	setNumEntity(i % 10);
	setNumDec(i / 10);
}

int main(void) {
	prepare();
	volatile int i = 0;
	volatile int j = 0;
	while(1) {
		for (i=0;i<100;i++) {
			setNumEntity(i);
			setNumDec(j);
			delay(5);
		}
	}
}
 
void dbgu_print_ascii(const char* string){}