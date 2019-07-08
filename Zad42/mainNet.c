#include "AT91SAM9263.h"
#include "AT91SAM9263-EK.h"

#define B(b) (1 << (b))

#define S7_S1_B B(28)
#define S7_S2_B B(30)

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


#define BTN1C (1 << 5)
#define BTN2C (1 << 4)
#define CLK_PCDE (1 << 4)

#define VC_LOOP 400

#define PERIOD 10000

const unsigned int S7_S_B[S7_segs] = {
    S7A_B,
    S7B_B,
    S7C_B,
    S7D_B,
    S7E_B,
    S7F_B,
    S7G_B,
    S7DP_B
};

const unsigned int S7_DGT_B[10] = { 
  S7A_B | S7B_B | S7C_B | S7D_B | S7E_B | S7F_B,
  S7B_B | S7C_B,
  S7A_B | S7B_B | S7G_B | S7E_B | S7D_B,
  S7A_B | S7B_B | S7C_B | S7D_B | S7G_B,
  S7F_B | S7G_B | S7B_B | S7C_B,
  S7A_B | S7F_B | S7G_B | S7C_B | S7D_B,
  S7A_B | S7F_B | S7G_B | S7C_B | S7D_B | S7E_B,
  S7A_B | S7B_B | S7C_B,
  S7A_B | S7B_B | S7C_B | S7D_B | S7E_B | S7F_B | S7G_B,
  S7A_B | S7B_B | S7C_B | S7D_B | S7F_B | S7G_B,
};

struct BtnState {
  unsigned int id;
  int state;
  int last_read;
};

int checkBtn(struct BtnState* btn) {
  int read = ((*AT91C_PIOC_PDSR & btn->id) == 0);
  if(btn->last_read == read) {
    btn->state = read;
  }
  btn->last_read = read;
  return btn->state;
}

#define PICNT_SHIFT 20

int checkPIT() {
  int i = 0;
  if(*AT91C_PITC_PISR & AT91C_PITC_PITS) {
    i = *AT91C_PITC_PIVR >> PICNT_SHIFT;
  }
  return i;
}

void decode2(unsigned int* s1, unsigned int* s2, int val) {
  int j = val % 10;
  val /= 10;
  int d = val % 10;
  *s2 = S7_DGT_B[j];
  *s1 = S7_DGT_B[d];
}

struct State {
  unsigned int seg1_B, seg2_B;
  int cnt;
  int cloop;
  int sh;
  struct BtnState btn;
};

struct State state = { 0 };

void pit_int();

int main(void) {
  state.btn.id = BTN1C;
  decode2(&state.seg1_B, &state.seg2_B, state.cnt);
  
  *AT91C_PIOC_PER = BTN1C;
  *AT91C_PIOC_ODR = BTN1C;
  *AT91C_PIOC_PPUER = BTN1C;
  *AT91C_PMC_PCER = CLK_PCDE;
    
  *AT91C_PIOB_PER = S7_B | S7_S1_B | S7_S2_B;
  *AT91C_PIOB_OER = S7_B | S7_S1_B | S7_S2_B;
  *AT91C_PIOB_CODR = S7_B;
  *AT91C_PIOB_SODR = S7_S1_B | S7_S2_B;
  
  *AT91C_AIC_IDCR = 1 << AT91C_ID_SYS;
  AT91C_AIC_SVR[AT91C_ID_SYS] = pit_int;
  AT91C_AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_INT_LEVEL_SENSITIVE | AT91C_AIC_PRIOR_HIGHEST;
  *AT91C_AIC_ICCR = 1 << AT91C_ID_SYS;
  *AT91C_AIC_IECR = 1 << AT91C_ID_SYS;
  
  *AT91C_PITC_PIMR = AT91C_PITC_PITIEN | AT91C_PITC_PITEN | PERIOD;
  
  while(1) {
  }
}

void pit_int() {
  int pi = checkPIT();
  int bs, dir;
  if(pi > 0) {
    if(state.sh) {
      *AT91C_PIOB_CODR = S7_S1_B | S7_B;
      *AT91C_PIOB_SODR = state.seg1_B | S7_S2_B;
      state.sh = 0;
    } else {
      *AT91C_PIOB_CODR = S7_S2_B | S7_B;
      *AT91C_PIOB_SODR = state.seg2_B | S7_S1_B;
      state.sh = 1;
    }
    ////
    bs = checkBtn(&state.btn);
    dir = bs ? -1 : 1;
    if(++state.cloop > VC_LOOP) {
      state.cnt += dir;
      if(state.cnt < 0)
	state.cnt = 99;
      else if(state.cnt > 99)
	state.cnt = 0;
      state.cloop = 0;
      decode2(&state.seg1_B, &state.seg2_B, state.cnt);
    }
  }
}

void dbgu_print_ascii(const char* string){}
