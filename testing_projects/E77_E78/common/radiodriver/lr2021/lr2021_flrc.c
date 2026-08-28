// Flrc commands API

#include "lr2021_flrc.h"

//Sets the modulation parameters for FLRC packets. FW configures respective modem registers. Will return CMD_FAIL in the status of the next command, if the packet type is not FLRC
void LR2021_SetFlrcModParams(uint8_t lr,uint8_t flrc_bitrate,uint8_t flrc_cr,uint8_t pulse_shape)
{
  uint8_t buf[2];

  buf[0] = flrc_bitrate & 0x07;
  buf[1] = (flrc_cr & 0x0f) << 4;
  buf[1] |= pulse_shape & 0x0f;
  LR2021_writeCmd(lr,LR2021_SET_FLRC_MOD_PARAMS,buf,2);
}

//Sets the packet parameters for FLRC packets. FW configures respective modem registers
void LR2021_SetFlrcPktParams(uint8_t lr,uint8_t agc_pbl_len,uint8_t sync_len,uint8_t sync_tx,uint8_t sync_match,uint8_t pkt_format,uint8_t crc,uint16_t pld_len)
{
  uint8_t buf[4];

  buf[0] = (agc_pbl_len & 0x0f) << 2;
  buf[0] |= sync_len & 0x03;
  buf[0] |= (sync_tx & 0x03) << 6;
  buf[1] = (sync_match & 0x07) << 3;
  buf[1] |= (pkt_format & 0x01) << 2;
  buf[1] |= crc & 0x3;
  buf[2] = (pld_len >> 8) & 0xff;
  buf[3] = pld_len & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_FLRC_MOD_PARAMS,buf,2);
}

//Gets the internal statistics of the received packets. Statistics are reset on a POR, sleep without memory retention and the command ResetRxStats
void LR2021_GetFlrcRxStats(uint8_t lr,FlrcRxStats_t *stats)
{
  uint8_t buf[10];
  
  LR2021_readCmd(lr,LR2021_GET_FLRC_RX_STATS,NULL,0,buf,10);
  stats->pktcount = ((uint16_t)buf[0] << 8) | buf[1];
  stats->crcerrors = ((uint16_t)buf[2] << 8) | buf[3];
  stats->lenerrors = ((uint16_t)buf[4] << 8) | buf[5];
  stats->crcok = ((uint16_t)buf[6] << 8) | buf[7];
  stats->falsesync = ((uint16_t)buf[8] << 8) | buf[9];
}

//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone irq), but rssi_sync is already updated on SyncWordValid irq
void LR2021_GetFlrcPktStatus(uint8_t lr, FlrcPktStatus_t *status)
{
  uint8_t buf[5];
  
  LR2021_readCmd(lr,LR2021_GET_FLRC_PKT_STATUS,NULL,0,buf,4);
  status->pkt_len = ((uint16_t)buf[0] << 8) | buf[1];
  status->rssi_avg = ((((int16_t)buf[4]) >> 2) & 0x01) | (buf[2] << 1);
  status->rssi_sync = (((int16_t)buf[4]) & 0x01) | (buf[3] << 1);
  status->sw_num = buf[4];
}

//Sets the syncword for FLRC packets. 16 or 32 bits can be sent for the syncword. Bytes 5 and 6 are optional if only 16bit syncword is required
void LR2021_SetFlrcSync(uint8_t lr,uint8_t sw_num,uint8_t syncword)
{
  uint8_t buf[5];

  buf[0] = sw_num;
  buf[1] = (syncword >> 24) & 0xff;
  buf[2] = (syncword >> 16) & 0xff;
  buf[3] = (syncword >> 8) & 0xff;
  buf[4] = syncword & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_FLRC_SYNC,buf,5);
}
