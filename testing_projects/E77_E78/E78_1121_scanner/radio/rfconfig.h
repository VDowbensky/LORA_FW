#ifndef _RFCONFIG_H_
#define _RFCONFIG_H_

#include "bsp.h"
#include "sx126x_config.h"
#include "lr112x_config.h"

#define LB_FREQ_MIN	50000000UL
#define LB_FREQ_MAX	6000000000UL

#define HB_FREQ_MIN	600000000UL
#define HB_FREQ_MAX	2700000000UL

#define FREQ_STEP_MIN		10000UL 
#define FREQ_STEP_MAX		1000000UL

#define RSSI_TR_MIN			-126
#define RSSI_TR_MAX			-40

typedef struct __attribute__((packed)) __attribute__((aligned(4)))
{
	uint32_t magicnumber;
	SX126Xconfig_t LBconfig;
	LR112Xconfig_t HBconfig;
}globalrfconfig_t;

extern globalrfconfig_t globalrfconfig;



#endif