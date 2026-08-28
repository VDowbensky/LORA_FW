#ifndef _LR112X_CONFIG_H_
#define _LR112X_CONFIG_H_

#include "bsp.h"

#define LR112X_SEPARATION_FREQ 950000000UL

typedef struct __attribute__((packed)) __attribute__((aligned(4)))
{
  uint8_t modem;

  uint32_t rffreq;
  int8_t txpower_subg;
	int8_t txpower_hf;
  uint8_t ramptime;
  //LoRa
  uint8_t sf; //spreading factor
  uint8_t bw; //bandwidth
  uint8_t cr; //coding rate
  bool ldropt;
  uint16_t loraprelen;
  bool implheader;
  uint8_t lorasync;
  uint8_t lorapaylen;
  bool loracrc;
  bool invertiq;
  //FSK parameters
  uint32_t br; //baud rate
  uint8_t rbw;
  uint8_t shaping;
  uint32_t fdev;
  uint16_t fskprelen;
	uint8_t predetlen;
  uint8_t fsksynclen;
  uint8_t fsksync[8];
  uint8_t addrcomp;
  uint8_t nodeaddr;
  uint8_t braddr; 
  bool varlen;
  uint8_t fskpaylen;
  uint8_t crctype;
  uint16_t crcinit;
  uint16_t crcpoly;
  uint8_t white;
  uint8_t whiteinit;
	float K_freq;
}LR112Xconfig_t;

void LR112X_setopmode(uint8_t lr,uint8_t mode);
void LR112X_config(uint8_t lr,LR112Xconfig_t *config);
void LR112X_RssiCal(uint8_t lr,uint32_t freq);

#endif
