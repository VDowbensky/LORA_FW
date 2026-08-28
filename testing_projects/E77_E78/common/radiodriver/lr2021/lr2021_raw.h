#ifndef _LR2021_RAW_H_
#define _LR2021_RAW_H_

//Raw I/Q commands
#define LR2021_SET_RAWIQ_TX_PARAMS    0x0293
#define LR2021_SET_RAWIQ_TRIG         0x0294
//TX mode selection
#define LR2021_RAW_TXMODE_IQ                0
#define LR2021_RAW_TXMODE_FREQ              1
#define LR2021_RAW_TXMODE_PHASE             2
//Trigger selection for the Raw IQ capture start
#define LR2021_RAW_TRIG_START_SOFT          0
#define LR2021_RAW_TRIG_START_RSSI          1
#define LR2021_RAW_TRIG_START_MODEMEXT      2
#define LR2021_RAW_TRIG_START_PREDET        3
#define LR2021_RAW_TRIG_START_RXDONE        4
#define LR2021_RAW_TRIG_START_SYNCFOUND     5
#define LR2021_RAW_TRIG_START_HICIRQ3       6
#define LR2021_RAW_TRIG_START_RTC           7
//Trigger selection for the Raw IQ capture stop. Automatically stopped on Rx stop
#define LR2021_RAW_TRIG_STOP_SOFT           0
#define LR2021_RAW_TRIG_STOP_RSSI           1
#define LR2021_RAW_TRIG_STOP_MODEMEXT       2
#define LR2021_RAW_TRIG_STOP_PREDET         3
#define LR2021_RAW_TRIG_STOP_RXDONE         4
#define LR2021_RAW_TRIG_STOP_SYNCFOUND      5
#define LR2021_RAW_TRIG_STOP_HICIRQ3        6
#define LR2021_RAW_TRIG_STOP_RTC            7

//Sets the parameters used for TX. Note: for mode = IQ, tx_sample_num is the number of IQ pairs, so the number of bytes to write into the TX FIFO is 2*tx_sample_num
void LR2021_SetRawIqTxParams(uint8_t lr,uint16_t tx_sample_num,uint32_t tx_sample_rate,uint8_t tx_mode);
//Sets the raw IQ capture trigger parameters
void LR2021_SetRawIqTrigger(uint8_t lr,uint8_t trigger_start,uint8_t trigger_stop);
//Sets the raw IQ capture trigger parameters
void LR2021_SetRawIqTriggerAdv(uint8_t lr,uint8_t trigger_start,uint8_t trigger_stop,uint16_t rssi_up,uint16_t rssi_down);


#endif
