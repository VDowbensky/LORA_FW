#include "lr112x_config.h"
#include "lr112x.h"
#include "radio.h"


//#define K_FREQ	1.0000051f //-6.8 ppm
//#define K_FREQ	1.0f

void LR112X_printerrors(uint8_t lr);
void LR112X_printstatus(uint8_t lr);
void LR112X_configcommon(uint8_t lr,LR112Xconfig_t *config);

#if _RSSI_DEBUG_MODE_
const LR112X_RssiCalibParams_t calib_0_600 =     {0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const LR112X_RssiCalibParams_t calib_600_2000 =  {0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const LR112X_RssiCalibParams_t calib_2000_2700 = {0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#else
const LR112X_RssiCalibParams_t calib_0_600 =     {12,12,14, 0, 1, 3, 4, 4, 3, 6, 6, 6, 6, 6, 6, 6, 6, 0};
const LR112X_RssiCalibParams_t calib_600_2000 =  { 2, 2, 2, 3, 3, 4, 5, 4, 4, 6, 5, 5, 6, 6, 6, 7, 6, 0};
const LR112X_RssiCalibParams_t calib_2000_2700 = { 6, 7, 6, 4, 3, 4,14,12,14,12,12,12,12, 8, 8, 9, 9, 2030};
#endif



void LR112X_config(uint8_t lr,LR112Xconfig_t *config)
{
	delay_ms(300);
	//LR112X_Wakeup(lr);
	LR112X_SetRegMode(lr,true);
	LR112X_SetStandby(lr,0);
	delay_ms(10);
	LR112X_SetDioAsRfSwitch(lr,LR112X_RFSW_ENABLE,LR112X_RFSW_STBY,LR112X_RFSW_RX,LR112X_RFSW_SUBG_TX,LR112X_RFSW_SUBG_TX_HP,LR112X_RFSW_HF_TX,LR112X_RFSW_GNSS,LR112X_RFSW_HF_RX);
	LR112X_ConfigLfClock(lr,LR112X_LF_CLK_RC,true);
	//LR112X_SetTcxoMode(lr,TCXO_3V0,100000); //no TCXO
	LR112X_ClearErrors(lr);
	LR112X_Calibrate(lr,0x3f);

	LR112X_ClearErrors(lr);
	LR112X_ClearIrq(lr,LR112X_IRQMASK_ALL);
	//lr11xx_system_get_version(NULL, &version);
	LR112X_SetStandby(0,1);
	LR112X_SetPacketType(lr,config->modem);
	if(config->modem == LR112X_MODEM_LORA)
	{
		LR112X_SetLoRaModulationParams(lr,config->sf,config->bw,config->cr,config->ldropt);
		LR112X_SetLoRaPacketParams(lr,config->loraprelen, config->implheader,config->lorapaylen,config->loracrc,config->invertiq);
		LR112X_SetLoRaSyncWord(lr, config->lorasync);
		LR112X_configcommon(lr,config);
	}
  else if (config->modem == LR112X_MODEM_FSK) 
	{
		uint8_t sw[8];
  
		LR112X_SetPacketType(lr,LR112X_MODEM_FSK);
		LR112X_SetFskModulationParams(lr,config->br,config->shaping,config->rbw,config->fdev);
		LR112X_SetFskPacketParams(lr,config->fskprelen,config->predetlen,config->fsksynclen,config->addrcomp,
                            config->varlen,config->fskpaylen,config->crctype,config->white);
		for(uint8_t i = 0; i < 8; i++) sw[i] = config->fsksync[i];
		LR112X_SetGfskSyncWord(lr,sw);
		LR112X_SetPacketAdrs(lr,config->nodeaddr,config->braddr);
		LR112X_SetGfskCrcParams(lr,config->crcinit,config->crcpoly);
		LR112X_SetGfskWhitParams(lr,config->whiteinit);
		LR112X_configcommon(lr,config);
	}
}

void LR112X_configcommon(uint8_t lr,LR112Xconfig_t *config)
{
	//uint8_t buf[4];
	
	LR112X_SetRfFrequency(lr,(uint32_t)(config->rffreq * globalrfconfig.HBconfig.K_freq));
	//LR112X_SetBufferBaseAddress(lr,0, 0x80);
	if(globalrfconfig.HBconfig.rffreq > LR112X_SEPARATION_FREQ)
	{
		LR112X_SetPaConfig(lr,LR11XX_PA_SEL_HF,LR11XX_PA_REG_SUPPLY_VREG,LR11XX_PA_DUTYCYCLE_HF,LR11XX_PA_HPSEL_HF);
		LR112X_SetTxParams(lr,config->txpower_hf,config->ramptime);
	}
	else
	{
		LR112X_SetPaConfig(lr,LR11XX_PA_SEL_SUBG_HP,LR11XX_PA_REG_SUPPLY_VBAT,LR11XX_PA_DUTYCYCLE_SUBG,LR11XX_PA_HPSEL_SUBG);
		LR112X_SetTxParams(lr,config->txpower_subg,config->ramptime);
	}
	LR112X_Calibrate(lr,0x3f);
	//calibrate image here
	//calibrate RSSI
	LR112X_RssiCal(lr,globalrfconfig.HBconfig.rffreq);
	LR112X_SetDioIrqParams(lr,LR112X_TX_DONE | LR112X_RX_DONE | LR112X_CRC_ERROR,0);
	LR112X_SetRxTxFallbackMode(lr,LR112X_FALLBACK_STBY_XOSC);
	LR112X_SetRxBoosted(lr,true);
	//The workaround is to set the bit 4 in the register 0x00F30024 when the chip ends a reception in the 2.4GHz band before launching a GNSS scan. - ???
	//LR112X_ReadRegMem32(lr,0x00f30024,buf,1);
	//buf[0] |= 0x10;
	//LR112X_WriteRegMem32(lr,0x00f30024,buf,1);
}

void LR112X_setopmode(uint8_t lr,uint8_t mode)
{
  switch(mode)
  {
    case RADIO_OPMODE_SLEEP:
    LR112X_SetSleep(lr,LR112X_SLEEP_POWERDOWN,false);
    break;

    case RADIO_OPMODE_STBYRC:
    LR112X_SetStandby(lr,0);
    break;

    case RADIO_OPMODE_STBYXOSC:
    LR112X_SetStandby(lr,1);
    break;

    case RADIO_OPMODE_FS:
    LR112X_SetFs(lr);
    break;

    case RADIO_OPMODE_TX:
    LR112X_SetTx(lr,0xffffff); //temp.
    break;

    case RADIO_OPMODE_RX:
    default:
    LR112X_SetRx(lr,0); //temp.
    break;

    case RADIO_OPMODE_TXSTREAMCW:
    LR112X_SetTxCw(lr);
    break;

    case RADIO_OPMODE_TXSTREAMPRE:
    LR112X_SetTxInfinitePreamble(lr);
    break;
  }
	//LR112X_printerrors(0);
}

void LR112X_printerrors(uint8_t lr)
{
	printf("ERRORS = 0x%04X\r\n",LR112X_GetErrors(lr));
}

void LR112X_printstatus(uint8_t lr)
{
	uint8_t stat1,stat2;
	
	LR112X_GetStatus(0,&stat1,&stat2);
	stat2 = (stat2 >> 1) & 0x07; //stat2
	stat1 = (stat1 >> 1) & 0x07; //stat1
	printf("STATUS:%d,%d\r\n",stat2,stat1);
}

void LR112X_RssiCal(uint8_t lr,uint32_t freq)
{
	if(freq <= 60000000) LR112X_SetRssiCalibration(lr,&calib_0_600);
	else if((freq > 60000000) && (globalrfconfig.HBconfig.rffreq <= 200000000)) LR112X_SetRssiCalibration(lr,&calib_600_2000);
	else LR112X_SetRssiCalibration(lr,&calib_2000_2700);
}
