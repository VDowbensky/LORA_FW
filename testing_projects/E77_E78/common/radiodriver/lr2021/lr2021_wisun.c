#include "lr2021_wisun.h"

// Wisun commands API
//Configure the wisun mode (1a, 1b, 2a, 2b, 3, 4a, 4b, 5)
void LR2021_SetWisunMode(uint8_t lr,uint8_t wisun_mode,uint8_t rx_bw) 
{
  uint8_t buf[2];

  buf[0] = wisun_mode & 0x7;
  buf[1] = rx_bw;
  LR2021_writeCmd(lr,LR2021_SET_WISUN_MODE,buf,2);
}

//Configure the wisun packet parameters
void LR2021_SetWisunPktParams(uint8_t lr,uint8_t whitening,uint8_t crc_on,uint8_t mode_switch_tx,uint8_t fec_tx,uint16_t frame_len_tx,uint8_t pbl_len_tx)
{
  uint8_t buf[4];

  buf[0] = (fcs_tx & 0x1) << 5;
  buf[0] |= (whitening & 0x1) << 4;
  buf[0] |= (crc_on & 0x1) << 3;
  buf[0] |= (mode_switch_tx & 0x1) << 2;
  buf[0] |= fec_tx & 0x3;
  buf[1] = (frame_len_tx >> 8) & 0xff;
  buf[2] = frame_len_tx & 0xff;
  buf[3] = pbl_len_tx;
  LR2021_writeCmd(lr,LR2021_SET_WISUN_PKT_PARAMS,buf,4);
}

//Configure the wisun packet parameters
void LR2021_SetWisunPktParamsAdv(uint8_t lr,uint8_t whitening,uint8_t crc_on,uint8_t mode_switch_tx,uint8_t fec_tx,uint16_t frame_len_tx,uint8_t pbl_len_tx,uint8_t pbl_detect)
{
  uint8_t buf[5];

  buf[0] = (fcs_tx & 0x1) << 5;
  buf[0] |= (whitening & 0x1) << 4;
  buf[0] |= (crc_on & 0x1) << 3;
  buf[0] |= (mode_switch_tx & 0x1) << 2;
  buf[0] |= fec_tx & 0x3;
  buf[1] = (frame_len_tx >> 8) & 0xff;
  buf[2] = frame_len_tx & 0xff;
  buf[3] = pbl_len_tx;
  buf[4] = pbl_detect;
  LR2021_writeCmd(lr,LR2021_SET_WISUN_PKT_PARAMS,buf,5);
}

//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone irq), but syncword_idx and rssi_sync are already updated on SyncWordValid irq
void LR2021_GetWisunPktStatus(uint8_t lr,WisunPktStatus_t *status)
{
  uint8_t buf[8];
  
  LR2021_readCmd(lr,LR2021_GET_WISUN_PKT_STATUS,NULL,0,buf,8);
  status->header = ((uint16_t)buf[0] << 8) | buf[1];
  status->pktlen = ((uint16_t)buf[2] << 8) | buf[3];
  status->rssi_avg = (((int16_t)buf[7] >> 2) & 0x01) | ((int16_t)buf[4] << 1);
  status->rssi_sync((int16_t)buf[7] & 0x01) | ((int16_t)buf[5] << 1);
  if(buf[6] & 0x80) status->syncword_idx = true;// Index of detected syncword (0/1)
  else status->syncword_idx = false;
  status->lqi = buf[8];
}

//Sets length of frame for TX for normal packets, or header value for mode_switch packets
void LR2021_SetWisunPktLen(uint8_t lr,uint16_t frame_len_tx)
{
  uint8_t buf[2];

  buf[0] = (frame_len_tx >> 8) & 0xff;
  buf[1] = frame_len_tx & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_WISUN_PKT_LEN,buf,2);
}
