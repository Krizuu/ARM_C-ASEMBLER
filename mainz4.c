#include"AT91SAM9263.h"
#include"AT91SAM9263.h"

#define PIOB_PER (volatile unsigned int * const) 0xFFFFF400
#define PIOB_PDR (volatile unsigned int * const) 0xFFFFF404
#define PIOB_OER (volatile unsigned int * const) 0xFFFFF410
#define PIOB_ODR (volatile unsigned int * const) 0xFFFFF414
#define PIOB_SODR (volatile unsigned int * const) 0xFFFFF430
#define PIOB_CODR (volatile unsigned int * const) 0xFFFFF434

#define PIOC_PER (volatile unsigned int * const) 0xFFFFF600
#define PIOC_PDR (volatile unsigned int * const) 0xFFFFF604
#define PIOC_OER (volatile unsigned int * const) 0xFFFFF610
#define PIOC_ODR (volatile unsigned int * const) 0xFFFFF614
#define PIOC_SODR (volatile unsigned int * const) 0xFFFFF630
#define PIOC_CODR (volatile unsigned int * const) 0xFFFFF634
#define PIOC_PUER (volatile unsigned int * const) 0xFFFFF664
#define PIOC_PDSR (volatile unsigned int * const) 0xFFFFF63C
#define PMC_PCER (volatile unsigned int * const) 0xFFFFFC10

#define CDE_CLOCK (1U<<4)
#define LED1 (1U<<8)
#define LED2 (1U<<29)
#define PB1 (1U<<5)
#define PB2 (1U<<4)



#define DISP_SET 0x7FF00000
#define LDIGIT (1U<<28)
#define RDIGIT (1U<<30)

#define SE_A (1U<<25)
#define SE_B (1U<<24)
#define SE_C (1U<<22)
#define SE_D (1U<<21)
#define SE_E (1U<<20)
#define SE_F (1U<<27)
#define SE_G (1U<<26)
#define SE_ALL SE_A |SE_B |SE_C |SE_D |SE_E |SE_F |SE_G;


#define BUT2_PRESSED (*PIOC_PDSR & PB2)==0

#define PIT_MR (volatile unsigned int * const) 0xFFFFFD30
#define PIT_SR (volatile unsigned int * const) 0xFFFFFD34
#define PIT_PIVR (volatile unsigned int * const) 0xFFFFFD38
#define PIT_PIIR (volatile unsigned int * const) 0xFFFFFD3C
#define PITEN (1U<<24)
#define PITIEN (1U<<25)
#define PICNT_MASK 0xFFF00000
volatile int int_counter=0;
void dbgu_print_ascii(const char* string){}
void initLED(){
	*PIOB_PER = LDIGIT | RDIGIT | SE_ALL; 
	*PIOB_OER = LDIGIT | RDIGIT | SE_ALL;
	*PIOB_CODR= SE_ALL;
	*PIOB_SODR= LDIGIT | RDIGIT ;

}
void INThandler(void){
	int_counter++;
	AT91C_BASE_PITC->PITC_PIVR;
}
void initPIT(){
	AT91C_BASE_PITC->PITC_PIMR=(AT91C_PITC_PITIEN | 6249);
	AT91C_BASE_PITC->PITC_PIVR;
}
void initAIC(){
	//Wyłączenie przerwania od Timera PIT na czas konfiguracji 
	AT91C_BASE_AIC->AIC_IDCR = 1 << AT91C_ID_SYS;
	//Ustawienie wskaźnika do procedury obsługującej przerwanie dla urządzeń peryferyjnych w tablicy wektorów 		AIC_SVR (AIC_SVR[AT91C_ID_SYS])
	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = (unsigned int)INThandler;
	//Konfiguracja poziomu i metody wyzwalania przerwania
	AT91C_BASE_AIC->AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_PRIOR_HIGHEST | AT91C_AIC_SRCTYPE_EXT_POSITIVE_EDGE;
	//Wyczyszczenie flagi przerwania urządzeń peryferyjnych 
	AT91C_BASE_AIC->AIC_ICCR = 1 << AT91C_ID_SYS;
	//Włącznie przerwania urządzeń peryferyjnych AT91C_ID_SYS
	AT91C_BASE_AIC->AIC_IECR = 1 << AT91C_ID_SYS;
}
int * initNUMS(int nums[10]){
	nums[0]=SE_A | SE_B |SE_C |SE_D |SE_E |SE_F;
	nums[1]=SE_B |SE_C;
	nums[2]=SE_A |SE_B |SE_G |SE_E |SE_D;
	nums[3]=SE_A |SE_B |SE_G |SE_C |SE_D;
	nums[4]=SE_F |SE_G |SE_B |SE_C;
	nums[5]=SE_A |SE_F |SE_G |SE_C |SE_D;
	nums[6]=SE_A |SE_C |SE_D |SE_E |SE_F |SE_G;
	nums[7]=SE_A |SE_B |SE_C;
	nums[8]=SE_A |SE_B |SE_C |SE_D |SE_E |SE_F |SE_G;
	nums[9]=SE_A |SE_B |SE_C |SE_D |SE_G |SE_F;
	return nums;
}
void initPB(){

	*PIOC_PER = PB2;
	*PIOC_ODR = PB2;
	*PIOC_PUER = PB2;

	*PMC_PCER = CDE_CLOCK; 
}
void delay(int x){
	volatile int i;
	for(i=0; i<x*1300; i++);
}
void clearPIT(){
	*PIT_PIVR;
}
void delayPIT(int ms){
	int_counter=0;
	AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITEN;
	while(int_counter< ms) {}
	AT91C_BASE_PITC->PITC_PIMR &= ~AT91C_PITC_PITEN;
	return;
}


int main(){
	initAIC();
	initLED();
	initPB();
	initPIT();
	int nums[10];
	initNUMS(nums);
	volatile int unit_count;
	volatile int dozens_count=-1;
	volatile int delay_counter=0;
	while(1){
		
		for(unit_count=0;unit_count<10;){
			
			if(BUT2_PRESSED){
					
					if(unit_count<0){dozens_count--; unit_count=9;}
					if(dozens_count<0) dozens_count=9;
					
			}else{
					
					if(unit_count==0)dozens_count++;
					if(unit_count==0 && dozens_count==10)dozens_count=0;
					

			}

			while(1){
				if(delay_counter==100){delay_counter=0;break;}
				else{
		
				*PIOB_CODR=RDIGIT;
				*PIOB_SODR= nums[unit_count];
					delayPIT(10);
					delay_counter++;
				*PIOB_CODR= nums[unit_count];
				*PIOB_SODR= RDIGIT;		

				*PIOB_CODR= LDIGIT;
				*PIOB_SODR= nums[dozens_count];
					delayPIT(10);
					delay_counter++;
				*PIOB_CODR= nums[dozens_count];
				*PIOB_SODR= LDIGIT;
				}
			}

			if(BUT2_PRESSED){
				unit_count--;
			}else{
				unit_count++;
			}
			
		}			
	}
	
while(1);
return 0;
}
