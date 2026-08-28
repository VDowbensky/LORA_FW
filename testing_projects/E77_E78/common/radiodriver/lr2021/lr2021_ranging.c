// Ranging commands API
#include "lr2021_ranging.h"

//Sets the ranging Id for this device (used in the slaves)
void LR112X_SetRangingAddr(uint8_t lr,uint32_t slave_addr,uint8_t check_length)
{
  uint8_t buf[5];

  buf[0] = (slave_addr >> 24) & 0xff;
  buf[1] = (slave_addr >> 16) & 0xff;
  buf[2] = (slave_addr >> 8) & 0xff;
  buf[3] = slave_addr & 0xff;
  buf[4] = check_length & 0x7;
  LR2021_writeCmd(lr,LR2021_SET_RANG_ADDR,buf,5);
}

//Sets the ranging Id for the requests (used in the master)
void LR112X_SetRangingReqAddr(uint8_t lr,uint32_t req_addr)
{
  uint8_t buf[4];

  buf[0] |= (req_addr >> 24) & 0xff;
  buf[1] |= (req_addr >> 16) & 0xff;
  buf[2] |= (req_addr >> 8) & 0xff;
  buf[3] |= req_addr & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_RANG_REQ_ADDR,buf,4);
}

//Gets the ranging result (For Master or spy only). Based on type parameter, different results are returned. The Distance [m] = rng1*150/(2^12*LoraBW), with LoraBW in MHz. For extended mode type=1 results, rng1 and rng2 values should be averaged to get a distance estimation unaffected by Doppler effect
void LR112X_GetRangingResultReq(uint8_t lr,uint8_t kind,RangingResult_t *result)
{
  uint8_t buf[8];
  
  LR2021_readCmd(lr,LR2021_GET_RANG_RESULT,&kind,1,buf,8);
  result->rng1 = ((uint32_t)buf[0] << 16) | (uint32_t)buf[1] << 8) | buf[2];
  result->rssi1 = buf[3];
  if(kind == 1)
  {
    result->rng2 = ((uint32_t)buf[4] << 16) | (uint32_t)buf[5] << 8) | buf[6];
    result->rssi2 = buf[7];
  }
}

//Sets the Tx->Rx delay for the ranging calibration
void LR112X_SetRangingRxTxDelay(uint8_t lr,uint32_t delay)
{
  uint8_t buf[4];

  buf[0] = (delay >> 24) & 0xff;
  buf[1] = (delay >> 16) & 0xff;
  buf[2] = (delay >> 8) & 0xff;
  buf[3] = delay & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_RANG_RXTX_DELAY,buf,4);
}

//Sets the ranging specific parameters
void LR112X_SetRangingParams(uint8_t lr,uint8_t extended_mode,uint8_t spy_mode,uint8_t nb_symbols) 
{
  uint8_t val;

  val = (extended_mode & 0x1) << 7;
  val |= (spy_mode & 0x1) << 6;
  val |= nb_symbols & 0x3f;
  LR2021_writeCmd(lr,LR2021_SET_RANG_PARAMS,&val,1);
}

//Gets the ranging counters for ranging exchanges. Statistics are reset on a POR, sleep without memory retention and the command ResetRxStats. Note: for extended ranging mode, the counters are incremented twice, once for each request/response
void LR112X_GetRangingStats(uint8_t lr,RangingStats_t *stats)
{
  uint8_t buf[10];
  
  LR2021_readCmd(lr,LR2021_GET_RANG_STATS,NULL,0,buf,14);
  stats->exchange_valid = ((uint16_t)buf[0] << 8) | buf[1];
  stats->request_valid = ((uint16_t)buf[2] << 8) | buf[3];
  stats->response_done = ((uint16_t)buf[4] << 8) | buf[5];
  stats->timeout = ((uint16_t)buf[6] << 8) | buf[7];
  stats->request_discarded = ((uint16_t)buf[8] << 8) | buf[9];
}
