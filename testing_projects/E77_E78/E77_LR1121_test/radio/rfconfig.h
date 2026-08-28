#ifndef _RFCONFIG_H_
#define _RFCONFIG_H_

#include "bsp.h"
#include "sx126x_config.h"
#include "lr112x_config.h"

typedef struct __attribute__((packed)) __attribute__((aligned(4)))
{
	uint32_t magicnumber;
	SX126Xconfig_t LBconfig;
	LR112Xconfig_t HBconfig;
}globalrfconfig_t;

extern globalrfconfig_t globalrfconfig;



#endif
