#ifndef _LR2021_RANGING_H_
#define _LR2021_RANGING_H_

//Ranging commands
#define LR2021_SET_RANG_ADDR          0x0278
#define LR2021_SET_RANG_REQ_ADDR      0x0279
#define LR2021_GET_RANG_RESULT        0x027a
#define LR2021_SET_RANG_RXTX_DELAY    0x027b
#define LR2021_SET_RANG_PARAMS        0x027c
#define LR2021_GET_RANG_STATS         0x027d
//Type of ranging result to return
#define LR2021_RESULT_LATEST_RAW      0
#define LR2021_RESULT_EXTENDED_RAW    1
#define LR2021_RESULT_GAIN_STEPS      2
//Extended mode enable: 0=disabled, 1=enabled
#define LR2021_EXTMODE_DISABLED       0
#define LR2021_EXTMODE_ENABLED        1
/// Spy mode enable: Set to 1 to enable spy mode on the next set_rx. In SPY mode, the radio acts like a normal device in slave mode and will send a response to any request for its address, but will also listen to requests/responses pairs not addressed to it
#define LR2021_SPYMODE_DISABLED       0
#define LR2021_SPYMODE_ENABLED        1

typedef struct
{
  uint32_t rng1;
  uint8_t rssi1;
  uint32_t rng2;
  uint8_t rssi2;
}RangingResult_t;

typedef struct
{
  uint16_t exchange_valid;
  uint16_t request_valid;
  uint16_t response_done;
  uint16_t timeout;
  uint16_t request_discarded;
}RangingStats_t;

//Sets the ranging Id for this device (used in the slaves)
void LR112X_SetRangingAddr(uint8_t lr,uint32_t slave_addr,uint8_t check_length);
//Sets the ranging Id for the requests (used in the master)
void LR112X_SetRangingReqAddr(uint8_t lr,uint32_t req_addr);
//Gets the ranging result (For Master or spy only). Based on type parameter, different results are returned. The Distance [m] = rng1*150/(2^12*LoraBW), with LoraBW in MHz. For extended mode type=1 results, rng1 and rng2 values should be averaged to get a distance estimation unaffected by Doppler effect
void LR112X_GetRangingResultReq(uint8_t lr,uint8_t kind,RangingResult_t *result);
//Sets the Tx->Rx delay for the ranging calibration
void LR112X_SetRangingRxTxDelay(uint8_t lr,uint32_t delay);
//Sets the ranging specific parameters
void LR112X_SetRangingParams(uint8_t lr,uint8_t extended_mode,uint8_t spy_mode,uint8_t nb_symbols);
//Gets the ranging counters for ranging exchanges. Statistics are reset on a POR, sleep without memory retention and the command ResetRxStats. Note: for extended ranging mode, the counters are incremented twice, once for each request/response
void LR112X_GetRangingStats(uint8_t lr,RangingStats_t *stats);

#endif


