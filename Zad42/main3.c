#define PIOB_PER (volatile unsigned int * const) 0xFFFFF400
#define PIOB_OER (volatile unsigned int * const) 0xFFFFF410
#define PIOB_CODR (volatile unsigned int * const) 0xFFFFF434
#define PIOB_SODR (volatile unsigned int * const) 0xFFFFF430

#define PIOC_PER (volatile unsigned int * const) 0xFFFFF600
#define PIOC_OER (volatile unsigned int * const) 0xFFFFF610
#define PIOC_CODR (volatile unsigned int * const) 0xFFFFF634
#define PIOC_SODR (volatile unsigned int * const) 0xFFFFF630

#define PIOC_PUER (volatile unsigned int * const) 0xFFFFF664
#define PIOC_PDSR (volatile unsigned int * const) 0xFFFFF63C
#define PIOC_ODR (volatile unsigned int * const) 0xFFFFF614

#define B(b) (1 << (b))

#define S7_S1_B B(28)
#define S7_S2_B B(30)
#define S7init B(31)

#define S7E_B B(20)
#define S7D_B B(21)
#define S7C_B B(22)
#define S7DP_B B(23)
#define S7B_B B(24)
#define S7A_B B(25)
#define S7G_B B(26)
#define S7F_B B(27)

#define S7_B (S7A_B | S7B_B | S7C_B | S7D_B | S7E_B | S7F_B | S7G_B | S7DP_B)

#define S7_segs 8

//#define DIODA1 8
//#define DIODA2 29
//#define PRZYCISK1 5 
//#define PRZYCISK2 4



//void dbgu_print_ascii(){}

void delay(int j){
	volatile int i = 0;
	j = j*1500;
	for(i=0;i<j;i++){}
}
/*void wylaczDiode()
{
        *PIOC_SODR = 1U << DIODA2;
}
void zalaczaDiode()
{
        *PIOC_CODR = 1U << DIODA2; 
}
void button()
{
        int i = 0;
	for(i=0;i<10; i++)
	{
	        if(!(*PIOC_PDSR & (1U << PRZYCISK1)))
	        {
	                wylaczDiode();
	        }
	        if(!(*PIOC_PDSR & (1U << PRZYCISK2)))
	        {       
	                 zalaczaDiode();
	        }
	        
	        delay(100);
	}
}
void initDIODA()
{
        *PIOB_PER = (1U << DIODA1);
        *PIOB_OER = (1U << DIODA1);
        *PIOB_SODR = (1U << DIODA1);    //gasi diode

        *PIOC_PER = (1U << DIODA2);
        *PIOC_OER = (1U << DIODA2);
        *PIOC_SODR = (1U << DIODA2);
}
void initPRZYCISK()
{
        *PIOC_PUER = (1U << PRZYCISK2) | (1U << PRZYCISK1);
        *PIOC_PDSR = (1U << PRZYCISK2) | (1U << PRZYCISK1);
        *PIOC_ODR = (1U << PRZYCISK2) | (1U << PRZYCISK1);
}*/
void initSeg()
{       
        //S7_S1_B;
        //S7init;
        *PIOB_PER = (S7_B | S7_S1_B | S7init);
        *PIOB_OER = (S7_B | S7_S1_B | S7init);
        *PIOB_CODR = S7_B;
}

int main(){
//initDIODA();
//initPRZYCISK();

initSeg();


/*while(1){
        *PIOB_SODR = 1U << DIODA1;
        button();
        *PIOB_CODR = 1U << DIODA1;
        button();
        }*/
//while(1)
//{
//}
}

