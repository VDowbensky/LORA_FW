#include "SX128X_config.h"
#include "sx128x.h"
#include "radio.h"

//#define SYNTH_STEP ((double)FXO / 33554432)

void SX128X_configcommon(void);

int8_t SX128X_config(void)
{
  //SX128X_setopmode(SX128X_OPMODE_STBYRC);
	SX128X_SetRegulatorMode(0,1);
	if(radioconfig.modem == MODEM_LORA)
	{
		uint8_t buf[2];
		uint8_t header;
		//uint8_t prelen;
		uint8_t crc;
		uint8_t invertiq;
		//uint8_t tmp;
		//uint8_t i;
		SX128X_SetPacketType(0,SX128X_MODEM_LORA);
		SX128X_LORA_SetModulationParams(0,radioconfig.sf << 4,radioconfig.bw,radioconfig.cr);
		//calculate preamble length patameter from radioconfig.loraprelen
		if(radioconfig.implheader == false) header = SX128X_HEADER_EXPLICIT;
		else header = SX128X_HEADER_IMPLICIT;
		if(radioconfig.loracrc == true) crc = SX128X_CRC_ENABLE;
		else crc = SX128X_CRC_DISABLE;
		if(radioconfig.invertiq == true) invertiq = SX128X_IQ_INVERTED;
		else invertiq = SX128X_IQ_STD;
		SX128X_LORA_SetPacketParams(0,radioconfig.loraprelen, header,radioconfig.lorapaylen,crc,invertiq);
    //set sync
		//It is also possible to configure the LoRa SyncWord. With the 1 byte SynchWord taking the format 0xXY, it is written across the MSB of two registers as described below:
		//write X in the register@0x944, in position [7:4] without modifying [3:0] (using a Read / Modify / Write operation)
		//write Y in the register@0x945, in position [7:4] without modifying [3:0] (using a Read / Modify / Write operation
		//SX128X_readRegs(sx,SX128X_REG_LR_SYNC_1,buf,2);
		//buf[0] &= 0x0f;
		//buf[1] &= 0x0f;
		//tmp = radioconfig.lorasync & 0xf0;
		//buf[0] |= tmp;
		//tmp = (radioconfig.lorasync & 0x0f) << 4;
		//buf[1] |= tmp;
		buf[0] = radioconfig.lorasync >> 8;
		buf[1] = radioconfig.lorasync & 0xff;
		SX128X_writeRegs(0,SX128X_REG_LR_SYNC_1, buf,2);
		SX128X_configcommon();
    return RADIO_OK;
	}
  else if (radioconfig.modem == MODEM_FSK) 
	{
		uint8_t buf[15];
		uint8_t modindex = (2 * radioconfig.br) / radioconfig.fdev;
  
		//SX128X_SetPacketType(0,SX128X_MODEM_FSK);
		SX128X_FSK_SetModulationParams(0,radioconfig.br,modindex,radioconfig.shaping);
		
		SX128X_FSK_FLRC_SetPacketParams(0,radioconfig.fskprelen,radioconfig.fsksynclen,radioconfig.syncmatch,radioconfig.varlen,radioconfig.fskpaylen,radioconfig.crctype,radioconfig.white);
		for(uint8_t i = 0; i < 15; i++) buf[i] = radioconfig.fsksync[i];
		SX128X_writeRegs(0,SX128X_REG_SYNC_1_4,buf,15);
		//SX128X_SetFskAddr(0,radioconfig.nodeaddr,radioconfig.braddr);
		buf[0] = radioconfig.whiteinit;
		buf[1] = radioconfig.crcpoly >> 8;//crc poly
		buf[2] = radioconfig.crcpoly & 0xff;
		buf[3] = radioconfig.crcinit >> 8;//crc init
		buf[4] = radioconfig.crcinit & 0xff;
		SX128X_writeRegs(0,SX128X_REG_WHITE_INIT,buf,5);
		SX128X_configcommon();
    return RADIO_OK;
	}
  else return FEATURE_NOT_SUPPORTED;
}

void SX128X_configcommon(void)
{
	//uint8_t buf[2];
	
	SX128X_SetRfFrequency(0,radioconfig.frequency0 / SX128X_SYNTH_STEP);
	SX128X_SetBufferBaseAddress(0,0, 0x80);
	SX128X_SetTxParams(0,radioconfig.txpower0,radioconfig.ramptime0);
	//SX128X_CalibrateIR();
	//SX128X_LNAboost(true); //???
	SX128X_SetDioIrqParams(0,SX128X_IRQ_TX_DONE | SX128X_IRQ_RX_DONE | SX128X_IRQ_CRC_ERROR,SX128X_IRQ_TX_DONE | SX128X_IRQ_RX_DONE,0,0);
}

void SX128X_setopmode(uint8_t mode)
{
  switch(mode)
  {
    case RADIO_OPMODE_SLEEP:
    SX128X_SetSleep(0,false);
    break;

    case RADIO_OPMODE_STBYRC:
    SX128X_SetStandby(0,0);
    break;

    case RADIO_OPMODE_STBYXOSC:
    SX128X_SetStandby(0,1);
    break;

    case RADIO_OPMODE_FS:
    SX128X_SetFs(0);
    break;

    case RADIO_OPMODE_TX:
    SX128X_SetTx(0,0,0); //temp.
    break;

    case RADIO_OPMODE_RX:
    default:
    SX128X_SetRx(0,0xff,0xffff); //temp.
    break;

    case RADIO_OPMODE_TXSTREAMCW:
    SX128X_SetCW(0);
    break;

    case RADIO_OPMODE_TXSTREAMPRE:
    SX128X_SetPRE(0);
    break;
  }
}
