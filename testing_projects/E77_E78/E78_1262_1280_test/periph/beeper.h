#ifndef _BSTIMER_H_
#define _BSTIMER_H_

#include "bsp.h"
#include "tremo_bstimer.h"

void beeper_init(void);
void beep(uint16_t freq,uint16_t dur);
void beeper_on(void);
void beeper_off(void);

extern uint16_t beep_duration;
extern bool beeper;


#endif
