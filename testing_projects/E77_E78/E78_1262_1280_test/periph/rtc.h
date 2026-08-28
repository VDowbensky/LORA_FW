#ifndef _RCC_H_
#define _RCC_H_

#include "bsp.h"
#include "tremo_rtc.h"

void rtc_init(void);
void rtc_set(uint16_t y,uint8_t mon,uint8_t d,uint8_t h,uint8_t min,uint8_t s);

extern rtc_calendar_t timestamp;
extern bool rtc_flag;

#endif
