#ifndef _ADC_H_
#define _ADC_H_

#include "bsp.h"

void myadc_init(void);
void vbatt_meas_enable(void);
void vbatt_meas_disable(void);
void kickADC(void);

extern float Vcc;
extern float T;

#endif
