// Raw commands API
#include "lr2021_raw.h"

//Sets the parameters used for TX. Note: for mode = IQ, tx_sample_num is the number of IQ pairs, so the number of bytes to write into the TX FIFO is 2*tx_sample_num
void LR2021_SetRawIqTxParams(uint8_t lr,uint16_t tx_sample_num,uint32_t tx_sample_rate,uint8_t tx_mode)
{
  uint8_t buf[7];

  buf[0] = (tx_sample_num >> 8) & 0xff;
  buf[1] = tx_sample_num & 0xff;
  buf[2] = (tx_sample_rate >> 24) & 0xff;
  buf[3] = (tx_sample_rate >> 16) & 0xff;
  buf[4] = (tx_sample_rate >> 8) & 0xff;
  buf[5] = tx_sample_rate & 0xff;
  buf[6] = tx_mode & 0x03;
  LR2021_writeCmd(lr,LR2021_SET_RAWIQ_TX_PARAMS,buf,7);
}

//Sets the raw IQ capture trigger parameters
void LR2021_SetRawIqTrigger(uint8_t lr,uint8_t trigger_start,uint8_t trigger_stop)
{
  uint8_t val;

  val = ((trigger_start & 0x0f) << 4;
  val |= (trigger_stop & 0x0f;
  LR2021_writeCmd(lr,LR2021_SET_RAWIQ_TRIG,&val,1);
}

//Sets the raw IQ capture trigger parameters
void LR2021_SetRawIqTriggerAdv(uint8_t lr,uint8_t trigger_start,uint8_t trigger_stop,uint16_t rssi_up,uint16_t rssi_down)
{
  uint8_t buf[4];

  buf[0] = (trigger_start & 0x0f) << 4;
  buf[0] |= trigger_stop & 0x0f;
  buf[1] = (rssi_up >> 1) & 0xff;
  buf[3] = rssi_up & 0xff;
  buf[2] = (rssi_down >> 1) & 0xff;
  buf[3] |= (rssi_down & 0xff) << 1;
  LR2021_writeCmd(lr,LR2021_SET_RAWIQ_TRIG,&val,1);
}
