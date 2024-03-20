#ifndef TIMER_COUNT
#define TIMER_COUNT

#include "sys.h"

extern u8 count_over;
void TIM5_Int_Init(u16 ms);
#endif
