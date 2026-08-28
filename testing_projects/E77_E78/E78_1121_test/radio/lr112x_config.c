#include "lr112x_config.h"
#include "lr112x.h"
#include "radio.h"


//#define K_FREQ	1.0000051f //-6.8 ppm
//#define K_FREQ	1.0f
/*
int8_t power_table[10]={-5,-2,1,4,7,10,13,16,19,22};
int8_t power_table_2p4G[10]={-18,-15,-12,-9,-6,-3,0,1,2,13};
int8_t power_table_1p9G[10]={-18,-15,-12,-9,-6,-3,0,1,2,13};
lr11xx_hal_status_t lr11xx_hal_reset( const void* context )
{	
    RF_NRESET_LOW();
    LL_mDelay( 2 );
    RF_NRESET_HIGH();

    return LR11XX_HAL_STATUS_OK;
}

lr11xx_hal_status_t lr11xx_hal_wakeup( const void* context )
{
    SPIn_CS_LOW();
    LL_mDelay( 1 );
    SPIn_CS_HIGH();

    return LR11XX_HAL_STATUS_OK;
}


void lr1121_init(void)
{
		// system init
	uint16_t errors;
	lr11xx_system_version_t version;
	uint32_t freq;
	int8_t pwr;
		
	lr11xx_system_set_reg_mode(NULL,LR11XX_SYSTEM_REG_MODE_DCDC); // DC-DC
	lr11xx_system_set_dio_as_rf_switch(NULL, &rf_switch_setup);
	// tcxo
	//lr11xx_system_set_tcxo_mode(NULL, LR11XX_SYSTEM_TCXO_CTRL_3_0V, 300);
	lr11xx_system_cfg_lfclk(NULL, LR11XX_SYSTEM_LFCLK_XTAL, true);
	lr11xx_system_clear_errors(NULL);
	lr11xx_system_calibrate(NULL, 0x3f);

	lr11xx_system_get_errors(NULL, &errors);
	lr11xx_system_clear_errors(NULL);
	lr11xx_system_clear_irq_status(NULL, LR11XX_SYSTEM_IRQ_ALL_MASK);

	lr11xx_system_get_version(NULL, &version);

	// radio init
	lr11xx_radio_set_pkt_type(NULL, LR11XX_RADIO_PKT_TYPE_LORA);
	if(lora_band==1)
		freq=lora_freq_2p4G*1000000;
	if(lora_band==0)
		freq=lora_freq*1000000;
	if(lora_band==2)
		freq=lora_freq_1p9G*1000000;
	lr11xx_radio_set_rf_freq(NULL, freq);
	// lr11xx_radio_set_rssi_calibration();
//	lr11xx_radio_set_pa_cfg(NULL, &pa_config);
	if(lora_band==1)
	{
		pwr=power_table_2p4G[lora_power_2p4G];
		lr11xx_radio_set_pa_cfg(NULL, &pa_config_2p4G);	
		lr11xx_radio_set_tx_params(NULL, pwr, LR11XX_RADIO_RAMP_48_US );	// // range [-17, +22] for sub-G, range [-18, 13] for 2.4G ( HF_PA )
	}
	if(lora_band==0)
	{
		pwr=power_table[lora_power];
		lr11xx_radio_set_pa_cfg(NULL, &pa_config_subGHz);	
		lr11xx_radio_set_tx_params(NULL, pwr, LR11XX_RADIO_RAMP_48_US );	// // range [-17, +22] for sub-G, range [-18, 13] for 2.4G ( HF_PA )
	}
	if(lora_band==2)
	{
		pwr=power_table_1p9G[lora_power_1p9G];
		lr11xx_radio_set_pa_cfg(NULL, &pa_config_2p4G);	
		lr11xx_radio_set_tx_params(NULL, pwr, LR11XX_RADIO_RAMP_48_US );	// // range [-17, +22] for sub-G, range [-18, 13] for 2.4G ( HF_PA )		
	}
//	lr11xx_radio_set_tx_params(NULL, pwr, LR11XX_RADIO_RAMP_48_US );	// // range [-17, +22] for sub-G, range [-18, 13] for 2.4G ( HF_PA )
	lr11xx_radio_set_rx_tx_fallback_mode(NULL, LR11XX_RADIO_FALLBACK_STDBY_RC);
	lr11xx_radio_cfg_rx_boosted(NULL, 0x00);		// enable_boost_mode

	lr11xx_radio_set_lora_mod_params(NULL, &lora_mod_params);
	lr11xx_radio_set_lora_pkt_params(NULL, &lora_pkt_params );
	//lr11xx_radio_set_lora_sync_word(NULL, LORA_SYNCWORD);

	air_time=lr11xx_radio_get_lora_time_on_air_in_ms(&lora_pkt_params,&lora_mod_params);
	air_time=air_time*4;

}
*/

void LR112X_printerrors(uint8_t lr);
void LR112X_printstatus(uint8_t lr);
void LR112X_configcommon(uint8_t lr,LR112Xconfig_t *config);

const LR112X_RssiCalibParams_t calib_0_600 =     {12,12,14, 0, 1, 3, 4, 4, 3, 6, 6, 6, 6, 6, 6, 6, 6, 0};
const LR112X_RssiCalibParams_t calib_600_2000 =  { 2, 2, 2, 3, 3, 4, 5, 4, 4, 6, 5, 5, 6, 6, 6, 7, 6, 0};
const LR112X_RssiCalibParams_t calib_2000_2700 = { 6, 7, 6, 4, 3, 4,14,12,14,12,12,12,12, 8, 8, 9, 9, 2030};

void LR112X_config(uint8_t lr,LR112Xconfig_t *config)
{
	delay_ms(300);
	//LR112X_Wakeup(lr);
	LR112X_SetRegMode(lr,true);
	LR112X_SetStandby(0,0);
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

//GainOffset: Global offset added to the Gain Tune values. The offset is a 12-bit signed value, where 1lsb = 0.5dB.
//The power seen by the LR1121 analog front-end is affected by external components such as the matching network, or RF switches. An incorrect RSSI results in a sensitivity degradation in (G)FSK mode and an incorrect gain selection in LoRa and GFSK mode. An incorrect gain can result in a missed detection (packet loss) or decreased resistance to interference.
//By default, the chip is calibrated for the 868-915MHz band on the LR1121 EVK.

//below 600MHz 0  12 12 14 0 1 3 4 4 3 6 6 6 6 6 6 6 6
//from 600MHz to 2GHz 0 2  2  2  3  3  4  5  4  4  6  5  5  6  6  6  7  6
//above 2GHz 2030 6 7 6 4 3 4 14 12 14 12 12 12 12 8 8 9 9

// uint8_t gain[] = { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 }; // NB: for the 2G4 path, the max gain is 16 - 17-20 can be ignored.
//    float power[] = { -35.0, -41.0, -45.0, -50.0, -53.5, -60.0, -65.0, -69.5, -75.0, -81.0, -82.5, -83.5, -84.0, -85.0, -86.0, -86.5, -87.0 };
//    /* Configure the chip at the system level */ 
//    lr11xx_system_reset( context );
//    lr11xx_system_set_reg_mode( context, reg_mode ); 
//    lr11xx_system_set_dio_as_rf_switch( context, rf_switch_cfg );
//    lr11xx_system_set_tcxo_mode( context, tune, timeout );  // Optional - only if there is a TCXO
//    lr11xx_system_clear_errors( context );
//    lr11xx_system_calibrate( context, 0x3F );  // 0x3F to enable all fields
//    /* Configure the chip at the modem level */ 
//    lr11xx_radio_set_pkt_type( context, LR11XX_RADIO_PKT_TYPE_GFSK ); 
//    lr11xx_radio_set_rf_freq( context, freq_in_hz );
//    lr11xx_system_calibrate_image_in_mhz( context, freq1_in_mhz, freq2_in_mhz ); 
//    lr11xx_radio_set_gfsk_mod_params( context, mod_params );  // Rx BW has to be set to
//LR11XX_RADIO_GFSK_BW_234300 - other modulation parameters can be anything 
//    lr11xx_radio_set_gfsk_pkt_params( context, pkt_params );  // Packet parameters can be
//anything
///* Configure the chip to be controlled manually */
//    lr11xx_regmem_write_regmem32_mask( context, 0x00F20214, 0x00080000, 0x00080000 );
//    lr11xx_regmem_write_regmem32_mask( context, 0x00F20230, 0x71110000, 0x71100000 );
//			
// lr11xx_radio_set_rssi_calibration( context, rssi_cal_table );  // All parrameters of rssi_cal_table set to 0
//    lr11xx_radio_set_rx_with_timeout_in_rtc_step( context, 0xFFFFFF );
//    for( int i = 0; i++; i < 17 )  // 17 is the number of elements in gain array 
//    {
//        const uint8_t gain_step = MIN( gain[i], 13 );
//        const uint8_t lna_boost = ( gain > 13 ) ? gain - 13 : 0;
//        lr11xx_regmem_write_regmem32_mask( context, 0x00F20214, 0x00F00000, gain_step << 20 ); 
//        lr11xx_regmem_write_regmem32_mask( context, 0x00F3008C, 0x00070000, lna_boost << 16 );

///* Wait for 1 ms */
///* Insert here a control for your test equipment to generate a tone at RF frequency set to freq_in_hz with an output power set to power[i] dBm */
//        lr11xx_radio_get_rssi_inst( context, rssi_in_dbm );
///* Add a way to log (gain[i], power[i], rssi_in_dbm) triplet to be able to compute offset and
//tunes for the RSSI calibration */
//    }			