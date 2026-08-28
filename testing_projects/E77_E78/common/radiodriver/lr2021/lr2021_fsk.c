// Fsk commands API
#include "lr2021_fsk.h"

//Sets the modulation parameters for FSK packets. FW configures respective modem registers. Will return CMD_FAIL in the status of the next command, if the packet type is not FSK
void LR2021_SetFskModParams(uint8_t lr,uint32_t bitrate,uint8_t pulse_shape,uint8_t rx_bw,uint32_t fdev)
{
  uint8_t buf[9];
  
  buf[0] = (bitrate >> 24) & 0xff;
  buf[1] = (bitrate >> 16) & 0xff;
  buf[2] = (bitrate >> 8) & 0xff;
  buf[3] = bitrate & 0xff;
  buf[4] = pulse_shape & 0x0f;
  buf[5] = rx_bw;
  buf[6] = (fdev >> 16) & 0xff;
  buf[7] = (fdev >> 8) & 0xff;
  buf[8] = fdev & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_FSK_MOD_PARAMS,buf,9);
}

//Sets the packet parameters for FSK packets.. Command will fail if packet type is not FSK
void LR2021_SetFskPktParams(uint8_t lr,uint16_t pbl_len_tx,uint8_t pbl_len_detect,bool pbl_long,uint8_t pld_len_unit,uint8_t addr_comp,
                            uint8_t fsk_pkt_format,uint16_t pld_len,uint8_t crc,uint8_t dc_free)
{
  uint8_t buf[7];

  buf[0] = ((pbl_len_tx >> 8) & 0xFF) as u8;
  buf[1] = (pbl_len_tx & 0xFF) as u8;
  buf[2] = pbl_len_detect as u8;
  buf[3] = ((pld_len_unit as u8) & 0x1) << 4;
  if(pbl_long) buf[3] |= 16;
  buf[3] |= ((addr_comp as u8) & 0x3) << 2;
  buf[3] |= (fsk_pkt_format as u8) & 0x3;
  buf[4] = ((pld_len >> 8) & 0xFF) as u8;
  buf[5] = (pld_len & 0xFF) as u8;
  buf[6] = ((crc as u8) & 0xF) << 4;
  buf[6] |= dc_free & 0xF;
  LR2021_writeCmd(lr,LR2021_SET_FSK_PKT_PARAMS,buf,9);
}

//Configure the whitening params for FSK packets, SX126x/LR11xx or SX128x compatible
void LR2021_SetFskWhiteParams(uint8_t lr,uint8_t whiten_type,uint16_t init) 
{
  uint8_t buf[2];

  buf[0] = (whiten_type & 0x1) << 4;
  buf[0] |= (init >> 8) & 0xff;
  buf[1] = init & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_FSK_WHITE_PARAMS,buf,2);
}

//Configure the CRC params for FSK packets
void LR2021_SetFskCrcParams(uint8_t lr,uint32_t polynom,uint32_t init)
{
  uint8_t buf[8];

  buf[0] = (polynom >> 24) & 0xff;
  buf[1] = (polynom >> 16) & 0xff;
  buf[2] = (polynom >> 8) & 0xff;
  buf[3] = polynom & 0xff;
  buf[4] = (init >> 24) & 0xff;
  buf[5] = (init >> 16) & 0xff;
  buf[6] = (init >> 8) & 0xff;
  buf[7] = init & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_FSK_CRC_PARAMS,buf,8);
}

//Configure the syncword for FSK packets
void LR2021_SetFskSync(uint8_t lr,uint64_t syncword,uint8_t bit_order,uint8_t nb_bits) 
{
  uint8_t buf[9];

  cmd[0] = (syncword >> 56) & 0xff;
  cmd[1] = (syncword >> 48) & 0xff;
  cmd[2] = (syncword >> 40) & 0xff;
  cmd[3] = (syncword >> 32) & 0xff;
  cmd[4] = (syncword >> 24) & 0xff;
  cmd[5] = (syncword >> 16) & 0xff;
  cmd[6] = (syncword >> 8) & 0xff;
  cmd[8] = syncword & 0xff;
  cmd[8] = (bit_order & 0x1) << 7;
  cmd[8] |= nb_bits & 0x7f;
  LR2021_writeCmd(lr,LR2021_SET_FSK_SYNC,buf,9);
}

//Configure the addresses for filtering for FSK packets
void LR2021_SetFskAddr(uint8_t lr,uint8_t addr_node,uint8_t addr_bcast) 
{
  uint8_t buf[2];

  buf[0] = addr_node;
  buf[1] = addr_bcast;
  LR2021_writeCmd(lr,LR2021_SET_FSK_ADDR,buf,2);
}

//Get FSK RX stats
void LR2021_GetFskRxStats(uint8_t lr,FskRxStats_t *stats)  
{
  uint8_t buf[14];
  
  LR2021_readCmd(lr,LR2021_GET_FSK_RX_STATS,NULL,0,buf,14);
  stats->pktcount = ((uint16_t)buf[0] << 8) | buf[1];
  stats->crcerrors = ((uint16_t)buf[2] << 8) | buf[3];
  stats->lenerrors = ((uint16_t)buf[4] << 8) | buf[5];
  stats->detections = ((uint16_t)buf[6] << 8) | buf[7];
  stats->syncok = ((uint16_t)buf[8] << 8) | buf[9];
  stats->syncfail = ((uint16_t)buf[10] << 8) | buf[11];
  stats->rtctimeouts = ((uint16_t)buf[12] << 8) | buf[13];
}

//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone irq), but rssi_sync is already updated on SyncWordValid irq
void LR2021_GetFskPktStatus(uint8_t lr,FskPktStatus_t *status)  
{
  uint8_t buf[6];
  
  LR2021_readCmd(lr,LR2021_GET_FSK_PKT_STATUS,NULL,0,buf,14);
  status->length = ((uint16_t)buf[0] << 8) | buf[1];
  status->rssi_avg = (((int16_t)buf[4] >> 2) & 0x01) | ((int16_t)buf[2] << 1);
  status->rssi_sync = ((int16_t)buf[4] & 0x01) | ((int16_t)buf[3] << 1);
  if((buf[4] >> 5) & 0x01) status->bcast_match = true;
  else status->bcast_match = false;
  if((buf[4] >> 4) & 0x01) status->node_match = true;
  else status->node_match = false;
  status->lqi = buf[5];
}

