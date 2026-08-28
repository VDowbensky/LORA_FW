#ifndef _RFCONFIG_H_
#define _RFCONFIG_H_

#include "bsp.h"
#include "SX126x_config.h"
#include "SX128x_config.h"

/*
typedef struct rfconfig
{
  uint8_t modem;

  uint32_t rffreq;
  int8_t txpower;
  uint8_t ramptime;
  //LoRa
  uint8_t sf; //spreading factor
  uint8_t bw; //bandwidth
  uint8_t cr; //coding rate
  bool opt;
  uint16_t loraprelen;
  bool header;
  uint16_t lorasync;
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
  uint8_t fsksync[16];//SX1280 needs 15 bytes
  uint8_t addrcomp;
	uint8_t syncmatch; //for SV1280 only
  uint8_t nodeaddr;
  uint8_t braddr; 
  bool varlen;
  uint8_t fskpaylen;
  uint8_t crctype;
  uint16_t crcinit;
  uint16_t crcpoly;
  bool white;
  uint8_t whiteinit;
}rfconfig_t;
*/

typedef struct
{
	uint32_t magicnumber;
	SX126Xconfig_t LBconfig;
	SX126Xconfig_t MBconfig;
	SX128Xconfig_t HBconfig;
}globalrfconfig_t;

extern globalrfconfig_t globalrfconfig;



#endif