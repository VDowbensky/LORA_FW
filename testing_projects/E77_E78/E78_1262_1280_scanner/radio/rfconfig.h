#ifndef _RFCONFIG_H_
#define _RFCONFIG_H_

#include "bsp.h"
#include "SX126x_config.h"
#include "SX128x_config.h"

#define LB_FREQ_MIN	50000000UL
#define LB_FREQ_MAX	600000000UL

#define MB_FREQ_MIN	600000000UL
#define MB_FREQ_MAX	1350000000UL

#define HB_FREQ_MIN	2100000000UL
#define HB_FREQ_MAX	2700000000UL

#define FREQ_STEP_MIN		10000UL 
#define FREQ_STEP_MAX		1000000UL

#define RSSI_TR_MIN			-126
#define RSSI_TR_MAX			-40

#define HB_RSSI_DELTA				-7

extern const uint32_t LBrssifreqs[];
extern const uint32_t MBrssifreqs[];
extern const uint32_t HBrssifreqs[];

extern const float LBrssideltas[];
extern const float MBrssideltas[];
extern const float HBrssideltas[];

typedef struct
{
	uint32_t magicnumber;
	SX126Xconfig_t LBconfig;
	SX126Xconfig_t MBconfig;
	SX128Xconfig_t HBconfig;
}globalrfconfig_t;

extern globalrfconfig_t globalrfconfig;



#endif