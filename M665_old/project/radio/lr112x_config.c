#include "lr112x_config.h"
#include "lr112x.h"
#include "radio.h"


//#define K_FREQ	1.0000051f //-6.8 ppm
//#define K_FREQ	1.0f

void LR112X_printerrors(void);
void LR112X_printstatus(void);
void LR112X_configcommon(void);

#if _RSSI_DEBUG_MODE_
const LR112X_RssiCalibParams_t calib_0_600 =     {0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const LR112X_RssiCalibParams_t calib_600_2000 =  {0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const LR112X_RssiCalibParams_t calib_2000_2700 = {0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#else
const LR112X_RssiCalibParams_t calib_0_600 =     {12,12,14, 0, 1, 3, 4, 4, 3, 6, 6, 6, 6, 6, 6, 6, 6, 0};
const LR112X_RssiCalibParams_t calib_600_2000 =  { 2, 2, 2, 3, 3, 4, 5, 4, 4, 6, 5, 5, 6, 6, 6, 7, 6, 0};
const LR112X_RssiCalibParams_t calib_2000_2700 = { 6, 7, 6, 4, 3, 4,14,12,14,12,12,12,12, 8, 8, 9, 9, 2030};
#endif



int8_t LR112X_config(void)
{
	delay_ms(300);
	//LR112X_Wakeup(0);
	LR112X_SetRegMode(0,true);
	LR112X_SetStandby(0,0);
	delay_ms(10);
	LR112X_SetDioAsRfSwitch(0,LR112X_RFSW_ENABLE,LR112X_RFSW_STBY,LR112X_RFSW_RX,LR112X_RFSW_SUBG_TX,LR112X_RFSW_SUBG_TX_HP,LR112X_RFSW_HF_TX,LR112X_RFSW_GNSS,LR112X_RFSW_HF_RX);
	//LR112X_ConfigLfClock(0,LR112X_LF_CLK_RC,true);
	LR112X_ConfigLfClock(0,LR112X_LF_CLK_XOSC,true); //E80
	LR112X_SetTcxoMode(0,TCXO_1V8,320); //no TCXO
	LR112X_ClearErrors(0);
	LR112X_Calibrate(0,0x3f);

	LR112X_ClearErrors(0);
	LR112X_ClearIrq(0,LR112X_IRQMASK_ALL);
	//lr11xx_system_get_version(NULL, &version);
	LR112X_SetStandby(0,1);
	if(radioconfig.modem == MODEM_LORA)
	{
		LR112X_SetPacketType(0,LR112X_MODEM_LORA);
		LR112X_SetLoRaModulationParams(0,radioconfig.sf,radioconfig.bw,radioconfig.cr,radioconfig.ldropt);
		LR112X_SetLoRaPacketParams(0,radioconfig.loraprelen, radioconfig.implheader,radioconfig.lorapaylen,radioconfig.loracrc,radioconfig.invertiq);
		LR112X_SetLoRaSyncWord(0, radioconfig.lorasync);
		LR112X_configcommon();
    return RADIO_OK;
	}
  else if (radioconfig.modem == MODEM_FSK) 
	{
		uint8_t sw[8];
  
		LR112X_SetPacketType(0,LR112X_MODEM_FSK);
		LR112X_SetFskModulationParams(0,radioconfig.br,radioconfig.shaping,radioconfig.rbw,radioconfig.fdev);
		LR112X_SetFskPacketParams(0,radioconfig.fskprelen,radioconfig.predetlen,radioconfig.fsksynclen,radioconfig.addrcomp,
                            radioconfig.varlen,radioconfig.fskpaylen,radioconfig.crctype,radioconfig.white);
		for(uint8_t i = 0; i < 8; i++) sw[i] = radioconfig.fsksync[i];
		LR112X_SetGfskSyncWord(0,sw);
		LR112X_SetPacketAdrs(0,radioconfig.nodeaddr,radioconfig.braddr);
		LR112X_SetGfskCrcParams(0,radioconfig.crcinit,radioconfig.crcpoly);
		LR112X_SetGfskWhitParams(0,radioconfig.whiteinit);
		LR112X_configcommon();
    return RADIO_OK;
	}
  else return FEATURE_NOT_SUPPORTED;
}

void LR112X_configcommon(void)
{
	//uint8_t buf[4];
	
	//LR112X_SetRfFrequency(0,(uint32_t)(radioconfig.frequency0 * radioconfig.K_freq));
	LR112X_SetRfFrequency(0,radioconfig.frequency0);
	//LR112X_SetBufferBaseAddress(0,0, 0x80);
	if(radioconfig.frequency0 > LR112X_SEPARATION_FREQ)
	{
		LR112X_SetPaConfig(0,LR11XX_PA_SEL_HF,LR11XX_PA_REG_SUPPLY_VREG,LR11XX_PA_DUTYCYCLE_HF,LR11XX_PA_HPSEL_HF);
		LR112X_SetTxParams(0,radioconfig.txpower0,radioconfig.ramptime0);
	}
	else
	{
		LR112X_SetPaConfig(0,LR11XX_PA_SEL_SUBG_HP,LR11XX_PA_REG_SUPPLY_VBAT,LR11XX_PA_DUTYCYCLE_SUBG,LR11XX_PA_HPSEL_SUBG);
		LR112X_SetTxParams(0,radioconfig.txpower1,radioconfig.ramptime1);
	}
	LR112X_Calibrate(0,0x3f);
	//calibrate image here
	//calibrate RSSI
	LR112X_RssiCal(0,radioconfig.frequency0);
	LR112X_SetDioIrqParams(0,LR112X_TX_DONE | LR112X_RX_DONE | LR112X_CRC_ERROR,0);
	LR112X_SetRxTxFallbackMode(0,LR112X_FALLBACK_STBY_XOSC);
	LR112X_SetRxBoosted(0,true);
	//The workaround is to set the bit 4 in the register 0x00F30024 when the chip ends a reception in the 2.4GHz band before launching a GNSS scan. - ???
	//LR112X_ReadRegMem32(0,0x00f30024,buf,1);
	//buf[0] |= 0x10;
	//LR112X_WriteRegMem32(0,0x00f30024,buf,1);
}

void LR112X_setopmode(uint8_t mode)
{
  switch(mode)
  {
    case RADIO_OPMODE_SLEEP:
    LR112X_SetSleep(0,LR112X_SLEEP_POWERDOWN,false);
    break;

    case RADIO_OPMODE_STBYRC:
    LR112X_SetStandby(0,0);
    break;

    case RADIO_OPMODE_STBYXOSC:
    LR112X_SetStandby(0,1);
    break;

    case RADIO_OPMODE_FS:
    LR112X_SetFs(0);
    break;

    case RADIO_OPMODE_TX:
    LR112X_SetTx(0,0xffffff); //temp.
    break;

    case RADIO_OPMODE_RX:
    default:
    LR112X_SetRx(0,0); //temp.
    break;

    case RADIO_OPMODE_TXSTREAMCW:
    LR112X_SetTxCw(0);
    break;

    case RADIO_OPMODE_TXSTREAMPRE:
    LR112X_SetTxInfinitePreamble(0);
    break;
  }
	//LR112X_printerrors(0);
}

void LR112X_printerrors(void)
{
	printf("ERRORS = 0x%04X\r\n",LR112X_GetErrors(0));
}

void LR112X_printstatus(void)
{
	uint8_t stat1,stat2;
	
	LR112X_GetStatus(0,&stat1,&stat2);
	stat2 = (stat2 >> 1) & 0x07; //stat2
	stat1 = (stat1 >> 1) & 0x07; //stat1
	printf("STATUS:%d,%d\r\n",stat2,stat1);
}

void LR112X_RssiCal(uint8_t lr,uint32_t freq)
{
	if(freq <= 60000000) LR112X_SetRssiCalibration(0,&calib_0_600);
	else if((freq > 60000000) && (freq <= 200000000)) LR112X_SetRssiCalibration(0,&calib_600_2000);
	else LR112X_SetRssiCalibration(0,&calib_2000_2700);
}
