#ifndef _RTC6715_DEFS_H_
#define _RTC6715_DEFS_H_

#include "bsp.h"

//Registers definitions

#define RTC6715_SYNTH_A               0x00 //R register
#define RTC6715_SYNTH_A_R_MSK         0x7fff

#define RTC6715_SYNTH_B               0x01 //N register
#define RTC6715_SYNTH_B_A_MSK         0x7f
#define RTC6715_SYNTH_B_A_POS         0x00
#define RTC6715_SYNTH_B_N_MSK         0xff80
#define RTC6715_SYNTH_B_N_POS         0x07

#define RTC6715_SYNTH_C               0x01 //synth control register
#define RTC6715_SYNTH_C_AGC6M0_MSK    


#endif
