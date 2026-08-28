#ifndef _RADIO_H_
#define _RADIO_H_

#include "bsp.h"
#include "sx126x.h"
#include "sx128x.h"
//#include "lr112x.h"
#include "sx126x_config.h"
#include "sx128x_config.h"
#include "rfconfig.h"

//Data buffers
#define TX_BUFFER_SIZE 256
#define RX_BUFFER_SIZE 256
#define RSSI_BUFFER_SIZE 256 //*8 = 2048 2 KB each buffer

#define RF_CONFIG_SIZE	1024

#define RADIO_L		0 //ASR6601 internal
#define RADIO_M		1 //aux. SX1262
#define RADIO_H			2 //aux. SX1280

#define RADIO_OPMODE_SLEEP						0
#define RADIO_OPMODE_STBYRC						2
#define RADIO_OPMODE_STBYXOSC         3
#define RADIO_OPMODE_FS               4
#define RADIO_OPMODE_RX               5
#define RADIO_OPMODE_TX               6
#define RADIO_OPMODE_TXSTREAMCW       7
#define RADIO_OPMODE_TXSTREAMPRE      8

#define MIN_FREQ_STEP									1000//Hz
#define MIN_TX_SWEEP_TIME							10
#define MAX_TX_SWEEP_TIME							60000 //us
#define MIN_RX_SWEEP_TIME							1
#define MAX_RX_SWEEP_TIME							60 //ms


typedef struct rssirecord
{
  uint32_t freguency;
  float rssi;
}rssirecord_t; //8 bytes

void radio_proc(void);

void radio_interface_init(void);
void radio_initconfigs(void);
bool radio_init(uint8_t radio);
bool radio_on(uint8_t radio);
bool radio_off(uint8_t radio);
bool radio_reset(uint8_t radio);
bool radio_config(uint8_t radio);
bool radio_setmodem(uint8_t radio,uint8_t modem);
bool radio_getmodem(uint8_t radio,uint8_t *modem);

bool radio_getstatus(uint8_t radio,uint8_t *mode,uint8_t *status);
bool radio_rx(uint8_t radio);
bool radio_setopmode(uint8_t radio,uint8_t opmode);
bool radio_standby(uint8_t radio,uint8_t mode);
bool radio_fs(uint8_t radio);
bool radio_senddata(uint8_t radio,uint8_t *txbuf,uint8_t len);
bool radio_setpower(uint8_t radio,int8_t power);
bool radio_stream(uint8_t radio,uint8_t mode); //test function, CW or preamble. If mode==0, TX off.
bool radio_sweeptx(uint8_t radio,uint32_t startfreq,uint32_t stopfreq,uint32_t step,uint32_t us,uint8_t stream);
float radio_get_rssi(uint8_t radio);
bool radio_scan(uint8_t radio,uint32_t start,uint32_t stop,uint32_t step,uint32_t ms,float tr);
bool radio_receivedata(uint8_t radio,uint8_t *rxbuf,uint8_t len);
bool radio_set_frequency(uint8_t radio, uint32_t freqHz);

uint8_t radio_readregister(uint8_t radio,uint16_t reg);
void radio_writeregister(uint8_t radio,uint16_t reg,uint8_t value);

bool radio_geterrors(uint8_t radio,uint8_t *status,uint16_t *errors);
bool radio_clearerrors(uint8_t radio);
void radio_getevents(uint8_t radio);
void radio_handleevents(uint8_t radio);

void display_rssi(uint32_t radio,uint32_t freq,float rssi);

extern bool sweeptx[];
extern bool sweeprx[];
extern bool txmode[];

extern bool radioflags[];
extern uint16_t radioevents[];

#endif
