#include "radio.h"
#include "tremo_spi.h"
#include "flash.h"
#include "ustimer.h"
#include "gui.h"
#include "beeper.h"

#define LB_RSSI_POINTS	13
const uint32_t LBrssifreqs[] = 
{
	150000000,
	200000000,
	250000000,
	300000000,
	350000000,
	400000000,
	450000000,
	500000000,
	550000000,
	600000000,
	650000000,
	700000000,
	750000000
};
const float LBrssideltas[LB_RSSI_POINTS] = 
{
	-22.0,
	-14.0,
	-7.5,
	-3.0,
	2.0,
	5.0,
	3.0,
	1.0,
	-2.0,
	-5.0,
	-8.0,
	-11.0,
	-13.0
};

#define MB_RSSI_POINTS	18
const uint32_t MBrssifreqs[MB_RSSI_POINTS] = 
{
	500000000,
	550000000,
	600000000,
	650000000,
	700000000,
	750000000,
	800000000,
	850000000,
	900000000,
	950000000,
	1000000000,
	1050000000,
	1100000000,
	1150000000,
	1200000000,
	1250000000,
	1300000000,
	1350000000
};
const float MBrssideltas[] = 
{
	-22.0,
	-19.0,
	-16.0,
	-13.0,
	-12.0,
	-5.0,
	-2.0,	
	0.0,
	0.0,
	0.0,
	-3.0,
	-8.0,
	-11.0,
	-13.0,
	-14.0,
	-16.0,
	-17.0,
	-19.0
};

#define HB_RSSI_POINTS	12
const uint32_t HBrssifreqs[] = 
{
	2100000000,
	2150000000,
	2200000000,
	2250000000,
	2300000000,
	2350000000,
	2400000000,
	2450000000,
	2500000000,
	2550000000,
	2600000000,
	2650000000
};
const float HBrssideltas[] = 
{
	-5.0,
	-6.0,
	-6.0,
	-6.0,
	-5.0,
	-5.0,
	-5.0,
	-5.0,
	-5.0,
	-5.0,
	-5.0,
	-5.0
};

globalrfconfig_t globalrfconfig;

bool txcomplete[3];
bool rxcomplete[3];

uint8_t opmodes[3];
uint8_t prevopmodes[3];

bool txmode[3];
bool sweeptx[3];
bool sweeprx[3];
float rssi_tr[3];
float rssi_peak[3];

uint32_t startfreq[3];
uint32_t stopfreq[3];
uint32_t freqstep[3];
uint32_t currfreq[3];
uint32_t prevfreq[3];

bool radioflags[3];
uint16_t radioevents[3];

void radio_proc(void)
{
	uint8_t radio;
	
	//handle radioflags
	for(radio = 0; radio < 3; radio++)
	{
		if(radioflags[radio] == true)
		{
			radioflags[radio] = false;
			radio_getevents(radio);
			radio_handleevents(radio);
		}
	}
	
	for(radio = 0; radio < 3; radio++)
	{
		if(sweepflag[radio])
		{
			sweepflag[radio] = false;
//			if(sweeptx[radio])
//			{
//				currfreq[radio] += freqstep[radio];
//				if(currfreq[radio] >= stopfreq[radio]) currfreq[radio] = startfreq[radio];
//				//radio_standby(radio,1);
//				radio_fs(radio);
//				radio_set_frequency(radio,currfreq[radio]);
//				radio_stream(radio,txmode[radio]);
//			}
//			if(sweeprx[radio])
//			{
				float rssi = radio_get_rssi(radio);
				if(rssi > rssi_tr[radio])
				{
					rssi_peak[radio] = rssi;
					printf("@R%d,%d,%d\r\n",radio,currfreq[radio] / 1000,(int8_t)rssi); //temporary; send to buffer instead
					display_rssi(radio,currfreq[radio]/1000,rssi_peak[radio]);
					beep(1000+1000*radio,50);
				}
				radio_fs(radio);
				currfreq[radio] += freqstep[radio];
				if(currfreq[radio] >= stopfreq[radio]) currfreq[radio] = startfreq[radio];
				radio_set_frequency(radio,currfreq[radio]);
				radio_rx(radio);
//			}
		}
	}
}


void radio_initconfigs(void)
{
  const uint8_t sync[8] = {0xc9,0x6c,0xc9,0x6c,0x23,0x6b,0x23,0x6b};
	
	//Low band (internal SX1262)
	globalrfconfig.LBconfig.modem = SX126X_MODEM_LORA;
  globalrfconfig.LBconfig.startfreq = 380000000;
	globalrfconfig.LBconfig.stopfreq = 470000000;
	globalrfconfig.LBconfig.scantimems = 10;
	globalrfconfig.LBconfig.freqstep = 100000;
	globalrfconfig.LBconfig.scan_active = true;
	
	globalrfconfig.LBconfig.rssithreshold = -90;
	globalrfconfig.LBconfig.scan_active = false;
	
  globalrfconfig.LBconfig.txpower = 10;
  globalrfconfig.LBconfig.ramptime = SX126X_RAMP_10U;
  //LoRa
  globalrfconfig.LBconfig.sf = SX126X_LORA_SF_8; //spreading factor
  globalrfconfig.LBconfig.bw = SX126X_LORA_BW_41p7; //bandwidth
  globalrfconfig.LBconfig.cr = SX126X_LORA_CR_4_5; //coding rate
  globalrfconfig.LBconfig.opt = false;
  globalrfconfig.LBconfig.loraprelen = 8; //???
  globalrfconfig.LBconfig.header = false;
  globalrfconfig.LBconfig.lorasync = 0xc96c;
  globalrfconfig.LBconfig.lorapaylen = 32;
  globalrfconfig.LBconfig.loracrc = true;
  globalrfconfig.LBconfig.invertiq = false;
  //FSK parameters
  globalrfconfig.LBconfig.br = 50000; //baud rate
  globalrfconfig.LBconfig.rbw = SX126X_FSK_BW_93p8;
  globalrfconfig.LBconfig.shaping = SX126X_FSK_BT0p5;
  globalrfconfig.LBconfig.fdev = 25000;
  globalrfconfig.LBconfig.fskprelen = 32;
	globalrfconfig.LBconfig.predetlen = SX126X_FSK_PREDET_8;
  globalrfconfig.LBconfig.fsksynclen = 16;
  for (uint8_t i = 0; i < 8; i++) globalrfconfig.LBconfig.fsksync[i] = sync[i];//SX1280 needs 15 bytes
  globalrfconfig.LBconfig.addrcomp = SX126X_FSK_ADDR_FILT_DISABLED;
  globalrfconfig.LBconfig.nodeaddr = 0; 
  globalrfconfig.LBconfig.braddr = 0xff; 
  globalrfconfig.LBconfig.varlen = false;
  globalrfconfig.LBconfig.fskpaylen = 32;
  globalrfconfig.LBconfig.crctype = SX126X_FSK_CRC_2B;
  globalrfconfig.LBconfig.crcinit = 0xffff;
  globalrfconfig.LBconfig.crcpoly = 0x1021; //0x8005
  globalrfconfig.LBconfig.white = true;
  globalrfconfig.LBconfig.whiteinit = 0xaa; //???
	
	//Mid band (external SX1262)
	globalrfconfig.MBconfig.modem = SX126X_MODEM_LORA;
  globalrfconfig.MBconfig.startfreq = 680000000;
	globalrfconfig.MBconfig.stopfreq = 990000000;
	globalrfconfig.MBconfig.freqstep = 100000;
	globalrfconfig.MBconfig.scantimems = 10;
	globalrfconfig.MBconfig.rssithreshold = -90;
	globalrfconfig.MBconfig.scan_active = true;
	
  globalrfconfig.MBconfig.txpower = 10;
  globalrfconfig.MBconfig.ramptime = SX126X_RAMP_10U;
  //LoRa
  globalrfconfig.MBconfig.sf = SX126X_LORA_SF_8; //spreading factor
  globalrfconfig.MBconfig.bw = SX126X_LORA_BW_62p5; //bandwidth
  globalrfconfig.MBconfig.cr = SX126X_LORA_CR_4_5; //coding rate
  globalrfconfig.MBconfig.opt = false;
  globalrfconfig.MBconfig.loraprelen = 8; //???
  globalrfconfig.MBconfig.header = false;
  globalrfconfig.MBconfig.lorasync = 0xc96c;
  globalrfconfig.MBconfig.lorapaylen = 32;
  globalrfconfig.MBconfig.loracrc = true;
  globalrfconfig.MBconfig.invertiq = false;
  //FSK parameters
  globalrfconfig.MBconfig.br = 50000; //baud rate
  globalrfconfig.MBconfig.rbw = SX126X_FSK_BW_93p8;
  globalrfconfig.MBconfig.shaping = SX126X_FSK_BT0p5;
  globalrfconfig.MBconfig.fdev = 25000;
  globalrfconfig.MBconfig.fskprelen = 32;
	globalrfconfig.MBconfig.predetlen = SX126X_FSK_PREDET_8;
  globalrfconfig.MBconfig.fsksynclen = 16;
  for (uint8_t i = 0; i < 8; i++) globalrfconfig.MBconfig.fsksync[i] = sync[i];//SX1280 needs 15 bytes
  globalrfconfig.MBconfig.addrcomp = SX126X_FSK_ADDR_FILT_DISABLED;
  globalrfconfig.MBconfig.nodeaddr = 0; 
  globalrfconfig.MBconfig.braddr = 0xff; 
  globalrfconfig.MBconfig.varlen = false;
  globalrfconfig.MBconfig.fskpaylen = 32;
  globalrfconfig.MBconfig.crctype = SX126X_FSK_CRC_2B;
  globalrfconfig.MBconfig.crcinit = 0xffff;
  globalrfconfig.MBconfig.crcpoly = 0x1021; //0x8005
  globalrfconfig.MBconfig.white = true;
  globalrfconfig.MBconfig.whiteinit = 0xaa; //???
	
	//High band (external SX1280)
	globalrfconfig.HBconfig.modem = SX128X_MODEM_LORA;
	//configs[2].modem = SX128X_MODEM_FSK;
  globalrfconfig.HBconfig.startfreq = 2100000000;
	globalrfconfig.HBconfig.stopfreq = 2700000000;
	globalrfconfig.HBconfig.freqstep = 500000;
	globalrfconfig.HBconfig.scantimems = 10;
	globalrfconfig.HBconfig.rssithreshold = -90;
	globalrfconfig.HBconfig.scan_active = true;
	
  globalrfconfig.HBconfig.txpower = 13;
  globalrfconfig.HBconfig.ramptime = SX128X_RAMP_04_US;
  //LoRa
  globalrfconfig.HBconfig.sf = SX128X_SF_8; //spreading factor
  globalrfconfig.HBconfig.bw = SX128X_BW_200; //bandwidth
  globalrfconfig.HBconfig.cr = SX128X_CR_4_5; //coding rate
  globalrfconfig.HBconfig.opt = false;
  globalrfconfig.HBconfig.loraprelen = 0x08; //???
  globalrfconfig.HBconfig.implheader = false;
  globalrfconfig.HBconfig.lorasync = 0xc96c; //???
  globalrfconfig.HBconfig.lorapaylen = 32;
  globalrfconfig.HBconfig.loracrc = true;
  globalrfconfig.HBconfig.invertiq = false;
  //FSK parameters
  globalrfconfig.HBconfig.br = 50000; //baud rate
  globalrfconfig.HBconfig.rbw = SX126X_FSK_BW_93p8; //fake
  globalrfconfig.HBconfig.shaping = SX126X_FSK_BT0p5;
  globalrfconfig.HBconfig.fdev = 25000; //to calculate modindex
  globalrfconfig.HBconfig.fskprelen = SX128X_FSK_PRE_LEN_32; //32;
	globalrfconfig.HBconfig.predetlen = 4;
  globalrfconfig.HBconfig.fsksynclen = 16;
  for (uint8_t i = 0; i < 8; i++) globalrfconfig.HBconfig.fsksync[i] = sync[i];//SX1280 needs 15 bytes
  //configs[2].addrcomp = SX126X_FSK_ADDR_FILT_DISABLED;
	globalrfconfig.HBconfig.syncmatch = SX128X_SYNCWORD_1; //for SV1280 only
  globalrfconfig.HBconfig.nodeaddr = 0; 
  globalrfconfig.HBconfig.braddr = 0xff; 
  globalrfconfig.HBconfig.varlen = false;
  globalrfconfig.HBconfig.fskpaylen = 32;
  globalrfconfig.HBconfig.crctype = SX128X_FSK_CRC_2B;
  globalrfconfig.HBconfig.crcinit = 0xffff;
  globalrfconfig.HBconfig.crcpoly = 0x1021; //0x8005
  globalrfconfig.HBconfig.white = true;
  globalrfconfig.HBconfig.whiteinit = 0xaa; //???
	
	globalrfconfig.magicnumber = MAGIC_NUMBER;
	
	writeconfig();
}

//General
void radio_interface_init(void) //setup GPIO, SPI, interrupts for all radios
{
  lorac_init(); //main
	//aux. RF power on
	gpio_init(EXTRF_POWER_PORT, EXTRF_POWER_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
	//aux. SX1262
	gpio_init(SX1262_NSS_PORT, SX1262_NSS_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
	gpio_init(SX1262_RESET_PORT, SX1262_RESET_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
	gpio_init(SX1262_BUSY_PORT, SX1262_BUSY_PIN, GPIO_MODE_INPUT_PULL_DOWN);
	gpio_init(SX1262_INT_PORT, SX1262_INT_PIN, GPIO_MODE_INPUT_PULL_DOWN);
	//aux. SX1280
	gpio_init(SX1280_NSS_PORT, SX1280_NSS_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
	gpio_init(SX1280_RESET_PORT, SX1280_RESET_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
	gpio_init(SX1280_TCXO_PORT, SX1280_TCXO_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
	gpio_config_drive_capability(SX1280_TCXO_PORT, SX1280_TCXO_PIN, GPIO_DRIVE_CAPABILITY_8MA);
	gpio_write(SX1280_TCXO_PORT, SX1280_TCXO_PIN,GPIO_LEVEL_HIGH);
	gpio_init(SX1280_BUSY_PORT, SX1280_BUSY_PIN, GPIO_MODE_INPUT_PULL_DOWN);
	gpio_init(SX1280_INT_PORT, SX1280_INT_PIN, GPIO_MODE_INPUT_PULL_DOWN);
	//aux. SPI
	gpio_set_iomux(AUX_SCK_PORT, AUX_SCK_PIN, 4);
	gpio_set_iomux(AUX_MOSI_PORT, AUX_MOSI_PIN, 4);
	gpio_set_iomux(AUX_MISO_PORT, AUX_MISO_PIN, 4);
	//init SPI module
	ssp_init_t init_struct;
	rcc_enable_peripheral_clk(AUX_RCC_PERIPHERAL, true);
	//ssp_init_struct(&init_struct);
	init_struct.ssp_sclk      = 8000000; //???
	init_struct.ssp_role      = SSP_ROLE_MASTER;
	init_struct.ssp_format    = SSP_FRAME_FORMAT_SPI;
	init_struct.ssp_data_size = SSP_DATA_SIZE_8BIT;
	//init_struct.ssp_clk_pol   = SPI_CLK_POLARITY_HIGH;
	//init_struct.ssp_clk_phase = SPI_CLK_PHASE_2EDGE;
	init_struct.ssp_clk_pol   = SPI_CLK_POLARITY_LOW;
	init_struct.ssp_clk_phase = SPI_CLK_PHASE_1EDGE;
	init_struct.ssp_dma_tx_en = DISABLE;
	init_struct.ssp_dma_rx_en = DISABLE;
	ssp_init(AUX_SSP, &init_struct);
	ssp_cmd(AUX_SSP, ENABLE);
	gpio_config_interrupt(SX1262_INT_PORT, SX1262_INT_PIN, GPIO_INTR_RISING_EDGE);
	gpio_config_interrupt(SX1280_INT_PORT, SX1280_INT_PIN, GPIO_INTR_RISING_EDGE);
  NVIC_ClearPendingIRQ(GPIO_IRQn);
  NVIC_EnableIRQ(GPIO_IRQn);
	delay_ms(100);
}

bool radio_init(uint8_t radio) //load current configuration, probably not needed due to separating RX/TX configs
{
  //radio_readconfig(radio,&txconfigs[radio],&rxconfigs[radio]);
  switch(radio)
  {
    case RADIO_L: //internal SX1262
    
    SX126X_reset(0);
    //set default parameters
    SX126X_config(0,&globalrfconfig.LBconfig); //default config is RX
		SX126X_SetStandby(0,1);
		//SX126X_SetRxTxFallbackMode(0,SX126X_FALLBACK_FS);
		//SX126X_SetDIO2AsRfSwitchCtrl(0,true);
		SX126X_ClearDeviceErrors(0);
		currfreq[0] = globalrfconfig.LBconfig.startfreq;
		prevfreq[0] = currfreq[0];
		radio_rx(0);
		SX126X_ClearDeviceErrors(1);
		//if(globalrfconfig.LBconfig.scan_active == true) action_flags[0] = true;
    return true;
		
		case RADIO_M: ////auxillary SX1262
		SX126X_reset(1);
		SX126X_Wakeup(1);
		delay_ms(10);
		SX126X_SetStandby(1,0);
		//SX126X_SetRegulatorMode(1,1);
		SX126X_SetDIO3AsTCXOCtrl(1,0x02,1000);
    //set default parameters
    SX126X_config(1,&globalrfconfig.MBconfig); //default config is RX
		//SX126X_SetDIO2AsRfSwitchCtrl(1,true);
		SX126X_SetStandby(1,1);
		//SX126X_SetRxTxFallbackMode(1,SX126X_FALLBACK_FS);
		currfreq[1] = globalrfconfig.MBconfig.startfreq;
		prevfreq[1] = currfreq[1];
		radio_rx(1);
		SX126X_ClearDeviceErrors(1);
		//if(globalrfconfig.MBconfig.scan_active == true) action_flags[1] = true;
		return true;
		
		case RADIO_H:
		SX128X_reset(0);
		SX128X_config(0,&globalrfconfig.HBconfig); //default config is RX
		currfreq[2] = globalrfconfig.HBconfig.startfreq;
		prevfreq[2] = currfreq[2];
		radio_rx(2);
		//if(globalrfconfig.HBconfig.scan_active == true) action_flags[2] = true;
		return true;
    
    default:
    return false; //radio not exist
  }
}


bool radio_on(uint8_t radio) //power switch, wake-up, init
{
  switch(radio)
  {
    case RADIO_L: //internal SX1262
    case RADIO_M: ////auxillary SX1262
    return true;
		
		case RADIO_H:
		return true;
    
    default:
    return false;
  }
}

bool radio_off(uint8_t radio) //sleep mode and/or power switch
{
  switch(radio)
  {
    case RADIO_L: //internal SX1262
    case RADIO_M: ////auxillary SX1262
    SX126X_SetSleep(radio,0,false);
    return true;
    
    case RADIO_H:
		SX128X_SetSleep(0,false);
		return true;
    
    default:
    return false;
  }
}

bool radio_getstatus(uint8_t radio,uint8_t *mode,uint8_t *status)
{
  uint8_t regval;
	
	switch(radio)
  {
    case RADIO_L: //internal SX1262
    case RADIO_M: ////auxillary SX1262
		regval = SX126X_GetStatus(radio);
		*mode = (regval >> 4) & 0x07;
		*status = (regval >> 1) & 0x07;
    return true;
    
		case RADIO_H:
		regval = SX128X_GetStatus(0);
		*mode = (regval >> 5) & 0x07;
		*status = (regval >> 2) & 0x07;
		return true;
		
    default:
    return false;
  }
}

bool radio_standby(uint8_t radio,uint8_t mode)
{
  if(mode > 1) return false;
	else
	{
		switch(radio)
		{
			case RADIO_L: //internal SX1262
			case RADIO_M: ////auxillary SX1262
			SX126X_SetStandby(radio,mode);
			if(mode == 0) opmodes[radio] = RADIO_OPMODE_STBYRC;
			else opmodes[radio] = RADIO_OPMODE_STBYXOSC;
			return true;
    
			case RADIO_H:
			SX128X_SetStandby(0,mode);
			if(mode == 0) opmodes[radio] = RADIO_OPMODE_STBYRC;
			else opmodes[radio] = RADIO_OPMODE_STBYXOSC;
			return true;
		
			default:
			return false;
		}
	}
}

bool radio_fs(uint8_t radio)
{
		switch(radio)
		{
			case RADIO_L: //internal SX1262
			case RADIO_M: ////auxillary SX1262
			SX126X_SetFs(radio);
			return true;
    
			case RADIO_H:
			SX128X_SetFs(0);
			return true;
		
			default:
			return false;
		}
}

bool radio_rx(uint8_t radio)
{
  switch(radio)
  {
    case RADIO_L: //internal SX1262
    case RADIO_M: ////auxillary SX1262
		SX126X_SetRx(radio,0);
		opmodes[radio] = RADIO_OPMODE_RX;
    return true;
    
		case RADIO_H:
		SX128X_SetRx(0,0,0);
		opmodes[radio] = RADIO_OPMODE_RX;
		return true;
		
    default:
    return false;
  }
}

bool radio_reset(uint8_t radio)
{
  radio_off(radio);
  delay_ms(100);
  radio_on(radio);
	return true;
}


bool radio_config(uint8_t radio) //power, modulation, ... before packet or signal sending
{
  switch(radio)
  {
    case RADIO_L: //internal SX1262
		SX126X_config(radio,&globalrfconfig.LBconfig);	
		return true;
		
    case RADIO_M: ////auxillary SX1262
    SX126X_config(radio,&globalrfconfig.MBconfig);
    return true;
    
    case RADIO_H:
		SX128X_config(0,&globalrfconfig.HBconfig);
		return true;
    
    default:
    return false; //radio not exist
  }
}

bool radio_setmodem(uint8_t radio,uint8_t modem)
{
  switch(radio)
  {
    case RADIO_L: //internal SX1262
		if(modem == 0) globalrfconfig.LBconfig.modem = SX126X_MODEM_FSK; 
		else if(modem == 1) globalrfconfig.LBconfig.modem = SX126X_MODEM_LORA;
		else return false;
    SX126X_config(radio,&globalrfconfig.LBconfig);
    return true;
		
    case RADIO_M: ////auxillary SX1262
		if(modem == 0) globalrfconfig.MBconfig.modem = SX126X_MODEM_FSK; 
		else if(modem == 1) globalrfconfig.MBconfig.modem = SX126X_MODEM_LORA;
		else return false;
    SX126X_config(radio,&globalrfconfig.MBconfig);
    return true;
    
    case RADIO_H:
		if(modem == 0) globalrfconfig.HBconfig.modem = SX128X_MODEM_FSK; 
		else if(modem == 1) globalrfconfig.HBconfig.modem = SX128X_MODEM_LORA; 
		else return false; //temp. not supported
		SX128X_config(0,&globalrfconfig.HBconfig);
		return true;
    
    default:
    return false; //radio not exist
  }
}

bool radio_getmodem(uint8_t radio,uint8_t *modem)
{
  switch(radio)
  {
    case RADIO_L: //internal SX1262
		if(globalrfconfig.LBconfig.modem == SX126X_MODEM_FSK) *modem = 0; 
		else if(globalrfconfig.LBconfig.modem == SX126X_MODEM_LORA) *modem = 1;
		else return false;
    return true;
			
    case RADIO_M: ////auxillary SX1262
		if(globalrfconfig.MBconfig.modem == SX126X_MODEM_FSK) *modem = 0; 
		else if(globalrfconfig.MBconfig.modem == SX126X_MODEM_LORA) *modem = 1;
		else return false;
    return true;
    
    case RADIO_H:
		if(globalrfconfig.HBconfig.modem == SX128X_MODEM_FSK) *modem = 0; 
		else if(globalrfconfig.HBconfig.modem == SX128X_MODEM_LORA) *modem = 1; 
		else return false; //temp. not supported
		return true;
    
    default:
    return false; //radio not exist
  }
}

//TX
//bool radio_setpower(uint8_t radio,int8_t power)
//{
//  switch(radio)
//  {
//    case RADIO_L: //internal SX1262
//		if((power > 22) || (power < -9)) return false;
//		else
//		{
//			globalrfconfig.LBconfig.txpower = power;
//			SX126X_SetTxParams(radio,globalrfconfig.LBconfig.txpower,globalrfconfig.LBconfig.ramptime);
//			return true;
//		}
//			
//    case RADIO_M: ////auxillary SX1262
//		if((power > 22) || (power < -9)) return false;
//		else
//		{
//			globalrfconfig.MBconfig.txpower = power;
//			SX126X_SetTxParams(radio,globalrfconfig.MBconfig.txpower,globalrfconfig.MBconfig.ramptime);
//			return true;
//		}
//    
//    case RADIO_H:
//		if((power > 13) || (power < -18)) return false;
//		else
//		{
//			globalrfconfig.HBconfig.txpower = power;
//			SX128X_SetTxParams(0,globalrfconfig.HBconfig.txpower,globalrfconfig.HBconfig.ramptime);
//			return true;
//		}
//    
//    default:
//    return false; //radio not exist
//  }
//}

//bool radio_senddata(uint8_t radio,uint8_t *txbuf,uint8_t len)
//{
//  //set data length if needed
//  //fill tx buffer
//  //start TX
//	return true;
//}

//bool radio_txhandler(uint8_t radio) //call on tx complete
//{
//  return false;
//}

bool radio_stream(uint8_t radio,uint8_t mode) //test function, CW or preamble. If mode==0, TX off.
{
  if(mode == 0) return radio_rx(radio);
  else
  {
  switch(radio)
    {
      case RADIO_L: //internal SX1262
			case RADIO_M: ////auxillary SX1262
			if(mode == 1) 
			{
				SX126X_SetCW(radio);
				prevopmodes[radio] = opmodes[radio];
				opmodes[radio] = RADIO_OPMODE_TXSTREAMCW;
			}
			else if(mode == 2) 
			{
				SX126X_SetTxInfinitePreamble(radio);
				prevopmodes[radio] = opmodes[radio];
				opmodes[radio] = RADIO_OPMODE_TXSTREAMPRE;
			}
			else 
			{
				//SX126X_SetRx(radio,0);
				radio_rx(radio);
				SX126X_ClearDeviceErrors(radio);
				return false;
			}
			SX126X_ClearDeviceErrors(radio);
      return true;
			
			case RADIO_H:
				
			if(mode == 1) 
			{
				SX128X_SetCW(0);
				prevopmodes[radio] = opmodes[radio];
				opmodes[radio] = RADIO_OPMODE_TXSTREAMCW;
			}
			else if(mode == 2) 
			{
				SX128X_SetPRE(0);
				prevopmodes[radio] = opmodes[radio];
				opmodes[radio] = RADIO_OPMODE_TXSTREAMPRE;
			}
			else return false;
			return true;
    
      default:
      return false;
    }
  }
}

//bool radio_sweeptx(uint8_t radio,uint32_t start,uint32_t stop,uint32_t step,uint32_t us,uint8_t stream) //if stream==0,stop jamming
//{
//  if(radio > 2) return false;
//	if(stop < (start+step)) return false;
//	if(step < MIN_FREQ_STEP) return false;
//	if((us < MIN_TX_SWEEP_TIME) || (us > MAX_TX_SWEEP_TIME)) return false;
//	if(stream > 2) return false;
//	
//	//if stream == 0, stop and go to RX
//	if(stream == 0)
//	{
//		ustimer_stop(radio);
//		txmode[radio] = 0;
//		sweeptx[radio] = false;
//		radio_set_frequency(radio,prevfreq[radio]);
//		currfreq[radio] = prevfreq[radio];
//		radio_rx(radio);
//		return true;
//	}
//	else
//	{
//		prevfreq[radio] = currfreq[radio];
//		startfreq[radio] = start;
//		stopfreq[radio] = stop;
//		freqstep[radio] = step;
//		txmode[radio] = stream;
//		radio_set_frequency(radio,startfreq[radio]);
//		currfreq[radio] = startfreq[radio];
//		radio_stream(radio,stream);
//		ustimer_setinterval(radio,us);
//		ustimer_start(radio);
//		sweeptx[radio] = true;
//		return true;
//	}
//}

float radio_get_rssi(uint8_t radio)
{
  float retval;
	uint8_t i;
	
	switch(radio)
    {
      case RADIO_L: //internal SX1262
			retval = -((float)SX126X_GetRssiInst(radio)/2);
			if(currfreq[0] <= LBrssifreqs[0]) retval -= LBrssideltas[0];
			else if(currfreq[0] >= LBrssifreqs[LB_RSSI_POINTS-1]) retval -= LBrssideltas[LB_RSSI_POINTS-1];
			else
			{
				for(i = 0; i < LB_RSSI_POINTS-1; i++)
				{
					if((currfreq[0] > LBrssifreqs[i]) && (currfreq[0] <= LBrssifreqs[i+1])) retval -= LBrssideltas[i+1];
				}
			}
			return retval;
			
			case RADIO_M: ////auxillary SX1262
			retval = -((float)SX126X_GetRssiInst(radio)/2);
			if(currfreq[1] <= MBrssifreqs[1]) retval -= MBrssideltas[1];
			else if(currfreq[1] >= MBrssifreqs[MB_RSSI_POINTS-1]) retval -= MBrssideltas[MB_RSSI_POINTS-1];
			else
			{
				for(i = 0; i < MB_RSSI_POINTS-1; i++)
				{
					if((currfreq[1] > MBrssifreqs[i]) && (currfreq[1] <= MBrssifreqs[i+1])) retval -= MBrssideltas[i+1];
				}
			}
			return retval;
			
			case RADIO_H:
			retval = -((float)SX128X_GetRssiInst(0)/2);
			if(currfreq[2] <= HBrssifreqs[2]) retval -= HBrssideltas[2];
			else if(currfreq[2] >= HBrssifreqs[HB_RSSI_POINTS-1]) retval -= HBrssideltas[HB_RSSI_POINTS-1];
			else
			{
				for(i = 0; i < HB_RSSI_POINTS-1; i++)
				{
					if((currfreq[2] > HBrssifreqs[i]) && (currfreq[2] <= HBrssifreqs[i+1])) retval -= HBrssideltas[i+1];
				}
			}
			return retval;
    
      default:
      return -174.0;
    }
}

bool radio_scan(uint8_t radio,uint32_t start,uint32_t stop,uint32_t step,uint32_t ms,float tr) //if us==0,stop scanning
{
  if(radio > 2) return false;
	if(stop < (start+step)) return false;
	if(step < MIN_FREQ_STEP) return false;
	if((ms < MIN_RX_SWEEP_TIME) || (ms > MAX_RX_SWEEP_TIME)) return false;
	SSD1306_Clear(0);
	
	if(tr >= 0)
	{
		//GUI_ShowString(0,0,"IDLE       ",16,1);
		ustimer_stop(radio);
		sweeprx[radio] = false;
		radio_set_frequency(radio,prevfreq[radio]);
		currfreq[radio] = prevfreq[radio];
		radio_rx(radio);
		return true;
	}
	else
	{
		//GUI_ShowString(0,0,"SCAN       ",16,1);
		startfreq[radio] = start;
		stopfreq[radio] = stop;
		freqstep[radio] = step;
		rssi_tr[radio] = tr;
		radio_set_frequency(radio,startfreq[radio]);
		currfreq[radio] = startfreq[radio];
		radio_rx(radio);
		ustimer_setinterval(radio,ms * 1000); //in usec
		ustimer_start(radio);
		sweeprx[radio] = true;
		return true;
	}
}

void radio_startscan(void)
{
	if(globalrfconfig.LBconfig.scan_active == true) radio_scan(0,globalrfconfig.LBconfig.startfreq,
																															globalrfconfig.LBconfig.stopfreq,
																															globalrfconfig.LBconfig.freqstep,
																															globalrfconfig.LBconfig.scantimems,
																															globalrfconfig.LBconfig.rssithreshold);
	
	if(globalrfconfig.MBconfig.scan_active == true) radio_scan(1,globalrfconfig.MBconfig.startfreq,
																															globalrfconfig.MBconfig.stopfreq,
																															globalrfconfig.MBconfig.freqstep,
																															globalrfconfig.MBconfig.scantimems,
																															globalrfconfig.MBconfig.rssithreshold);
	
	if(globalrfconfig.HBconfig.scan_active == true) radio_scan(2,globalrfconfig.HBconfig.startfreq,
																															globalrfconfig.HBconfig.stopfreq,
																															globalrfconfig.HBconfig.freqstep,
																															globalrfconfig.HBconfig.scantimems,
																															globalrfconfig.HBconfig.rssithreshold);
}

void radio_stopscan(void)
{
	radio_scan(0,globalrfconfig.LBconfig.startfreq,
																															globalrfconfig.LBconfig.stopfreq,
																															globalrfconfig.LBconfig.freqstep,
																															globalrfconfig.LBconfig.scantimems,
																															0);
	
	radio_scan(1,globalrfconfig.MBconfig.startfreq,
																															globalrfconfig.MBconfig.stopfreq,
																															globalrfconfig.MBconfig.freqstep,
																															globalrfconfig.MBconfig.scantimems,
																															0);
	
	radio_scan(2,globalrfconfig.HBconfig.startfreq,
																															globalrfconfig.HBconfig.stopfreq,
																															globalrfconfig.HBconfig.freqstep,
																															globalrfconfig.HBconfig.scantimems,
																															0);
	for(uint8_t i = 1; i < 3; i++) sweepflag[i] = false;
}                                                                                                                                                                                                                                                                                                                                                                               

bool radio_rxhandler(uint8_t radio,uint8_t *rxbuf,uint8_t len) //call if packet received
{
  switch(radio)
  {
    case RADIO_L: //internal SX1262
		case RADIO_M: ////auxillary SX1262
    return true;
		
		case RADIO_H:
		return true;
    
    default:
    return false;
  }
}

bool radio_set_frequency(uint8_t radio, uint32_t freq) //change frequency "on-the-fly"
{
  switch(radio)
  {
    case RADIO_L: //internal SX1262
		case RADIO_M: ////auxillary SX1262
	  currfreq[radio] = freq;
		SX126X_SetRfFrequency(radio,(uint32_t)(freq / SX126X_SYNTH_STEP));
    return true;
		
		case RADIO_H:
		currfreq[radio] = freq;
		SX128X_SetRfFrequency(0,(uint32_t)(freq / SX128X_SYNTH_STEP));
		return true;
    
    default:
    return false;
  }
}

bool radio_setopmode(uint8_t radio,uint8_t opmode)
{
	if(opmode > 8) return false;
	else
	{
		switch(radio)
		{
			case RADIO_L: //internal SX1262
			case RADIO_M: ////auxillary SX1262
			SX126X_setopmode(radio,opmode);
			return true;
			
			case RADIO_H:
			SX128X_setopmode(0,opmode);
			return true; 
			
			default:
			return false; 
		}
	}
}

uint8_t radio_readregister(uint8_t radio,uint16_t reg)
{
  switch(radio)
  {
    case RADIO_L: //internal SX1262
		case RADIO_M: ////auxillary SX1262
		return SX126X_readReg(radio,reg);
		
		case RADIO_H:
		return SX128X_readReg(0,reg);
    
    default:
    return 0;
  }
}

void radio_writeregister(uint8_t radio,uint16_t reg,uint8_t value)
{
  switch(radio)
  {
    case RADIO_L: //internal SX1262
		case RADIO_M: ////auxillary SX1262
		SX126X_writeReg(radio,reg,value);
		break;
		
		case RADIO_H:
		SX128X_writeReg(0,reg,value);
		break;
    
    default:
    break;
  }
}

bool radio_geterrors(uint8_t radio,uint8_t *status,uint16_t *errors)
{
		switch(radio)
		{
			case RADIO_L: //internal SX1262
			case RADIO_M: ////auxillary SX1262
			SX126X_GetDeviceErrors(radio,status, errors);
			return true;
			
			case RADIO_H:
			
			return false; 
			
			default:
			return false; 
		}
}

bool radio_clearerrors(uint8_t radio)
{
		switch(radio)
		{
			case RADIO_L: //internal SX1262
			case RADIO_M: ////auxillary SX1262
			SX126X_ClearDeviceErrors(radio);
			return true;
			
			case RADIO_H:
			
			return false; 
			
			default:
			return false; 
		}
}

void display_rssi(uint32_t radio,uint32_t freq,float rssi)
{
	char strbuffer[64];

	sprintf(strbuffer,"%d:%d,%.1f  ",radio+1,freq,rssi);
	GUI_ShowString(0,16+16*radio,strbuffer,16,1);
}

void radio_getevents(uint8_t radio)
{
	switch(radio)
	{
		case 0:
		case 1:
		radioevents[radio] = SX126X_GetIrqStatus(radio);
		SX126X_ClearIrqStatus(radio,SX126X_ALL_IRQMSK);
		//SX126X_ClearDeviceErrors(radio);
		break;
		
		case 2:
		radioevents[2] = SX128X_GetIrqStatus(0);
		SX128X_ClearIrqStatus(0,SX128X_IRQ_ALL);
		break;
		
		default:
		break;
	}
}

void radio_handleevents(uint8_t radio)
{
	switch(radio)
	{
		case 0:
		case 1:
		//All modes
		if(radioevents[radio] & SX126X_TXDONE_IRQMSK)
		{
			radioevents[radio] &= ~SX126X_TXDONE_IRQMSK;
		}
		if(radioevents[radio] & SX126X_RXDONE_IRQMSK)
		{
			radioevents[radio] &= ~SX126X_RXDONE_IRQMSK;
		}
		if(radioevents[radio] & SX126X_RPEDET_IRQMSK)
		{
			radioevents[radio] &= ~SX126X_RPEDET_IRQMSK;
		}
		if(radioevents[radio] & SX126X_CRCERR_IRQMSK)
		{
			radioevents[radio] &= ~SX126X_CRCERR_IRQMSK;
		}
		if(radioevents[radio] & SX126X_TIMEOUT_IRQMSK)
		{
			radioevents[radio] &= ~SX126X_TIMEOUT_IRQMSK;
		}
		//FSK only
		if(radioevents[radio] & SX126X_SYNCDET_IRQMSK)
		{
			radioevents[radio] &= ~SX126X_SYNCDET_IRQMSK;
			//probably alarm signaling
		}
		//LoRa only
		if(radioevents[radio] & SX126X_HEADERDET_IRQMSK)
		{
			radioevents[radio] &= ~SX126X_HEADERDET_IRQMSK;
		}
		if(radioevents[radio] & SX126X_HEADERERR_IRQMSK)
		{
			radioevents[radio] &= ~SX126X_HEADERERR_IRQMSK;
		}
		if(radioevents[radio] & SX126X_CADDONE_IRQMSK)
		{
			radioevents[radio] &= ~SX126X_CADDONE_IRQMSK;
		}
		if(radioevents[radio] & SX126X_CADDET_IRQMSK)
		{
			radioevents[radio] &= ~SX126X_CADDET_IRQMSK;
		}
		break;
		
		case 2:
		//All modes
		if(radioevents[2] & SX128X_IRQ_TX_DONE)
		{
			radioevents[2] &= ~SX128X_IRQ_TX_DONE;
		}
		if(radioevents[2] & SX128X_IRQ_RX_DONE)
		{
			radioevents[2] &= ~SX128X_IRQ_RX_DONE;
		}
		if(radioevents[2] & SX128X_IRQ_PREAMBLE_DETECTED)
		{
			radioevents[2] &= ~SX128X_IRQ_PREAMBLE_DETECTED;
		}
		if(radioevents[2] & SX128X_IRQ_CRC_ERROR)
		{
			radioevents[2] &= ~SX128X_IRQ_CRC_ERROR;
		}
		if(radioevents[2] & SX128X_IRQ_RX_TX_TIMEOUT)
		{
			radioevents[2] &= ~SX128X_IRQ_RX_TX_TIMEOUT;
		}
		//FSK only
		if(radioevents[2] & SX128X_IRQ_SYNCWORD_VALID)
		{
			radioevents[2] &= ~SX128X_IRQ_SYNCWORD_VALID;
		}
		if(radioevents[2] & SX128X_IRQ_SYNCWORD_ERROR)
		{
			radioevents[2] &= ~SX128X_IRQ_SYNCWORD_ERROR;
		}
		//LoRa only
		if(radioevents[2] & SX128X_IRQ_HEADER_VALID)
		{
			radioevents[2] &= ~SX128X_IRQ_HEADER_VALID;
		}
		if(radioevents[2] & SX128X_IRQ_HEADER_ERROR)
		{
			radioevents[2] &= ~SX128X_IRQ_HEADER_ERROR;
		}
		if(radioevents[2] & SX128X_IRQ_CAD_DONE)
		{
			radioevents[2] &= ~SX128X_IRQ_CAD_DONE;
		}
		if(radioevents[2] & SX128X_IRQ_CAD_ACTIVITY_DETECTED)
		{
			radioevents[2] &= ~SX128X_IRQ_CAD_ACTIVITY_DETECTED;
		}
		break;
		
		default:
		break;
	}
	radioevents[2] = 0;
	radio_rx(2);
}


//DIO1 interrupt
void GPIO_IRQHandler(void)
{
  if(gpio_get_interrupt_status(SX1262_INT_PORT, SX1262_INT_PIN) == SET) 
  {
    gpio_clear_interrupt(SX1262_INT_PORT, SX1262_INT_PIN);
		radioflags[1] = true;
  }	
	if(gpio_get_interrupt_status(SX1280_INT_PORT, SX1280_INT_PIN) == SET) 
  {
    gpio_clear_interrupt(SX1280_INT_PORT, SX1280_INT_PIN);
		radioflags[2] = true;
  }
}

