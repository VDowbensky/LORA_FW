#include <stdio.h>
#include "app_cli.h"
#include "bsp.h"
#include "adc.h"
#include "flash.h"
#include "gui.h"
#include "radio.h"
//#include "rtc.h"

CommandState_t state;
char ciBuffer[256];

//uint8_t txmode = 0;

//General
void cli_reset(int argc, char **argv);
void cli_getuid(int argc, char **argv);
void cli_getver(int argc, char **argv);
void cli_getdevid(int argc, char **argv);
void cli_setdevid(int argc, char **argv);
//RF settings
void cli_getkfreq(int argc, char **argv);
void cli_setkfreq(int argc, char **argv);

void cli_getmodem(int argc, char **argv);
void cli_setmodem(int argc, char **argv);
void cli_getfreq(int argc, char **argv);
void cli_setfreq(int argc, char **argv);
void cli_getpower(int argc, char **argv);
void cli_setpower(int argc, char **argv);

void cli_getctune(int argc, char **argv);
void cli_setctune(int argc, char **argv);

void cli_getRSSI(int argc, char **argv);

//LoRa
void cli_lr_getmodparams(int argc, char **argv);
void cli_lr_setmodparams(int argc, char **argv);
void cli_lr_getpacketparams(int argc, char **argv);
void cli_lr_setpacketparams(int argc, char **argv);
void cli_lr_getsync(int argc, char **argv);
void cli_lr_setsync(int argc, char **argv);

//FSK
//baudrate,pulse shape,RXBW,deviation
void cli_fs_getmodparams(int argc, char **argv);
void cli_fs_setmodparams(int argc, char **argv);
//prelen, predetlen,synclen,addr.filt,varlen,paylen,crctype,whitening
void cli_fs_getpacketparams(int argc, char **argv);
void cli_fs_setpacketparams(int argc, char **argv);
//node and broadcast addr.
void cli_fs_getaddr(int argc, char **argv);
void cli_fs_setaddr(int argc, char **argv);
//sync word(s)
void cli_fs_getsync(int argc, char **argv);
void cli_fs_setsync(int argc, char **argv);
//CRC init, CRC poly, whitening init
void cli_fs_getcrcwhite(int argc, char **argv);
void cli_fs_setcrcwhite(int argc, char **argv);


//debug functions
void cli_readreg(int argc, char **argv);
void cli_writereg(int argc, char **argv);
void cli_initconfig(int argc, char **argv);
void cli_storeconfig(int argc, char **argv);

void cli_dumpregs(int argc, char **argv);
void cli_getstatus(int argc, char **argv);
void cli_setopmode(int argc, char **argv);
void cli_geterrors(int argc, char **argv);
void cli_clearerrors(int argc, char **argv);


//void cli_storeconfig(int argc, char **argv);

void cli_sendburst(int argc, char **argv);
void cli_stopburst (int argc, char **argv);
void cli_txstream(int argc, char **argv);

//device dependent
void cli_getvt(int argc, char **argv);
void cli_gettemp(int argc, char **argv);
void cli_getvbat(int argc, char **argv);

//special tests
void cli_sweeptx(int argc, char **argv);
void cli_sweeprx(int argc, char **argv);

void cli_gettime(int argc, char **argv);

CommandEntry_t commands[] =
  {
    //general
    COMMAND_ENTRY("RESET", "", cli_reset, "Device reset"),
    COMMAND_ENTRY("GET_UID", "", cli_getuid, "Get CPU ID"),
    COMMAND_ENTRY("GET_VERSION", "", cli_getver, "Get HW/FW version"),
    COMMAND_ENTRY("GET_DEVID", "", cli_getdevid, "Get device ID"),
    COMMAND_ENTRY("SET_DEVID", "w", cli_setdevid, "Get device ID"),
    //RF settings
    COMMAND_ENTRY("GET_MODEM", "w", cli_getmodem, ""),
    COMMAND_ENTRY("SET_MODEM", "ww", cli_setmodem, ""),
		
		COMMAND_ENTRY("GET_KFREQ", "w", cli_getkfreq, ""),
    COMMAND_ENTRY("SET_KFREQ", "ww", cli_setkfreq, ""),
		
    COMMAND_ENTRY("GET_FREQ", "w", cli_getfreq, "Get frequency settings"),
    COMMAND_ENTRY("SET_FREQ", "ww", cli_setfreq, "Set frequency settings"),
    COMMAND_ENTRY("GET_POWER", "w", cli_getpower, ""),
    COMMAND_ENTRY("SET_POWER", "ww", cli_setpower, ""),
    COMMAND_ENTRY("GET_CTUNE", "w", cli_getctune, ""),
    COMMAND_ENTRY("SET_CTUNE", "ww", cli_setctune, ""),

    COMMAND_ENTRY("GET_RSSI", "w", cli_getRSSI, ""),
    
    //LoRa
    COMMAND_ENTRY("LR_GET_MODPARAMS", "w", cli_lr_getmodparams, "Get LoRa modulation parameters"),
    COMMAND_ENTRY("LR_SET_MODPARAMS", "wwwww", cli_lr_setmodparams, "Set LoRa modulation parameters"),
    COMMAND_ENTRY("LR_GET_PACKETPARAMS", "w", cli_lr_getpacketparams, "Get LoRa packet parameters"),
    COMMAND_ENTRY("LR_SET_PACKETPARAMS", "wwwwww", cli_lr_setpacketparams, "Set LoRa packet parameters"),
    COMMAND_ENTRY("LR_GET_SYNC", "w", cli_lr_getsync, "Get LoRa sync word"),
    COMMAND_ENTRY("LR_SET_SYNC", "ww", cli_lr_setsync, "Set LoRa sync word"),
                        
    //FSK
    COMMAND_ENTRY("FS_GET_MODPARAMS", "w", cli_fs_getmodparams, "Get FSK modulation parameters"),
    COMMAND_ENTRY("FS_SET_MODPARAMS", "wwwww", cli_fs_setmodparams, "Set FSK modulation parameters"),
    COMMAND_ENTRY("FS_GET_PACKETPARAMS", "w", cli_fs_getpacketparams, "Get FSK packet parameters"),
    COMMAND_ENTRY("FS_SET_PACKETPARAMS", "wwwwwwwww", cli_fs_setpacketparams, "Set FSK packet parameters"),
    COMMAND_ENTRY("FS_GET_ADDR", "w", cli_fs_getaddr, "Get FSK node and broadcast addresses"),
    COMMAND_ENTRY("FS_SET_ADDR", "www", cli_fs_setaddr, "Set FSK node and broadcast addresses"),
    COMMAND_ENTRY("FS_GET_SYNC", "w", cli_fs_getsync, "Get FSK sync word"),
    COMMAND_ENTRY("FS_SET_SYNC", "wwwwwwwww", cli_fs_setsync, "Set FSK sync word"),
    COMMAND_ENTRY("FS_GET_CRCWH", "w", cli_fs_getcrcwhite, "Get FSK CRC/WH parameters"),
    COMMAND_ENTRY("FS_SET_CRCWH", "wwww", cli_fs_setcrcwhite, "Set FSK CRC/WH parameters"),

    //RF tests
    COMMAND_ENTRY("SEND_PACKET", "wwww", cli_sendburst, "Start packet burst"),
    COMMAND_ENTRY("STOP_TX", "w", cli_stopburst, "Stop packet burst"),
    COMMAND_ENTRY("TX_STREAM", "ww", cli_txstream, "Start/stop TX stream"),
    //System health - device dependent
    COMMAND_ENTRY("GET_VT", "", cli_getvt, "Get ADC data"),
    COMMAND_ENTRY("GET_TEMP", "w", cli_gettemp, ""),
		COMMAND_ENTRY("GET_VBAT", "w", cli_getvbat, ""),

    COMMAND_ENTRY("READ_REG", "ww", cli_readreg, ""),
    COMMAND_ENTRY("WRITE_REG", "www", cli_writereg, ""),
    COMMAND_ENTRY("DUMP_REGS", "www", cli_dumpregs, ""),
		COMMAND_ENTRY("GET_STATUS", "w", cli_getstatus, ""),
		COMMAND_ENTRY("SET_OPMODE", "ww", cli_setopmode, ""),
		
		COMMAND_ENTRY("GET_ERRORS", "w", cli_geterrors, ""),
		COMMAND_ENTRY("CLEAR_ERRORS", "w", cli_clearerrors, ""),
                        
    COMMAND_ENTRY("SWEEP_TX", "wwwwww", cli_sweeptx, "TX sweep"),
    COMMAND_ENTRY("SWEEP_RX", "wwwwww", cli_sweeprx, "RX scan"),

    COMMAND_ENTRY("INIT_CONFIG", "", cli_initconfig, ""),
		COMMAND_ENTRY("STORE_CONFIG", "", cli_storeconfig, ""),
		COMMAND_ENTRY("GET_TIME", "", cli_gettime, ""),
		
		COMMAND_ENTRY(NULL, NULL, NULL, NULL),
  };


void cli_init(void)
{
  ciInitState(&state, ciBuffer, sizeof(ciBuffer), commands);
}

void cli_proc(void)
{
  char input = RETARGET_ReadChar();
  if(input != '\0' && input != 0xFF) ciProcessInput(&state, &input, 1);
}

void ciErrorCallback(char* command, CommandError_t error)
{
  if (error == CI_UNKNOWN_COMMAND) 
	{
    printf("INVALID COMMAND\r\n");
  } 
	else if (error == CI_MAX_ARGUMENTS) 
	{
    printf("TOO MANY ARGUMENTS\r\n");
  } 
	else if (error == CI_INVALID_ARGUMENTS) 
	{
    printf("INVALID ARGUMENT\r\n");
  }
}


//cli functions implementation
//General
void cli_reset(int argc, char **argv)
{
  printf("RESET: OK\r\n");
	delay_ms(100);
	NVIC_SystemReset();
}

void cli_getuid(int argc, char **argv)
{
  uint64_t id;
	id = *(uint64_t*)0x4002002c;
	//id = EFC->SN_L | ((uint64_t)EFC->SN_H << 32);
	printf("GET_UID: 0x%llX\r\n", id);
}

void cli_getver(int argc, char **argv)
{
  printf("GET_VERSION: HW=%d,FW=%d.%d\r\n", HW_VERSION, FW_VERSION, FW_REVISION);
}

void cli_getdevid(int argc, char **argv)
{
  printf("GET_DEVID: TODO\r\n");
}

void cli_setdevid(int argc, char **argv)
{
//  uint16_t id;
//  id = ciGetUnsigned(argv[1]);
//  radioConfig.deviceID = id;
//  writeconfig();
	printf("SET_DEVID: TODO\r\n");
}


//RF settings
void cli_getmodem(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	uint8_t modem;
	if(radio_getmodem(radio,&modem) == true) printf("GET_MODEM: %u,%u\r\n",radio,modem);
	else printf("GET_MODEM: ERROR\r\n");
}

void cli_setmodem(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	uint8_t modem = ciGetUnsigned(argv[2]);
	if(radio_setmodem(radio,modem) == true) printf("SET_MODEM: %u,%u\r\n",radio,modem);
	else printf("SET_MODEM: ERROR\r\n");
}


void cli_getfreq(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	
	if(radio > 1)
	{
		printf("GET_FREQ: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L) printf("GET_FREQ: %u,%u\r\n",radio,globalrfconfig.LBconfig.rffreq / 1000);
	else printf("GET_FREQ: %u,%u\r\n",radio,globalrfconfig.HBconfig.rffreq / 1000);
}

void cli_setfreq(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	uint32_t freq = ciGetUnsigned(argv[2]);
	
	if(radio > 1)
	{
		printf("SET_FREQ: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L) globalrfconfig.LBconfig.rffreq = freq * 1000;
	else globalrfconfig.HBconfig.rffreq = freq * 1000;
	if(radio_set_frequency(radio,freq * 1000) == true) printf("SET_FREQ: %u,%u\r\n",radio,freq);
	else printf("SET_FREQ: %u,ERROR\r\n",radio);
	//updatescreen();
}


void cli_getpower(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	if(radio > 1)
	{
		printf("GET_POWER: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L) printf("GET_POWER: %d,%d dBm\r\n",radio,globalrfconfig.LBconfig.txpower);
	else 
	{
		if(globalrfconfig.HBconfig.rffreq > LR112X_SEPARATION_FREQ) printf("GET_POWER: %d,%d dBm\r\n",radio,globalrfconfig.HBconfig.txpower_hf);
		else printf("GET_POWER: %d,%d dBm\r\n",radio,globalrfconfig.HBconfig.txpower_subg);
	}
}

void cli_setpower(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	int8_t pwr = ciGetSigned(argv[2]);
	if(radio > 1)
	{
		printf("SET_POWER: INVALID RADIO\r\n");
		return;
	}
	if(radio_setpower(radio,pwr) == true) 
	{
		if(radio == RADIO_L) printf("SET_POWER: %d,%d dBm\r\n",radio,globalrfconfig.LBconfig.txpower);
		else 
		{
			if(globalrfconfig.HBconfig.rffreq > LR112X_SEPARATION_FREQ) printf("SET_POWER: %d,%d dBm\r\n",radio,globalrfconfig.HBconfig.txpower_hf);
			else printf("SET_POWER: %d,%d dBm\r\n",radio,globalrfconfig.HBconfig.txpower_subg);
		}
		radio_config(radio);
	}
	else printf("SET_POWER: ERROR\r\n");
}

void cli_getctune(int argc, char **argv)
{
//  uint8_t ctunea, ctuneb;

//  ctunea = SX126X_readReg(SX126X_REG_XTATRIM);
//  ctuneb = SX126X_readReg(SX126X_REG_XTBTRIM);
	printf("GET_CTUNE: NOT SUPPORTED\r\n");
}

void cli_setctune(int argc, char **argv)
{
//  uint8_t tune;

//  tune = ciGetUnsigned(argv[1]);
//  if(tune > 94) tune = 94;
//  RADIO_setctune(tune);
//  printf("SET_CTUNE: %d\r\n",tune);
	printf("SET_CTUNE: NOT SUPPORTED\r\n");
}


void cli_getRSSI(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	if(radio > 1)
	{
		printf("GET_RSSI: INVALID RADIO\r\n");
		return;
	}
	printf("GET_RSSI: %d,%.1f\r\n",radio,radio_get_rssi(radio));
}


//LoRa

void cli_lr_getmodparams(int argc, char **argv)
{
  float bw_kHz;
	uint8_t bw,sf,cr;
	bool opt;
	uint8_t radio = ciGetUnsigned(argv[1]);
	
	if(radio > 1) 
	{
		printf("LR_GET_MODPARAMS: INVALID RADIO\r\n");
		return;
	}
	switch(radio)
	{
		case RADIO_L:
		bw = globalrfconfig.LBconfig.bw;
		sf = globalrfconfig.LBconfig.sf;
		cr = globalrfconfig.LBconfig.cr;
		opt = globalrfconfig.LBconfig.ldropt;
		break;

		case RADIO_H:
		bw = globalrfconfig.HBconfig.bw;
		sf = globalrfconfig.HBconfig.sf;
		cr = globalrfconfig.HBconfig.cr;
		opt = globalrfconfig.HBconfig.ldropt;
		break;
		
		default:
		break;
	}
	if(bw == LORA_BW_7p8) bw_kHz = 7.8;
	else if(bw == LORA_BW_10p4) bw_kHz = 10.4;
	else if(bw == LORA_BW_15p6) bw_kHz = 15.6;
	else if(bw == LORA_BW_20p8) bw_kHz = 20.8;
	else if(bw == LORA_BW_31p3) bw_kHz = 31.3;
	else if(bw == LORA_BW_41p7) bw_kHz = 41.7;
	else if(bw == LORA_BW_62p5) bw_kHz = 62.5;
	else if(bw == LORA_BW_125) bw_kHz = 125.0;
	else if(bw == LORA_BW_250) bw_kHz = 250.0;
	else if(bw == LORA_BW_500) bw_kHz = 500.0;
	else if(bw == LORA_BW_203) bw_kHz = 203.0;
	else if(bw == LORA_BW_406) bw_kHz = 406.0;
	else if(bw == LORA_BW_812) bw_kHz = 812.0;
	else bw_kHz = -1.0;
 	printf("LR_GET_MODPARAMS: %d\r\nBW=%.1f\r\nSF=%d\r\n",radio,bw_kHz,sf);
	printf("CR:");
	switch(cr)
	{
		case 0:
    printf("OFF\r\n");
    break;
    case LORA_CR_4_5:
    printf("4_5\r\n");
    break;
    case LORA_CR_4_6:
    printf("4_6\r\n");
    break;
    case LORA_CR_4_7:
    printf("4_7\r\n");
    break;
    case LORA_CR_4_8:
    printf("4_8\r\n");
    break;
		case LORA_CR_4_5_LI:
    printf("LI_4_5\r\n");
    break;
		case LORA_CR_4_6_LI:
    printf("LI_4_6\r\n");
    break;
		case LORA_CR_4_8_LI:
    printf("LI_4_8\r\n");
    break;
    default:
    printf("INVALID\r\n");
    break;
	}
		printf("LROPT:");
		if(opt == true) printf("ON\r\n");
		else printf("OFF\r\n");
}

void cli_lr_setmodparams(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	uint16_t bw = ciGetUnsigned(argv[2]);
	uint8_t sf = ciGetUnsigned(argv[3]);
	uint8_t cr = ciGetUnsigned(argv[4]);
	uint8_t opt = ciGetUnsigned(argv[5]);
	
	uint8_t bw_val;
	float bw_kHz;
	bool lpopt;
	
	if(radio > 1) 
	{
		printf("LR_SET_MODPARAMS: INVALID RADIO\r\n");
		return;
	}
	
	if(radio == RADIO_L)
	{
    if(bw <= 8) {bw_val = LORA_BW_7p8; bw_kHz = 7.8;}
    else if(bw <= 11) {bw_val = LORA_BW_10p4; bw_kHz = 10.4;}
    else if(bw <= 16) {bw_val = LORA_BW_15p6; bw_kHz = 15.6;}
    else if(bw <= 21) {bw_val = LORA_BW_20p8; bw_kHz = 20.8;}
    else if(bw <= 31) {bw_val = LORA_BW_31p3; bw_kHz = 31.3;}
    else if(bw <= 42) {bw_val = LORA_BW_41p7; bw_kHz = 41.7;}
    else if(bw <= 63) {bw_val = LORA_BW_62p5; bw_kHz = 62.5;}
    else if(bw <= 125) {bw_val = LORA_BW_125; bw_kHz = 125.0;}
    else if(bw <= 250) {bw_val = LORA_BW_250; bw_kHz = 250.0;}
    else {bw_val = LORA_BW_500; bw_kHz = 500.0;}
		if(sf < 6) sf = 6;
		if(sf > 12) sf = 12;
		if(cr > 4) cr = 4;
		if(opt != 0) lpopt = true;
		else lpopt = false;
		globalrfconfig.LBconfig.bw = bw_val;
		globalrfconfig.LBconfig.sf = sf;
		globalrfconfig.LBconfig.cr = cr;
		globalrfconfig.LBconfig.ldropt = lpopt;
		SX126X_config(0,&globalrfconfig.LBconfig);
	}
	else //SX1280
	{
		if(bw <= 63) {bw_val = LORA_BW_62p5; bw_kHz = 62.5;}
    else if(bw <= 125) {bw_val = LORA_BW_125; bw_kHz = 125.0;}
		else if(bw <= 125) {bw_val = LORA_BW_125; bw_kHz = 125.0;}
    else if(bw <= 203) {bw_val = LORA_BW_203; bw_kHz = 203.0;}
		else if(bw <= 250) {bw_val = LORA_BW_250; bw_kHz = 250.0;}
		else if(bw <= 406) {bw_val = LORA_BW_406; bw_kHz = 406.0;}
		else if(bw <= 500) {bw_val = LORA_BW_500; bw_kHz = 500.0;}
		else {bw_val = LORA_BW_812; bw_kHz = 812.0;}
		if(sf < 5) sf = 5;
		if(sf > 12) sf = 12;
		if(cr > 7) cr = 7;
		if(opt != 0) lpopt = true;
		else lpopt = false;
		globalrfconfig.HBconfig.bw = bw_val;
		globalrfconfig.HBconfig.sf = sf;
		globalrfconfig.HBconfig.cr = cr;
		globalrfconfig.HBconfig.ldropt = lpopt;
		LR112X_config(0,&globalrfconfig.HBconfig);
	}
    printf("LR_SET_MODPARAMS: %d\r\nBW=%.1f\r\nSF=%d\r\n",radio,bw_kHz,sf);
    printf("CR:");
    switch(cr)
    {
      case 0:
      printf("OFF\r\n");
      break;
      case 1:
      printf("4_5\r\n");
      break;
      case 2:
      printf("4_6\r\n");
      break;
      case 3:
      printf("4_7\r\n");
      break;
      case 4:
      printf("4_8\r\n");
      break;
			case 5:
      printf("LI_4_5\r\n");
      break;
			case 6:
      printf("LI_4_6\r\n");
      break;
			case 7:
      printf("LI_4_8\r\n");
      break;
      default:
      printf("INVALID\r\n");
      break;
    }
    printf("LROPT:");
    if(lpopt == true) printf("ON\r\n");
    else printf("OFF\r\n");
}

void cli_lr_getpacketparams(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	
	if(radio > 1) 
	{
		printf("LR_GET_PACKETPARAMS: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L)
	{
	  printf("LR_GET_PACKETPARAMS: 0\r\nPRE_LEN=%d\r\nPAY_LEN=%d\r\nHEADER:",globalrfconfig.LBconfig.loraprelen,globalrfconfig.LBconfig.lorapaylen);
		if(globalrfconfig.LBconfig.header == true) printf("IMPLICIT\r\n");
		else printf("EXPLICIT\r\n");
  	printf("CRC:");
		if(globalrfconfig.LBconfig.loracrc == true) printf("ON\r\n");
		else printf("OFF\r\n");
		printf("INVERTIQ:");
		if(globalrfconfig.LBconfig.invertiq == true) printf("ON\r\n");
		else printf("OFF\r\n");
	}
	else
	{
	  printf("LR_GET_PACKETPARAMS: 2\r\nPRE_LEN=%d\r\nPAY_LEN=%d\r\nHEADER:",globalrfconfig.HBconfig.loraprelen,globalrfconfig.HBconfig.lorapaylen);
		if(globalrfconfig.HBconfig.implheader == true) printf("IMPLICIT\r\n");
		else printf("EXPLICIT\r\n");
  	printf("CRC:");
		if(globalrfconfig.HBconfig.loracrc == true) printf("ON\r\n");
		else printf("OFF\r\n");
		printf("INVERTIQ:");
		if(globalrfconfig.HBconfig.invertiq == true) printf("ON\r\n");
		else printf("OFF\r\n");
	}
}

void cli_lr_setpacketparams(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
  uint16_t pre = ciGetUnsigned(argv[2]) & 0xffff;
  uint8_t pay = ciGetUnsigned(argv[2]) & 0xff;
  uint8_t head = ciGetUnsigned(argv[4]) & 0x1;
  uint8_t crc = ciGetUnsigned(argv[5]) & 0x1;
  uint8_t inviq = ciGetUnsigned(argv[6]) & 0x1;
	
	if(radio > 1) 
	{
		printf("LR_SET_PACKETPARAMS: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L)
	{
    globalrfconfig.LBconfig.loraprelen = pre;
    globalrfconfig.LBconfig.lorapaylen = pay;
    if(head != 0) globalrfconfig.LBconfig.header = true;
    else globalrfconfig.LBconfig.header = false;
    if(crc != 0) globalrfconfig.LBconfig.loracrc = true;
    else globalrfconfig.LBconfig.loracrc = false;
    if(inviq != 0) globalrfconfig.LBconfig.invertiq = true;
    else globalrfconfig.LBconfig.invertiq = false;
    SX126X_config(0,&globalrfconfig.LBconfig);
    printf("LR_SET_PACKETPARAMS: 0\r\nPRE_LEN=%d\r\nPAY_LEN=%d\r\nHEADER:",globalrfconfig.LBconfig.loraprelen,globalrfconfig.LBconfig.lorapaylen);
    if(globalrfconfig.LBconfig.header == true) printf("IMPLICIT\r\n");
    else printf("EXPLICIT\r\n");
    printf("CRC:");
    if(globalrfconfig.LBconfig.loracrc == true) printf("ON\r\n");
    else printf("OFF\r\n");
    printf("INVERTIQ:");
    if(globalrfconfig.LBconfig.invertiq == true) printf("ON\r\n");
    else printf("OFF\r\n");
	}
	else
	{
    globalrfconfig.HBconfig.loraprelen = pre;
    globalrfconfig.HBconfig.lorapaylen = pay;
    if(head != 0) globalrfconfig.HBconfig.implheader = true;
    else globalrfconfig.HBconfig.implheader = false;
    if(crc != 0) globalrfconfig.HBconfig.loracrc = true;
    else globalrfconfig.HBconfig.loracrc = false;
    if(inviq != 0) globalrfconfig.HBconfig.invertiq = true;
    else globalrfconfig.HBconfig.invertiq = false;
    LR112X_config(0,&globalrfconfig.HBconfig);
    printf("LR_SET_PACKETPARAMS: 2\r\nPRE_LEN=%d\r\nPAY_LEN=%d\r\nHEADER:",globalrfconfig.HBconfig.loraprelen,globalrfconfig.HBconfig.lorapaylen);
    if(globalrfconfig.HBconfig.implheader == true) printf("IMPLICIT\r\n");
    else printf("EXPLICIT\r\n");
    printf("CRC:");
    if(globalrfconfig.HBconfig.loracrc == true) printf("ON\r\n");
    else printf("OFF\r\n");
    printf("INVERTIQ:");
    if(globalrfconfig.HBconfig.invertiq == true) printf("ON\r\n");
    else printf("OFF\r\n");
	}
}

void cli_lr_getsync(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	if(radio > 1) 
	{
		printf("LR_GET_SYNC: ERROR\r\n");
		return;
	}
  if(radio == RADIO_L) printf("LR_GET_SYNC: 0,0x%04X\r\n",globalrfconfig.LBconfig.lorasync);
	else printf("LR_GET_SYNC: 2,0x%04X\r\n",globalrfconfig.HBconfig.lorasync);
}

void cli_lr_setsync(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	uint16_t sync = ciGetUnsigned(argv[2]);
	if(radio > 1) 
	{
		printf("LR_SET_SYNC: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L)
	{
		globalrfconfig.LBconfig.lorasync = sync;
		SX126X_config(0,&globalrfconfig.LBconfig);
		printf("LR_SET_SYNC: 0,0x%04X\r\n",globalrfconfig.LBconfig.lorasync);
	}
	else
	{
		globalrfconfig.HBconfig.lorasync = sync;
		LR112X_config(0,&globalrfconfig.HBconfig);
		printf("LR_SET_SYNC: 2,0x%04X\r\n",globalrfconfig.HBconfig.lorasync);
	}
}


//FSK
//baudrate,pulse shape,RXBW,deviation
void cli_fs_getmodparams(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
  float bw_kHz;
	
	if(radio > 1) 
	{
		printf("FS_GETMODPARAMS: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L)
	{
		if(globalrfconfig.LBconfig.rbw == FSK_BW_4p8) bw_kHz = 4.8;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_5p8) bw_kHz = 5.8;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_7p3) bw_kHz = 7.3;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_11p7) bw_kHz = 11.7;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_19p5) bw_kHz = 19.5;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_23p4) bw_kHz = 23.4;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_29p3) bw_kHz = 29.3;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_39) bw_kHz = 39.0;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_46p9) bw_kHz = 46.9;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_58p6) bw_kHz = 58.6;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_78p2) bw_kHz = 78.2;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_93p8) bw_kHz = 93.8;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_117p3) bw_kHz = 117.3;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_156p2) bw_kHz = 156.2;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_187p2) bw_kHz = 187.2;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_234p3) bw_kHz = 234.3;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_312) bw_kHz = 312.0;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_373p6) bw_kHz = 373.6;
		else if(globalrfconfig.LBconfig.rbw == FSK_BW_467) bw_kHz = 467.0;
		else bw_kHz = -1.0;
	}
	else
	{
		if(globalrfconfig.HBconfig.rbw == FSK_BW_4p8) bw_kHz = 4.8;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_5p8) bw_kHz = 5.8;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_7p3) bw_kHz = 7.3;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_11p7) bw_kHz = 11.7;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_19p5) bw_kHz = 19.5;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_23p4) bw_kHz = 23.4;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_29p3) bw_kHz = 29.3;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_39) bw_kHz = 39.0;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_46p9) bw_kHz = 46.9;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_58p6) bw_kHz = 58.6;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_78p2) bw_kHz = 78.2;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_93p8) bw_kHz = 93.8;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_117p3) bw_kHz = 117.3;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_156p2) bw_kHz = 156.2;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_187p2) bw_kHz = 187.2;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_234p3) bw_kHz = 234.3;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_312) bw_kHz = 312.0;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_373p6) bw_kHz = 373.6;
		else if(globalrfconfig.HBconfig.rbw == FSK_BW_467) bw_kHz = 467.0;
		else bw_kHz = -1.0;
	}
	printf("FS_GETMODPARAMS: %d\r\nBR=%d\r\nSH=%d\r\nBW=%.1f\r\nDEV=%d\r\n",radio,globalrfconfig.LBconfig.br,globalrfconfig.LBconfig.shaping,bw_kHz,globalrfconfig.LBconfig.fdev);
}

void cli_fs_setmodparams(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
  uint32_t br = ciGetUnsigned(argv[2]);
  uint8_t sh = ciGetUnsigned(argv[3]); //4 - shaping
  uint32_t bw = ciGetUnsigned(argv[4]); //5 - RX BW
  uint32_t dev = ciGetUnsigned(argv[5]);
  float bw_kHz;
  uint8_t bw_val;
	
	if(radio > 1) 
	{
		printf("FS_SETMODPARAMS: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L)
	{
		if(br < 600) br = 600;
		if(br > 300000) br = 300000;
		if(dev < 600) dev = 600;
		if(dev > 200000) dev = 200000;
		if(bw <= 5) {bw_val = FSK_BW_4p8; bw_kHz = 4.8;}
		else if(bw <= 6) {bw_val = FSK_BW_5p8; bw_kHz = 5.8;}
		else if(bw <= 8) {bw_val = FSK_BW_7p3; bw_kHz = 7.3;}
		else if(bw <= 10) {bw_val = FSK_BW_9p7; bw_kHz = 9.7;}
		else if(bw <= 12) {bw_val = FSK_BW_11p7; bw_kHz = 11.7;}
		else if(bw <= 15) {bw_val = FSK_BW_14p6; bw_kHz = 14.6;}
		else if(bw <= 20) {bw_val = FSK_BW_19p5; bw_kHz = 19.5;}
		else if(bw <= 24) {bw_val = FSK_BW_23p4; bw_kHz = 23.4;}
		else if(bw <= 30) {bw_val = FSK_BW_29p3; bw_kHz = 29.3;}
		else if(bw <= 39) {bw_val = FSK_BW_39; bw_kHz = 39.0;}
		else if(bw <= 47) {bw_val = FSK_BW_46p9; bw_kHz = 46.9;}
		else if(bw <= 59) {bw_val = FSK_BW_58p6; bw_kHz = 58.6;}
		else if(bw <= 79) {bw_val = FSK_BW_78p2; bw_kHz = 78.2;}
		else if(bw <= 94) {bw_val = FSK_BW_93p8; bw_kHz = 93.8;}
		else if(bw <= 118) {bw_val = FSK_BW_117p3; bw_kHz = 117.3;}
		else if(bw <= 157) {bw_val = FSK_BW_156p2; bw_kHz = 156.2;}
		else if(bw <= 188) {bw_val = FSK_BW_187p2; bw_kHz = 187.2;}
		else if(bw <= 235) {bw_val = FSK_BW_234p3; bw_kHz = 234.3;}
		else if(bw <= 312) {bw_val = FSK_BW_312; bw_kHz = 312.0;}
		else if(bw <= 374) {bw_val = FSK_BW_373p6; bw_kHz = 373.6;}
		else  {bw_val = FSK_BW_467; bw_kHz = 467.0;}
	}
	if(radio == RADIO_L)
	{
		globalrfconfig.LBconfig.br = br;
		globalrfconfig.LBconfig.shaping = sh;
		globalrfconfig.LBconfig.rbw = bw_val;
		globalrfconfig.LBconfig.fdev = dev;
		SX126X_config(0,&globalrfconfig.LBconfig);
		
	}
	else
	{
		globalrfconfig.HBconfig.br = br;
		globalrfconfig.HBconfig.shaping = sh;
		globalrfconfig.HBconfig.rbw = bw_val;
		globalrfconfig.HBconfig.fdev = dev;
		LR112X_config(0,&globalrfconfig.HBconfig);
	}
	printf("FS_SETMODPARAMS: %d\r\nBR=%d\r\nSH=%d\r\nBW=%.1f\r\nDEV=%d\r\n",radio,globalrfconfig.LBconfig.br,globalrfconfig.LBconfig.shaping,bw_kHz,globalrfconfig.LBconfig.fdev);
}

//prelen, predetlen,synclen,addr.filt,varlen,paylen,crctype,whitening
void cli_fs_getpacketparams(int argc, char **argv) //bool FskVarLen, bool FskWhiteEn,uint8_t FskCrcType;
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	
	if(radio > 1) 
	{
		printf("FS_GET_PACKETPARAMS: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L)
	{
		uint16_t prelen;
		uint8_t predetlen;
		uint8_t synclen;
		uint8_t paylen;
		uint8_t addrcomp;
		bool varlen;
		uint8_t crc;
		bool white;
		
		prelen = globalrfconfig.LBconfig.fskprelen;
		predetlen = globalrfconfig.LBconfig.predetlen;
		synclen = globalrfconfig.LBconfig.fsksynclen;
		paylen = globalrfconfig.LBconfig.fskpaylen;
		addrcomp = globalrfconfig.LBconfig.addrcomp;
		varlen = globalrfconfig.LBconfig.varlen;
		crc = globalrfconfig.LBconfig.crctype;
		white = globalrfconfig.LBconfig.white;

		printf("FS_GET_PACKETPARAMS:%d\r\nPRE_LEN=%d\r\nPRE_DET=%d\r\nSYNC_LEN=%d\r\n",radio,prelen,predetlen,synclen);
		if(addrcomp == 0) printf("ADDR_COMP:OFF\r\n");
		else if(addrcomp == 1) printf("ADDR_COMP:NODE\r\n");
		else printf("ADDR_COMP:NODE_BR\r\n");
		if(varlen == true) printf("LEN:VAR\r\n"); 
		else printf("LEN:FIXED\r\n");
		printf("PAY_LEN=%d\r\n",paylen);
		switch(crc)
		{
    case 0:
      printf("CRC:1BYTE\r\n");
      break;
    case 1:
      printf("CRC:OFF\r\n");
      break;
    case 2:
      printf("CRC:2BYTES\r\n");
      break;
    case 4:
      printf("CRC:1BYTE_INV\r\n");
      break;
    case 6:
      printf("CRC:2BYTES_INV\r\n");
      break;
    default:
      printf("CRC:INVALID\r\n");
      break;
  }
  if(white == true) printf("WHITE:ON\r\n"); 
  else printf("WHITE:OFF\r\n"); 
	}
	else
	{
		printf("FS_GET_PACKETPARAMS: 2,TODO\r\n");
	}
}

void cli_fs_setpacketparams(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	
	if(radio > 1) 
	{
		printf("FS_SET_PACKETPARAMS: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L)
	{
		uint16_t prelen = ciGetUnsigned(argv[2]);
		uint8_t predetlen = ciGetUnsigned(argv[3]);
		uint8_t synclen = ciGetUnsigned(argv[4]);
		uint8_t addrfilt = ciGetUnsigned(argv[5]);
		bool varlen = (bool)ciGetUnsigned(argv[6]);
		uint8_t paylen = ciGetUnsigned(argv[7]);
		uint8_t crctype = ciGetUnsigned(argv[8]);
		bool whiteen = (bool)ciGetUnsigned(argv[9]);
		
		globalrfconfig.LBconfig.fskprelen = prelen;
		globalrfconfig.LBconfig.predetlen = predetlen;
		globalrfconfig.LBconfig.fsksynclen = synclen;
		globalrfconfig.LBconfig.addrcomp = addrfilt;
		globalrfconfig.LBconfig.varlen = varlen;
		globalrfconfig.LBconfig.fskpaylen = paylen;
		globalrfconfig.LBconfig.crctype = crctype;
		globalrfconfig.LBconfig.white = whiteen;
		SX126X_config(0,&globalrfconfig.LBconfig);
		printf("FS_SET_PACKETPARAMS:\r\nPRE_LEN=%d\r\nPRE_DET=%d\r\nSYNC_LEN=%d\r\n",prelen,predetlen,synclen);
		if(addrfilt == 0) printf("ADDR_COMP:OFF\r\n");
		else if(addrfilt == 1) printf("ADDR_COMP:NODE\r\n");
		else printf("ADDR_COMP:NODE_BR\r\n");
		if(varlen == true) printf("LEN:VAR\r\n"); 
		else printf("LEN:FIXED\r\n");
		printf("PAY_LEN=%d\r\n",paylen);
		switch(crctype)
		{
			case 0:
			printf("CRC:1BYTE\r\n");
			break;
			case 1:
			printf("CRC:OFF\r\n");
			break;
			case 2:
			printf("CRC:2BYTES\r\n");
			break;
			case 4:
			printf("CRC:1BYTE_INV\r\n");
			break;
			case 6:
			printf("CRC:2BYTES_INV\r\n");
			break;
			default:
			printf("CRC:INVALID\r\n");
			break;
		}
		if(whiteen == true) printf("WHITE:ON\r\n"); 
		else printf("WHITE:OFF\r\n"); 
	}
	else
	{
		printf("FS_SET_PACKETPARAMS: 2,TODO\r\n");
	}
}

//node and broadcast addr.
void cli_fs_getaddr(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	
	if(radio > 1) 
	{
		printf("FS_GET_ADDR: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L)
	{
		printf("FS_GET_ADDR: 0,0x%02X,0x%02X\r\n",globalrfconfig.LBconfig.nodeaddr,globalrfconfig.LBconfig.braddr);
	}
	else
	{
		printf("FS_GET_ADDR: 2,TODO\r\n");
	}
}

void cli_fs_setaddr(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	
	if(radio > 1) 
	{
		printf("FS_GET_ADDR: INVALID RADIO\r\n");
		return;
	}
	uint8_t nodeaddr = ciGetUnsigned(argv[2]);
	uint8_t braddr = ciGetUnsigned(argv[3]);
	
	if(radio == RADIO_L)
	{
		globalrfconfig.LBconfig.nodeaddr = nodeaddr;
		globalrfconfig.LBconfig.nodeaddr = braddr;
		SX126X_config(0,&globalrfconfig.LBconfig);
		printf("FS_SET_ADDR: %d,0x%02X,0x%02X\r\n",radio,nodeaddr,braddr);
	}
	else
	{
		globalrfconfig.HBconfig.nodeaddr = nodeaddr;
		globalrfconfig.HBconfig.nodeaddr = braddr;
		LR112X_config(0,&globalrfconfig.HBconfig);
		printf("FS_SET_ADDR: %d,0x%02X,0x%02X\r\n",radio,nodeaddr,braddr);
	}
}

//sync word(s)
void cli_fs_getsync(int argc, char **argv)
{
	uint8_t i;
	uint8_t radio = ciGetUnsigned(argv[1]);
	if(radio > 1) 
	{
		printf("FS_GET_SYNC: INVALID RADIO\r\n");
		return;
	}
	printf("FS_GET_SYNC: 0,");
	if(radio == RADIO_L)
	{
		for(i = 0; i < 7; i++) printf("0x%02X,",globalrfconfig.LBconfig.fsksync[i]);
		printf("0x%02X\r\n",globalrfconfig.LBconfig.fsksync[7]);
	}
	else
	{
		for(i = 0; i < 7; i++) printf("0x%02X,",globalrfconfig.HBconfig.fsksync[i]);
		printf("0x%02X\r\n",globalrfconfig.HBconfig.fsksync[7]);
	}
}

void cli_fs_setsync(int argc, char **argv)
{
	uint8_t i;
	uint8_t radio = ciGetUnsigned(argv[1]);
	if(radio > 1) 
	{
		printf("FS_SET_SYNC: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L)
	{
    for(i = 0; i < 8; i++) globalrfconfig.LBconfig.fsksync[i] = ciGetUnsigned(argv[i+2]);
    SX126X_config(0,&globalrfconfig.LBconfig);
    printf("FS_SET_SYNC: ");
    for(i = 0; i < 7; i++) printf("0x%02X,",globalrfconfig.LBconfig.fsksync[i]);
    printf("0x%02X\r\n",globalrfconfig.LBconfig.fsksync[7]);
	}
	else
	{
    for(i = 0; i < 8; i++) globalrfconfig.LBconfig.fsksync[i] = ciGetUnsigned(argv[i+2]);
    LR112X_config(0,&globalrfconfig.HBconfig);
    printf("FS_SET_SYNC: ");
    for(i = 0; i < 7; i++) printf("0x%02X,",globalrfconfig.HBconfig.fsksync[i]);
    printf("0x%02X\r\n",globalrfconfig.HBconfig.fsksync[7]);
	}
}
//CRC init, CRC poly, whitening init
void cli_fs_getcrcwhite(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	if(radio > 1) 
	{
		printf("FS_GET_CRCWH: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L)
	{
		printf("FS_GET_CRCWH: 0\r\nCRC_INIT=0x%04X\r\nCRC_POLY=0x%04X\r\nWH_INIT=0x%02X\r\n",globalrfconfig.LBconfig.crcinit,globalrfconfig.LBconfig.crcpoly,globalrfconfig.LBconfig.whiteinit);
	}
	else
	{
		printf("FS_GET_CRCWH: 1\r\nCRC_INIT=0x%04X\r\nCRC_POLY=0x%04X\r\nWH_INIT=0x%02X\r\n",globalrfconfig.HBconfig.crcinit,globalrfconfig.HBconfig.crcpoly,globalrfconfig.HBconfig.whiteinit);
	}
}

void cli_fs_setcrcwhite(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	uint16_t c_init = ciGetUnsigned(argv[2]);
	uint16_t c_poly = ciGetUnsigned(argv[3]);
	uint8_t w_init = ciGetUnsigned(argv[4]);
	if(radio > 1) 
	{
		printf("FS_SET_CRCWH: INVALID RADIO\r\n");
		return;
	}
	if(radio == RADIO_L)
	{
    globalrfconfig.LBconfig.crcinit = c_init;
    globalrfconfig.LBconfig.crcpoly = c_poly;
    globalrfconfig.LBconfig.whiteinit = w_init;
    SX126X_config(0,&globalrfconfig.LBconfig);
    printf("FS_SET_CRCWH: 0\r\nCRC_INIT=0x%04X\r\nCRC_POLY=0x%04X\r\nWH_INIT=0x%02X\r\n",globalrfconfig.LBconfig.crcinit,globalrfconfig.LBconfig.crcpoly,globalrfconfig.LBconfig.whiteinit);
	}
	else
	{
    globalrfconfig.HBconfig.crcinit = c_init;
    globalrfconfig.HBconfig.crcpoly = c_poly;
    globalrfconfig.HBconfig.whiteinit = w_init;
    SX126X_config(0,&globalrfconfig.LBconfig);
    printf("FS_SET_CRCWH: 1\r\nCRC_INIT=0x%04X\r\nCRC_POLY=0x%04X\r\nWH_INIT=0x%02X\r\n",globalrfconfig.HBconfig.crcinit,globalrfconfig.HBconfig.crcpoly,globalrfconfig.HBconfig.whiteinit);
	}
}

//System tests
void cli_sendburst(int argc, char **argv)
{
//  txpacketcount = ciGetUnsigned(argv[1]);
//  if(txpacketcount == 0) txpacketcount = 1;
//  inter_packet_delay = ciGetUnsigned(argv[2]);
//  slaveID = ciGetUnsigned(argv[3]);
//  if(inter_packet_delay < 100) inter_packet_delay = 100;
//  radio_startburst();
//  printf("SEND_PACKET: %d,%d\r\n",txpacketcount,inter_packet_delay);
//	updatescreen();
}

void cli_stopburst (int argc, char **argv)
{
//  if(master)
//  {
//    master = false;
//    tx_needed = false;
//    printf("STOP_TX: OK\r\n");
//  }
//  else printf("STOP_TX: ERROR\r\n");
//	updatescreen();
}


void cli_txstream(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	uint8_t stream = ciGetUnsigned(argv[2]);
	
	if(radio_stream(radio,stream) == true) printf("TX_STREAM: %u,%u\r\n",radio,stream);
	else printf("TX_STREAM: ERROR\r\n");
//	updatescreen();
}

void cli_sweeptx(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	uint32_t startfreq = ciGetUnsigned(argv[2]) * 1000;
	uint32_t stopfreq = ciGetUnsigned(argv[3]) * 1000;
	uint32_t step = ciGetUnsigned(argv[4]) * 1000;
	uint32_t us = ciGetUnsigned(argv[5]);
	uint8_t stream = ciGetUnsigned(argv[6]);
	
	//restrictions
	if(radio > 2) 
	{
		printf("SWEEP_TX: INVALID RADIO\r\n");
		return;
	}
	if(stopfreq < (startfreq+step))
	{
		printf("SWEEP_TX: %d,FREQ ERROR",radio);
		return;
	}
	if(step < MIN_FREQ_STEP)
	{
		printf("SWEEP_TX: %d,FREQ STEP ERROR",radio);
		return;
	}
	if((us < MIN_TX_SWEEP_TIME) || (us > MAX_TX_SWEEP_TIME))
	{
		printf("SWEEP_TX: %d,TIME INTERVAL ERROR",radio);
		return;
	}
	if(stream > 2)
	{
		printf("SWEEP_TX: %d,STREAM ERROR",radio);
		return;
	}
	if(radio_sweeptx(radio,startfreq,stopfreq,step,us,stream) == true)
	{
		//check result, on or off
		if(txmode[radio] == 0) printf("SWEEP_TX: %d,STOP\r\n",radio);
		else printf("SWEEP_TX: %d,START\r\n",radio);
	}
	else
	{
		printf("SWEEP_TX: %d,ERROR\r\n",radio);	
	}

}

void cli_sweeprx(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	uint32_t startfreq = ciGetUnsigned(argv[2]) * 1000;
	uint32_t stopfreq = ciGetUnsigned(argv[3]) * 1000;
	uint32_t step = ciGetUnsigned(argv[4]) * 1000;
	uint32_t ms = ciGetUnsigned(argv[5]);
	float rssitr = (float)ciGetSigned(argv[6]);

	//restrictions
	if(radio > 2) 
	{
		printf("SWEEP_RX: INVALID RADIO\r\n");
		return;
	}
	if(stopfreq < (startfreq+step))
	{
		printf("SWEEP_RX: %d,FREQ ERROR",radio);
		return;
	}
	if(step < MIN_FREQ_STEP)
	{
		printf("SWEEP_RX: %d,FREQ STEP ERROR",radio);
		return;
	}
	if((ms < MIN_RX_SWEEP_TIME) || (ms > MAX_RX_SWEEP_TIME))
	{
		printf("SWEEP_RX: %d,TIME INTERVAL ERROR",radio);
		return;
	}
	
	if(radio_scan(radio,startfreq,stopfreq,step,ms,rssitr) == true)
	{
		//check result, on or off
		if(sweeprx[radio] == false) printf("SWEEP_RX: %d,STOP\r\n",radio);
		else printf("SWEEP_RX: %d,START\r\n",radio);
	}
	else
	{
		printf("SWEEP_RX: %d,ERROR\r\n",radio);	
	}
}

void cli_getrssioffset(int argc, char **argv)
{
  printf("GET_RSSIOFFSET: TODO\r\n");
}

void cli_setrssioffset(int argc, char **argv)
{
  printf("SET_RSSIOFFSET: TODO\r\n");
}

//Device dependent commands
void cli_getvt(int argc, char **argv)
{
  printf("GET_VT: T=%.3f,V=%.3f\r\n",T,Vcc);
}


void cli_readreg(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	uint16_t reg = ciGetUnsigned(argv[2]);
	printf("READ_REG: 0x%02X\r\n",radio_readregister(radio,reg));
}

void cli_writereg(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	uint16_t reg = ciGetUnsigned(argv[2]);
	uint8_t val = ciGetUnsigned(argv[3]) & 0xff;

  radio_writeregister(radio,reg,val);
  printf("WRITE_REG: 0x%04X,0x%02X\r\n",reg,val);
}

void cli_dumpregs(int argc, char **argv)
{
  uint16_t i;
	uint8_t radio = ciGetUnsigned(argv[1]);
	uint16_t reg_l = ciGetUnsigned(argv[2]);
	uint16_t reg_h = ciGetUnsigned(argv[3]);
	
  printf("DUMP_REGS:\r\n"); 
  for(i = reg_l; i <= reg_h; i++)
  {
    printf("0x%04X,0x%02X\r\n",i,radio_readregister(radio,i));
  }
}

void cli_getstatus(int argc, char **argv)
{
	uint8_t status = 0;
	uint8_t mode = 0;
	uint8_t radio = ciGetUnsigned(argv[1]);
	
	if(radio_getstatus(radio,&mode,&status) == true) printf("GET_STATUS: %d,0x%02X,0x%02X\r\n",radio,mode,status);
	else printf("GET_STATUS: ERROR\r\n");
}

void cli_setopmode(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	uint8_t mode = ciGetUnsigned(argv[2]);
	if(radio_setopmode(radio,mode) == true) printf("SET_OPMODE: %d,%d\r\n",radio,mode);
	else printf("SET_OPMODE: ERROR\r\n");
}

void cli_geterrors(int argc, char **argv)
{
	uint16_t errors = 0;
	uint8_t status = 0;
	uint8_t radio = ciGetUnsigned(argv[1]);
	if(radio_geterrors(radio,&status,&errors) == true) printf("GET_ERRORS: %d,0x%04X\r\n",radio,errors);
	else printf("GET_ERRORS: ERROR\r\n");
}

void cli_clearerrors(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	if(radio_clearerrors(radio) == true) printf("CLEAR_ERRORS: OK\r\n");
	else printf("CLEAR_ERRORS: ERROR\r\n");
}

void cli_initconfig(int argc, char **argv)
{
  radio_initconfigs();
	printf("INIT_CONFIG: OK\r\n");
}

void cli_storeconfig(int argc, char **argv)
{
	writeconfig();
	printf("STORE_CONFIG: OK\r\n");
}

void cli_gettemp(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	if(radio != 1)
	{
		printf("GET_TEMP: ERROR\r\n");
	}
	else
	{
		printf("GET_TEMP: 1,%.1f\r\n",LR112X_GetTemp(0));
	}
}

void cli_getvbat(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
	if(radio != 1)
	{
		printf("GET_VBAT: ERROR\r\n");
	}
	else
	{
		printf("GET_VBAT: 1,%.1f\r\n",LR112X_GetVbat(0));
	}
}

void cli_getkfreq(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
		
	if(radio != 1)
	{
		printf("GET_KFREQ: ERROR\r\n");
	}
	else
	{
		printf("GET_KFREQ: 1,%.7f\r\n",globalrfconfig.HBconfig.K_freq);
	}
}

void cli_setkfreq(int argc, char **argv)
{
	uint8_t radio = ciGetUnsigned(argv[1]);
		
	if(radio != 1)
	{
		printf("SET_KFREQ: ERROR\r\n");
	}
	else
	{
		int16_t ppm = ciGetSigned(argv[2]); //in 0.1 ppm units
		float k = 1 + ((float)ppm / 10000000.0);
		globalrfconfig.HBconfig.K_freq = k;
		radio_config(1);
		printf("SET_KFREQ: 1,%.7f\r\n",globalrfconfig.HBconfig.K_freq);
	}
}

void cli_gettime(int argc, char **argv)
{
	//printf("GET_TIME: %d.%02d.%02d,%02d:%02d:%02d\r\n",timestamp.day,timestamp.month,timestamp.year,timestamp.hour,timestamp.minute,timestamp.second);
}

void updatescreen(void)
{
//		SSD1306_Clear(0);
//	sprintf(strbuffer, "Freq:%dkHz",radioConfig.RfFreq/1000);
//	GUI_ShowString(0,0,strbuffer,16,1);
//	sprintf(strbuffer, "Channel:%d",radioConfig.Channel);
//	GUI_ShowString(0,16,strbuffer,16,1);
//	sprintf(strbuffer, "Power:%ddBm",radioConfig.TxPowerDbm);
//	GUI_ShowString(0,32,strbuffer,16,1);
	//sprintf(strbuffer, "%02d:%02d:%02d",timestamp.hour,timestamp.minute,timestamp.second);
	//GUI_ShowString(0,0,strbuffer,16,1);
//	switch(opmode)
//	{
//    case SX126X_OPMODE_SLEEP:
//		sprintf(strbuffer, "Mode: Sleep");
//    break;

//    case SX126X_OPMODE_STBYRC:
//    sprintf(strbuffer, "Mode: StbyRC");
//    break;

//    case SX126X_OPMODE_STBYXOSC:
//    sprintf(strbuffer, "Mode: StbyXO");
//    break;

//    case SX126X_OPMODE_FS:
//    sprintf(strbuffer, "Mode: FS");
//    break;

//    case SX126X_OPMODE_TX:
//			sprintf(strbuffer, "Mode: TX packet");
//    break;

//    case SX126X_OPMODE_RX:
//    default:
//    sprintf(strbuffer, "Mode: RX");
//    break;

//    case SX126X_OPMODE_TXSTREAMCW:
//    sprintf(strbuffer, "Mode: TX CW");
//    break;

//    case SX126X_OPMODE_TXSTREAMPRE:
//    sprintf(strbuffer, "Mode: TX Pre");
//    break;
//	}
//	GUI_ShowString(0,48,strbuffer,16,1);
}

      


