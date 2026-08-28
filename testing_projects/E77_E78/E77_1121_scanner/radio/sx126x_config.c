#include "SX126X_config.h"
#include "sx126x.h"
#include "radio.h"


void SX126X_configcommon(uint8_t sx,SX126Xconfig_t *config);

void SX126X_config(uint8_t sx,SX126Xconfig_t *config)
{
  //SX126X_setopmode(SX126X_OPMODE_STBYRC);
	SX126X_SetPacketType(sx,config->modem);
	if(config->modem == SX126X_MODEM_LORA)
	{
		SX126X_SetLoRaModParams(sx,config->sf,config->bw,config->cr);
		SX126X_SetLoRaPacketParams(sx,config->loraprelen, config->header,config->lorapaylen,config->loracrc,config->invertiq);
		//set sync
		//It is also possible to configure the LoRa SyncWord. With the 1 byte SynchWord taking the format 0xXY, it is written across the MSB of two registers as described below:
		//write X in the register@0x944, in position [7:4] without modifying [3:0] (using a Read / Modify / Write operation)
		//write Y in the register@0x945, in position [7:4] without modifying [3:0] (using a Read / Modify / Write operation
		//SX126X_readRegs(sx,SX126X_REG_LRSYNC_H,buf,2);
		//buf[0] &= 0x0f;
		//buf[1] &= 0x0f;
		//tmp = config->lorasync & 0xf0;
		//buf[0] |= tmp;
		//tmp = (config->lorasync & 0x0f) << 4;
		//buf[1] |= tmp;
		SX126X_writeReg(sx,SX126X_REG_LRSYNC_H,(config->lorasync & 0xff00) >> 8);
		SX126X_writeReg(sx,SX126X_REG_LRSYNC_L,config->lorasync & 0xff);
		SX126X_configcommon(sx,config);
	}
  else if (config->modem == SX126X_MODEM_FSK) 
	{
		//uint8_t sw[8];
  
		SX126X_SetPacketType(sx,SX126X_MODEM_FSK);
		SX126X_SetFskModParams(sx,config->br,config->shaping,config->rbw,config->fdev);
		SX126X_SetFskPacketParams(sx,config->fskprelen,config->predetlen,config->fsksynclen,config->addrcomp,
                            config->varlen,config->fskpaylen,config->crctype,config->white);
		//for(uint8_t i = 0; i < 8; i++) sw[i] = config->fsksync[i];
		//SX126X_SetFskSyncWord(sx,sw);
		SX126X_SetFskSyncWord(sx,config->fsksync);
		SX126X_SetFskAddr(sx,config->nodeaddr,config->braddr);
		SX126X_SetFskCrcWhitening(sx,config->crcinit,config->crcpoly,config->whiteinit);
		SX126X_configcommon(sx,config);
	}
}

void SX126X_configcommon(uint8_t sx,SX126Xconfig_t *config)
{
	SX126X_SetRfFrequency(sx,(uint32_t)(config->startfreq / SX126X_SYNTH_STEP));
	SX126X_SetBufferBaseAddress(sx,0, 0x80);
	SX126X_SetPaConfig(sx,4, 7, false);
  SX126X_writeReg(sx,SX126X_REG_OCPCONFIG, 0x38);// 140 mA; current max 160mA for the whole device
  SX126X_SetTxParams(sx,config->txpower,config->ramptime);
	//SX126X_CalibrateIR();
	SX126X_LNAboost(sx,true); //???
	SX126X_SetDIO2AsRfSwitchCtrl(sx,true);
	SX126X_SetDioIrqParams(sx,SX126X_TXDONE_IRQMSK | SX126X_RXDONE_IRQMSK | SX126X_CRCERR_IRQMSK,SX126X_TXDONE_IRQMSK | SX126X_RXDONE_IRQMSK,0,0);
}

void SX126X_setopmode(uint8_t sx,uint8_t mode)
{
  switch(mode)
  {
    case RADIO_OPMODE_SLEEP:
    SX126X_SetSleep(sx,0,false);
    break;

    case RADIO_OPMODE_STBYRC:
    SX126X_SetStandby(sx,0);
    break;

    case RADIO_OPMODE_STBYXOSC:
    SX126X_SetStandby(sx,1);
    break;

    case RADIO_OPMODE_FS:
    SX126X_SetFs(sx);
    break;

    case RADIO_OPMODE_TX:
    SX126X_SetTx(sx,0xffffff); //temp.
    break;

    case RADIO_OPMODE_RX:
    default:
    SX126X_SetRx(sx,0); //temp.
    break;

    case RADIO_OPMODE_TXSTREAMCW:
    SX126X_SetCW(sx);
    break;

    case RADIO_OPMODE_TXSTREAMPRE:
    SX126X_SetTxInfinitePreamble(sx);
    break;
  }
}
