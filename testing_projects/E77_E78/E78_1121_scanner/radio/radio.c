#include "radio.h"
#include "tremo_spi.h"
#include "flash.h"
#include "ustimer.h"
#include "gui.h"
#include "beeper.h"

globalrfconfig_t globalrfconfig;

bool txcomplete[2];
bool rxcomplete[2];

uint8_t opmodes[2];
uint8_t prevopmodes[2];

bool txmode[2];
bool sweeptx[2];
bool sweeprx[2];
float rssi_tr[2];
float rssi_peak[2];

uint32_t startfreq[2];
uint32_t stopfreq[2];
uint32_t freqstep[2];
uint32_t currfreq[2];
uint32_t prevfreq[2];

bool radioflags[2];
uint32_t radioevents[2];

void radio_proc(void)
{
	uint8_t radio;
	
		for(radio = 0; radio < 2; radio++)
	{
		if(radioflags[radio] == true)
		{
			radioflags[radio] = false;
			radio_getevents(radio);
			radio_handleevents(radio);
		}
	}
	
	for(radio = 0; radio < 2; radio++)
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
  globalrfconfig.LBconfig.startfreq = 800000000;
	globalrfconfig.LBconfig.stopfreq =  1000000000;
	globalrfconfig.LBconfig.scantimems = 10;
	globalrfconfig.LBconfig.freqstep = 100000;
	globalrfconfig.LBconfig.rssithreshold = -90;
	globalrfconfig.LBconfig.scan_active = true;
	
  globalrfconfig.LBconfig.txpower = 10;
  globalrfconfig.LBconfig.ramptime = SX126X_RAMP_10U;
  //LoRa
  globalrfconfig.LBconfig.sf = LORA_SF_8; //spreading factor
  globalrfconfig.LBconfig.bw = LORA_BW_41p7; //bandwidth
  globalrfconfig.LBconfig.cr = LORA_CR_4_5; //coding rate
  globalrfconfig.LBconfig.ldropt = false;
  globalrfconfig.LBconfig.loraprelen = 8; //???
  globalrfconfig.LBconfig.header = false;
  globalrfconfig.LBconfig.lorasync = 0xc96c;
  globalrfconfig.LBconfig.lorapaylen = 32;
  globalrfconfig.LBconfig.loracrc = true;
  globalrfconfig.LBconfig.invertiq = false;
  //FSK parameters
  globalrfconfig.LBconfig.br = 50000; //baud rate
  globalrfconfig.LBconfig.rbw = FSK_BW_93p8;
  globalrfconfig.LBconfig.shaping = FSK_BT0p5;
  globalrfconfig.LBconfig.fdev = 25000;
  globalrfconfig.LBconfig.fskprelen = 32;
	globalrfconfig.LBconfig.predetlen = 4;
  globalrfconfig.LBconfig.fsksynclen = 16;
  for (uint8_t i = 0; i < 8; i++) globalrfconfig.LBconfig.fsksync[i] = sync[i];//SX1280 needs 15 bytes
  globalrfconfig.LBconfig.addrcomp = FSK_ADDR_FILT_DISABLED;
  globalrfconfig.LBconfig.nodeaddr = 0; 
  globalrfconfig.LBconfig.braddr = 0xff; 
  globalrfconfig.LBconfig.varlen = false;
  globalrfconfig.LBconfig.fskpaylen = 32;
  globalrfconfig.LBconfig.crctype = FSK_CRC_2B;
  globalrfconfig.LBconfig.crcinit = 0xffff;
  globalrfconfig.LBconfig.crcpoly = 0x1021; //0x8005
  globalrfconfig.LBconfig.white = true;
  globalrfconfig.LBconfig.whiteinit = 0xaa; //???
	
	//High band (external LR1121)
	globalrfconfig.HBconfig.modem = LR112X_MODEM_LORA;
	globalrfconfig.HBconfig.K_freq = 1.0;
  globalrfconfig.HBconfig.startfreq = 1000000000;
	globalrfconfig.HBconfig.stopfreq = 2700000000;
	globalrfconfig.HBconfig.scantimems = 10;
	globalrfconfig.HBconfig.freqstep = 100000;
	globalrfconfig.LBconfig.rssithreshold = -90;
	globalrfconfig.HBconfig.scan_active = true;
	
  globalrfconfig.HBconfig.txpower_subg = 14; //-9 to +22
	globalrfconfig.HBconfig.txpower_hf = 13;
  globalrfconfig.HBconfig.ramptime = LR112X_PA_RAMP_16U;
  //LoRa
  globalrfconfig.HBconfig.sf = LORA_SF_8; //spreading factor
  globalrfconfig.HBconfig.bw = LORA_BW_203; //bandwidth
  globalrfconfig.HBconfig.cr = LORA_CR_4_5; //coding rate
  globalrfconfig.HBconfig.ldropt = false;
  globalrfconfig.HBconfig.loraprelen = 0x08; //???
  globalrfconfig.HBconfig.implheader = false;
  globalrfconfig.HBconfig.lorasync = 0x12; //private
  globalrfconfig.HBconfig.lorapaylen = 32;
  globalrfconfig.HBconfig.loracrc = true;
  globalrfconfig.HBconfig.invertiq = false;
  //FSK parameters
  globalrfconfig.HBconfig.br = 50000; //baud rate
  globalrfconfig.HBconfig.rbw = FSK_BW_93p8; //fake
  globalrfconfig.HBconfig.shaping = FSK_BT0p5;
  globalrfconfig.HBconfig.fdev = 25000; //to calculate modindex
  globalrfconfig.HBconfig.fskprelen = 32;
	globalrfconfig.HBconfig.predetlen = FSK_PREDET_8;
  globalrfconfig.HBconfig.fsksynclen = 16;
  for (uint8_t i = 0; i < 8; i++) globalrfconfig.HBconfig.fsksync[i] = sync[i];
  //configs[2].addrcomp = SX126X_FSK_ADDR_FILT_DISABLED;
	globalrfconfig.HBconfig.addrcomp = FSK_ADDR_FILT_DISABLED; //for SV1280 only
  globalrfconfig.HBconfig.nodeaddr = 0; 
  globalrfconfig.HBconfig.braddr = 0xff; 
  globalrfconfig.HBconfig.varlen = false;
  globalrfconfig.HBconfig.fskpaylen = 32;
  globalrfconfig.HBconfig.crctype = FSK_CRC_2B;
  globalrfconfig.HBconfig.crcinit = 0xffff;
  globalrfconfig.HBconfig.crcpoly = 0x1021; //0x8005
  globalrfconfig.HBconfig.white = LR112X_FSK_WHITE_126X;
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
	gpio_write(LR112X_RESET_PORT, EXTRF_POWER_PIN,GPIO_LEVEL_HIGH);
	//aux. LR1121
	gpio_init(LR112X_NSS_PORT, LR112X_NSS_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
	gpio_write(LR112X_NSS_PORT, LR112X_NSS_PIN,GPIO_LEVEL_HIGH);
	gpio_init(LR112X_RESET_PORT, LR112X_RESET_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
	gpio_write(LR112X_RESET_PORT, LR112X_RESET_PIN,GPIO_LEVEL_HIGH);
	gpio_init(LR112X_BUSY_PORT, LR112X_BUSY_PIN, GPIO_MODE_INPUT_PULL_DOWN);
	gpio_init(LR112X_INT_PORT, LR112X_INT_PIN, GPIO_MODE_INPUT_PULL_DOWN);
	//aux. SPI
	gpio_set_iomux(AUX_SCK_PORT, AUX_SCK_PIN, 4);
	gpio_set_iomux(AUX_MOSI_PORT, AUX_MOSI_PIN, 4);
	gpio_set_iomux(AUX_MISO_PORT, AUX_MISO_PIN, 4);
	//init SPI module
	ssp_init_t init_struct;
	rcc_enable_peripheral_clk(AUX_RCC_PERIPHERAL, true);
	//ssp_init_struct(&init_struct);
	init_struct.ssp_sclk      = 4000000; //???
	init_struct.ssp_role      = SSP_ROLE_MASTER;
	init_struct.ssp_format    = SSP_FRAME_FORMAT_SPI;
	init_struct.ssp_data_size = SSP_DATA_SIZE_8BIT;
	init_struct.ssp_clk_pol   = SPI_CLK_POLARITY_LOW;
	init_struct.ssp_clk_phase = SPI_CLK_PHASE_1EDGE;
	init_struct.ssp_dma_tx_en = DISABLE;
	init_struct.ssp_dma_rx_en = DISABLE;
	ssp_init(AUX_SSP, &init_struct);
	ssp_cmd(AUX_SSP, ENABLE);
	
	gpio_config_interrupt(LR112X_INT_PORT, LR112X_INT_PIN, GPIO_INTR_RISING_EDGE);
	gpio_config_interrupt(LR112X_INT_PORT, LR112X_INT_PIN, GPIO_INTR_RISING_EDGE);
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
		//SX126X_SetDIO2AsRfSwitchCtrl(0,true);
		SX126X_ClearDeviceErrors(0);
		currfreq[0] = globalrfconfig.LBconfig.startfreq;
		prevfreq[0] = currfreq[0];
		//SX126X_SetRx(0,0);
		radio_rx(0);
    return true;
		
		case RADIO_H:
		LR112X_reset(0);
		LR112X_config(0,&globalrfconfig.HBconfig); //default config is RX
		prevfreq[1] = globalrfconfig.HBconfig.startfreq;
		currfreq[1] = prevfreq[1];
		radio_rx(1);
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
    SX126X_SetSleep(0,0,false);
    return true;
    
    case RADIO_H:
		LR112X_SetSleep(0,LR112X_SLEEP_POWERDOWN,0);
		return true;
    
    default:
    return false;
  }
}

bool radio_getstatus(uint8_t radio,uint8_t *mode,uint8_t *status)
{
	uint8_t stat1,stat2;
	
	switch(radio)
  {
    case RADIO_L: //internal SX1262
		stat1 = SX126X_GetStatus(0);
		*mode = (stat1 >> 4) & 0x07;
		*status = (stat1 >> 1) & 0x07;
    return true;
    
		case RADIO_H:
		LR112X_GetStatus(0,&stat1,&stat2);
		
		*mode = (stat2 >> 1) & 0x07; //stat2
		//*status = stat1;
		//*mode = stat2;
		*status = (stat1 >> 1) & 0x07; //stat1
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
			SX126X_SetStandby(0,mode);
			if(mode == 0) opmodes[radio] = RADIO_OPMODE_STBYRC;
			else opmodes[radio] = RADIO_OPMODE_STBYXOSC;
			return true;
    
			case RADIO_H:
			LR112X_SetStandby(0,mode);
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
			SX126X_SetFs(0);
			return true;
    
			case RADIO_H:
			LR112X_SetFs(0);
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
		SX126X_SetRx(0,0);
		opmodes[0] = RADIO_OPMODE_RX;
    return true;
    
		case RADIO_H:
		LR112X_SetRx(0,0);
		opmodes[1] = RADIO_OPMODE_RX;
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
		SX126X_config(0,&globalrfconfig.LBconfig);	
		return true;
    
    case RADIO_H:
		LR112X_config(0,&globalrfconfig.HBconfig);
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
    SX126X_config(0,&globalrfconfig.LBconfig);
    return true;
		
    case RADIO_H:
		if(modem == 0) globalrfconfig.HBconfig.modem = LR112X_MODEM_FSK; 
		else if(modem == 1) globalrfconfig.HBconfig.modem = LR112X_MODEM_LORA; 
		else return false; //temp. not supported
		LR112X_config(0,&globalrfconfig.HBconfig);
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
    
    case RADIO_H:
		if(globalrfconfig.HBconfig.modem == LR112X_MODEM_FSK) *modem = 0; 
		else if(globalrfconfig.HBconfig.modem == LR112X_MODEM_LORA) *modem = 1; 
		//*modem = LR112X_GetPacketType(0);
		else return false; //temp. not supported
		return true;
    
    default:
    return false; //radio not exist
  }
}

//TX
bool radio_setpower(uint8_t radio,int8_t power)
{
	switch(radio)
  {
    case RADIO_L: //internal SX1262
		if((power > 22) || (power < -9)) return false;
		else
		{
			globalrfconfig.LBconfig.txpower = power;
			SX126X_SetTxParams(radio,globalrfconfig.LBconfig.txpower,globalrfconfig.LBconfig.ramptime);
			return true;
		}
    
    case RADIO_H:
		if(globalrfconfig.HBconfig.rffreq > LR112X_SEPARATION_FREQ) 
		{
			if((power > 13) || (power < -18)) return false;
			else globalrfconfig.HBconfig.txpower_hf = power;
		}
		else 
		{
			if((power > 22) || (power < -9)) return false;
			else globalrfconfig.HBconfig.txpower_subg = power;
		}
		LR112X_SetTxParams(0,power,globalrfconfig.HBconfig.ramptime);
		return true;
    
    default:
    return false; //radio not exist
  }
}

bool radio_senddata(uint8_t radio,uint8_t *txbuf,uint8_t len)
{
  //set data length if needed
  //fill tx buffer
  //start TX
	return true;
}

bool radio_txhandler(uint8_t radio) //call on tx complete
{
  return false;
}

bool radio_stream(uint8_t radio,uint8_t mode) //test function, CW or preamble. If mode==0, TX off.
{
  if(mode == 0) return radio_rx(radio);
  else
  {
  switch(radio)
    {
      case RADIO_L: //internal SX1262
			if(mode == 1) 
			{
				SX126X_SetCW(0);
				prevopmodes[0] = opmodes[0];
				opmodes[0] = RADIO_OPMODE_TXSTREAMCW;
			}
			else if(mode == 2) 
			{
				SX126X_SetTxInfinitePreamble(0);
				prevopmodes[radio] = opmodes[0];
				opmodes[0] = RADIO_OPMODE_TXSTREAMPRE;
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
				LR112X_SetTxCw(0);
				prevopmodes[1] = opmodes[radio];
				opmodes[1] = RADIO_OPMODE_TXSTREAMCW;
			}
			else if(mode == 2) 
			{
				LR112X_SetTxInfinitePreamble(0);
				prevopmodes[1] = opmodes[radio];
				opmodes[1] = RADIO_OPMODE_TXSTREAMPRE;
			}
			else return false;
			return true;
    
      default:
      return false;
    }
  }
}

bool radio_sweeptx(uint8_t radio,uint32_t start,uint32_t stop,uint32_t step,uint32_t us,uint8_t stream) //if stream==0,stop jamming
{
  if(radio > 2) return false;
	if(stop < (start+step)) return false;
	if(step < MIN_FREQ_STEP) return false;
	if((us < MIN_TX_SWEEP_TIME) || (us > MAX_TX_SWEEP_TIME)) return false;
	if(stream > 2) return false;
	
	//if stream == 0, stop and go to RX
	if(stream == 0)
	{
		ustimer_stop(radio);
		txmode[radio] = 0;
		sweeptx[radio] = false;
		radio_set_frequency(radio,prevfreq[radio]);
		currfreq[radio] = prevfreq[radio];
		radio_rx(radio);
		return true;
	}
	else
	{
		prevfreq[radio] = currfreq[radio];
		startfreq[radio] = start;
		stopfreq[radio] = stop;
		freqstep[radio] = step;
		txmode[radio] = stream;
		radio_set_frequency(radio,startfreq[radio]);
		currfreq[radio] = startfreq[radio];
		radio_stream(radio,stream);
		ustimer_setinterval(radio,us);
		ustimer_start(radio);
		sweeptx[radio] = true;
		return true;
	}
}

float radio_get_rssi(uint8_t radio)
{
	switch(radio)
    {
      case RADIO_L: //internal SX1262
			return -((float)SX126X_GetRssiInst(0)/2);
			
			case RADIO_H:
			return -((float)(LR112X_GetRssiInst(0)/2));

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
	if(globalrfconfig.HBconfig.scan_active == true) radio_scan(1,globalrfconfig.HBconfig.startfreq,
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
	radio_scan(1,globalrfconfig.HBconfig.startfreq,
																															globalrfconfig.HBconfig.stopfreq,
																															globalrfconfig.HBconfig.freqstep,
																															globalrfconfig.HBconfig.scantimems,
																															0);
	for(uint8_t i = 1; i < 2; i++) sweepflag[i] = false;
}  



bool radio_rxhandler(uint8_t radio,uint8_t *rxbuf,uint8_t len) //call if packet received
{
  switch(radio)
  {
    case RADIO_L: //internal SX1262
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
	  currfreq[0] = freq;
		SX126X_SetRfFrequency(0,(uint32_t)(freq / SX126X_SYNTH_STEP));
    return true;
		
		case RADIO_H:
		currfreq[1] = freq;
		LR112X_SetRfFrequency(0,freq);
		//temp.
		if(freq > LR112X_SEPARATION_FREQ)
		{
			LR112X_SetPaConfig(0,LR11XX_PA_SEL_HF,LR11XX_PA_REG_SUPPLY_VREG,LR11XX_PA_DUTYCYCLE_HF,LR11XX_PA_HPSEL_HF);
			LR112X_SetTxParams(0,globalrfconfig.HBconfig.txpower_hf,globalrfconfig.HBconfig.ramptime);
		}
		else
		{
			LR112X_SetPaConfig(0,LR11XX_PA_SEL_SUBG_HP,LR11XX_PA_REG_SUPPLY_VBAT,LR11XX_PA_DUTYCYCLE_SUBG,LR11XX_PA_HPSEL_SUBG);
			LR112X_SetTxParams(0,globalrfconfig.HBconfig.txpower_subg,globalrfconfig.HBconfig.ramptime);
		}
		LR112X_RssiCal(0,freq);
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
			SX126X_setopmode(radio,opmode);
			return true;
			
			case RADIO_H:
			LR112X_setopmode(0,opmode);
			return true; 
			
			default:
			return false; 
		}
	}
}

uint32_t radio_readregister(uint8_t radio,uint32_t reg)
{
	switch(radio)
  {
    case RADIO_L: //internal SX1262
		return SX126X_readReg(0,(uint16_t)reg);
		
		case RADIO_H:
		//LR112X_ReadRegMem32(0,reg, buf,4);	
		//return *(uint32_t*)buf;
		return LR112X_ReadReg32(0,reg);
    default:
    return 0;
  }
}

void radio_writeregister(uint8_t radio,uint32_t reg,uint32_t value)
{
  switch(radio)
  {
    case RADIO_L: //internal SX1262
		SX126X_writeReg(0,(uint16_t)reg,(uint8_t)value);
		break;
		
		case RADIO_H:
		//LR112X_WriteRegMem32(0,reg,(uint8_t*)&value,4);
		LR112X_WriteReg32(0,reg,value);
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
			SX126X_GetDeviceErrors(0,status, errors);
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
			SX126X_ClearDeviceErrors(0);
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

	sprintf(strbuffer,"%d:%d,%.1f  ",radio,freq,rssi);
	GUI_ShowString(0,16+16*radio,strbuffer,16,1);
}

void handleradioevents(uint8_t radio)
{
	switch(radio)
	{
		case 0:
		radioevents[radio] = SX126X_GetIrqStatus(radio);
		SX126X_ClearIrqStatus(radio,SX126X_ALL_IRQMSK);
		break;
		
		case 1:
		//
		
		//radioevents[radio] = LR112X_GetIrqStatus(radio);
		LR112X_ClearIrq(radio,LR112X_IRQMASK_ALL);
		break;
		
		default:
		break;
	}
}

//DIO1 interrupt
void GPIO_IRQHandler(void)
{
  if(gpio_get_interrupt_status(LR112X_INT_PORT, LR112X_INT_PIN) == SET) 
  {
    gpio_clear_interrupt(LR112X_INT_PORT, LR112X_INT_PIN);
		radioflags[1] = true;
  }	
}

void radio_getevents(uint8_t radio)
{
	switch(radio)
	{
		case 0:
		radioevents[radio] = SX126X_GetIrqStatus(0);
		SX126X_ClearIrqStatus(0,SX126X_ALL_IRQMSK);
		//SX126X_ClearDeviceErrors(radio);
		break;
		
		case 1:
		//radioevents[radio] = LR112X_GetIrqStatus(0);
		LR112X_ClearIrq(0,LR112X_IRQMASK_ALL);
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
		//All modes
		if(radioevents[0] & SX126X_TXDONE_IRQMSK)
		{
			
		}
		if(radioevents[0] & SX126X_RXDONE_IRQMSK)
		{
			
		}
		if(radioevents[0] & SX126X_RPEDET_IRQMSK)
		{
			
		}
		if(radioevents[0] & SX126X_CRCERR_IRQMSK)
		{
			
		}
		if(radioevents[0] & SX126X_TIMEOUT_IRQMSK)
		{
			
		}
		//FSK only
		if(radioevents[0] & SX126X_SYNCDET_IRQMSK)
		{
			
		}
		//LoRa only
		if(radioevents[0] & SX126X_HEADERDET_IRQMSK)
		{
			
		}
		if(radioevents[0] & SX126X_HEADERERR_IRQMSK)
		{
			
		}
		if(radioevents[0] & SX126X_CADDONE_IRQMSK)
		{
			
		}
		if(radioevents[0] & SX126X_CADDET_IRQMSK)
		{
			
		}
		break;

		case 1:
		//All modes
		if(radioevents[1] & LR112X_TX_DONE)
		{
			
		}
		if(radioevents[1] & LR112X_RX_DONE)
		{
			
		}
		if(radioevents[1] & LR112X_PRE_DET)
		{
			
		}
		if(radioevents[1] & LR112X_CRC_ERROR)
		{
			
		}
		if(radioevents[1] & LR112X_TIMEOUT)
		{
			
		}
		//FSK only
		if(radioevents[1] & LR112X_SYNC_DET)
		{
			
		}
		if(radioevents[1] & LR112X_FSK_LEN_ERROR)
		{
			
		}
		if(radioevents[1] & LR112X_FSK_ADDR_ERROR)
		{
			
		}
		//LoRa only
		if(radioevents[1] & LR112X_HEADER_ERROR)
		{
			
		}
		if(radioevents[1] & LR112X_CAD_DONE)
		{
			
		}
		if(radioevents[1] & LR112X_CAD_DETECTED)
		{
			
		}
		if(radioevents[1] & LR112X_CMD_ERROR)
		{
			
		}
		if(radioevents[1] & LR112X_ERROR)
		{
			
		}
		if(radioevents[1] & LR112X_LR_RX_TIMESTAMP)
		{
			
		}
		if(radioevents[1] & LR112X_LBD)
		{
			
		}
		break;

		default:
		break;
	}
	radioevents[radio] = 0;
}
