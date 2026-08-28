#include "SX128X_config.h"
#include "sx128x.h"
#include "radio.h"

//#define SYNTH_STEP ((double)FXO / 33554432)

void SX128X_configcommon(uint8_t sx,SX128Xconfig_t *config);

void SX128X_config(uint8_t sx,SX128Xconfig_t *config)
{
  //SX128X_setopmode(SX128X_OPMODE_STBYRC);
	SX128X_SetRegulatorMode(sx,1);
	SX128X_SetPacketType(sx,config->modem);
	if(config->modem == SX128X_MODEM_LORA)
	{
		uint8_t buf[2];
		uint8_t header;
		//uint8_t prelen;
		uint8_t crc;
		uint8_t invertiq;
		//uint8_t tmp;
		uint8_t i;
		
		SX128X_LORA_SetModulationParams(sx,config->sf << 4,config->bw,config->cr);
		//calculate preamble length patameter from config->loraprelen

		if(config->implheader == false) header = SX128X_HEADER_EXPLICIT;
		else header = SX128X_HEADER_IMPLICIT;
		if(config->loracrc == true) crc = SX128X_CRC_ENABLE;
		else crc = SX128X_CRC_DISABLE;
		if(config->invertiq == true) invertiq = SX128X_IQ_INVERTED;
		else invertiq = SX128X_IQ_STD;
		SX128X_LORA_SetPacketParams(sx,config->loraprelen, header,config->lorapaylen,crc,invertiq);
    //set sync
		//It is also possible to configure the LoRa SyncWord. With the 1 byte SynchWord taking the format 0xXY, it is written across the MSB of two registers as described below:
		//write X in the register@0x944, in position [7:4] without modifying [3:0] (using a Read / Modify / Write operation)
		//write Y in the register@0x945, in position [7:4] without modifying [3:0] (using a Read / Modify / Write operation
		//SX128X_readRegs(sx,SX128X_REG_LR_SYNC_1,buf,2);
		//buf[0] &= 0x0f;
		//buf[1] &= 0x0f;
		//tmp = config->lorasync & 0xf0;
		//buf[0] |= tmp;
		//tmp = (config->lorasync & 0x0f) << 4;
		//buf[1] |= tmp;
		buf[0] = config->lorasync >> 8;
		buf[1] = config->lorasync & 0xff;
		SX128X_writeRegs(sx,SX128X_REG_LR_SYNC_1, buf,2);
		SX128X_configcommon(sx,config);
	}
  else if (config->modem == SX128X_MODEM_FSK) 
	{
		uint8_t buf[15];
		uint8_t modindex = (2 * config->br) / config->fdev;
  
		SX128X_SetPacketType(sx,SX128X_MODEM_FSK);
		SX128X_FSK_SetModulationParams(sx,config->br,modindex,config->shaping);
		
		SX128X_FSK_FLRC_SetPacketParams(sx,config->fskprelen,config->fsksynclen,config->syncmatch,config->varlen,config->fskpaylen,config->crctype,config->white);
		for(uint8_t i = 0; i < 15; i++) buf[i] = config->fsksync[i];
		SX128X_writeRegs(sx,SX128X_REG_SYNC_1_4,buf,15);
		//SX128X_SetFskAddr(sx,config->nodeaddr,config->braddr);
		buf[0] = config->whiteinit;
		buf[1] = config->crcpoly >> 8;//crc poly
		buf[2] = config->crcpoly & 0xff;
		buf[3] = config->crcinit >> 8;//crc init
		buf[4] = config->crcinit & 0xff;
		SX128X_writeRegs(sx,SX128X_REG_WHITE_INIT,buf,5);
		SX128X_configcommon(sx,config);
	}
}

void SX128X_configcommon(uint8_t sx,SX128Xconfig_t *config)
{
	uint8_t buf[2];
	
	SX128X_SetRfFrequency(sx,config->startfreq / SX128X_SYNTH_STEP);
	SX128X_SetBufferBaseAddress(sx,0, 0x80);
	SX128X_SetTxParams(0,config->txpower,config->ramptime);
	//SX128X_CalibrateIR();
	//SX128X_LNAboost(true); //???
	SX128X_SetDioIrqParams(sx,SX128X_IRQ_TX_DONE | SX128X_IRQ_RX_DONE | SX128X_IRQ_CRC_ERROR,SX128X_IRQ_TX_DONE | SX128X_IRQ_RX_DONE,0,0);
}

void SX128X_setopmode(uint8_t sx,uint8_t mode)
{
  switch(mode)
  {
    case RADIO_OPMODE_SLEEP:
    SX128X_SetSleep(sx,false);
    break;

    case RADIO_OPMODE_STBYRC:
    SX128X_SetStandby(sx,0);
    break;

    case RADIO_OPMODE_STBYXOSC:
    SX128X_SetStandby(sx,1);
    break;

    case RADIO_OPMODE_FS:
    SX128X_SetFs(sx);
    break;

    case RADIO_OPMODE_TX:
    SX128X_SetTx(sx,0,0); //temp.
    break;

    case RADIO_OPMODE_RX:
    default:
    SX128X_SetRx(sx,0xff,0xffff); //temp.
    break;

    case RADIO_OPMODE_TXSTREAMCW:
    SX128X_SetCW(sx);
    break;

    case RADIO_OPMODE_TXSTREAMPRE:
    SX128X_SetPRE(sx);
    break;
  }
}
