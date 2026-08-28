// Ook commands API
#include "lr2021_ook.h"

//Sets the OOK modulation parameters
void LR2021_SetOokModParams(uint8_t lr,uint32_t bitrate,uint8_t pulse_shape,uint8_t rx_bw)
{
  uint8_t buf[6];

  buf[0] = (bitrate >> 24) & 0xff
  buf[1] = (bitrate >> 16) & 0xff;
  buf[2] = (bitrate >> 8) & 0xff;
  buf[3] = bitrate & 0xff;
  buf[4] = pulse_shape & 0x0f;
  buf[5] = rx_bw;
  LR2021_writeCmd(lr,LR2021_SET_OOK_MOD_PARAMS,buf,6);
}

//Sets the OOK modulation parameters
void LR2021_SetOokModParamsAdv(uint8_t lr,uint32_t bitrate,uint8_t pulse_shape,uint8_t rx_bw,uint8_t depth)
{
  uint8_t buf[7];

  buf[0] = (bitrate >> 24) & 0xff
  buf[1] = (bitrate >> 16) & 0xff;
  buf[2] = (bitrate >> 8) & 0xff;
  buf[3] = bitrate & 0xff;
  buf[4] = pulse_shape & 0x0f;
  buf[5] = rx_bw;
  buf[6] = depth & 0x01;
  LR2021_writeCmd(lr,LR2021_SET_OOK_MOD_PARAMS,buf,7);
}

//Sets the OOK packet parameters. It is recommended to have either whitening or manchester encoding enabled for OOK
void LR2021_SetOokPktParams(uint8_t lr,uint16_t pre_len_tx,uint8_t addr_comp,uint8_t pkt_format,uint16_t pld_len,uint8_t crc,uint8_t encoding)
{
  uint8_t buf[6];

  buf[0] = (pre_len_tx >> 8) & 0xff;
  buf[1] = pre_len_tx & 0xff;
  buf[2] = (addr_comp & 0x03) << 2;
  buf[3] |= pkt_format & 0x03;
  buf[3] = (pld_len >> 8) & 0xff;
  buf[4] = pld_len & 0xff;
  buf[5] = (crc & 0x0f) << 4;
  buf[5] |= encoding & 0x0f;
  LR2021_writeCmd(lr,LR2021_SET_OOK_PKT_PARAMS,buf,6);
}

//Sets the OOK CRC parameters (same as for FSK)
void LR2021_SetOokCrcParams(uint8_t lr,uint32_t polynom,uint32_t init)
{
  uint8_t buf[8];

  buf[2] = (polynom >> 24) & 0xff;
  buf[3] = (polynom >> 16) & 0xff;
  buf[4] = (polynom >> 8) & 0xff;
  buf[5] = polynom & 0xff;
  buf[6] = (init >> 24) & 0xff;
  buf[7] = (init >> 16) & 0xff;
  buf[8] = (init >> 8) & 0xff;
  buf[9] = init & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_OOK_CRC_PARAMS,buf,8);
}

//Sets the OOK syncword. Limited to 32bits max
void LR2021_SetOokSync(uint8_t lr,uint32_t syncword,uint8_t bit_order,uint8_t nb_bits)
{
  uint8_t buf[5];

  buf[0] = (syncword >> 24) & 0xff;
  buf[1] = (syncword >> 16) & 0xff;
  buf[2] = (syncword >> 8) & 0xff;
  buf[3] = syncword & 0xff;
  buf[4] = (bit_order & 0x01) << 7;
  buf[4] |= nb_bits & 0x7f;
  LR2021_writeCmd(lr,LR2021_SET_OOK_SYNC,buf,5);
}

//Sets the OOK address (same as for FSK)
void LR112X_SetOokAddr(uint8_t lr,uint8_t addr_node,uint8_t addr_bcast)  
{
  uint8_t buf[2];

  buf[0] = addr_node;
  buf[1] = addr_bcast;
  LR2021_writeCmd(lr,LR2021_SET_OOK_ADDR,buf,2);
}

//Gets the internal statistics of the received packets. Statistics are reset on a POR, sleep without memory retention and the command ResetRxStats
void LR112X_GetOokRxStats(uint8_t lr,OokRxStats_t *stats)
{
  uint8_t buf[6];
  
  LR2021_readCmd(lr,LR2021_GET_OOK_RX_STATS,NULL,0,buf,6);
  stats->pktcount = ((uint16_t)buf[0] << 8) | buf[1];
  stats->crcerrors = ((uint16_t)buf[2] << 8) | buf[3];
  stats->lenerrors = ((uint16_t)buf[4] << 8) | buf[5];
}

//Get OOK RX stats
void LR112X_GetOokRxStatsAdv(uint8_t lr,OokRxStats_t *stats)  
{
  uint8_t buf[14];
  
  LR2021_readCmd(lr,LR2021_GET_OOK_RX_STATS,NULL,0,buf,14);
  stats->pktcount = ((uint16_t)buf[0] << 8) | buf[1];
  stats->crcerrors = ((uint16_t)buf[2] << 8) | buf[3];
  stats->lenerrors = ((uint16_t)buf[4] << 8) | buf[5];
  stats->detections = ((uint16_t)buf[6] << 8) | buf[7];
  stats->syncok = ((uint16_t)buf[8] << 8) | buf[9];
  stats->syncfail = ((uint16_t)buf[10] << 8) | buf[11];
  stats->rtctimeouts = ((uint16_t)buf[12] << 8) | buf[13];
}

//Gets the status of the last received OOK packet. Status is updated at the end of a reception (RxDone irq), but rssi_sync is already updated on SyncWordValid irq
void LR112X_GetOokPktStatus(uint8_t lr,OokPktStatus_t *status)
{
    [0x02, 0x87]
  uint8_t buf[6];
  
  LR2021_readCmd(lr,LR2021_GET_OOK_PKT_STATUS,NULL,0,buf,14);
  status->length = ((uint16_t)buf[0] << 8) | buf[1];
  status->rssi_avg = (((int16_t)buf[4] >> 2) & 0x01) | ((int16_t)buf[2] << 1);
  status->rssi_high = ((int16_t)buf[4] & 0x01) | ((int16_t)buf[3] << 1);
  if((buf[4] >> 5) & 0x01) status->bcast_match = true;
  else status->bcast_match = false;
  if((buf[4] >> 4) & 0x01) status->node_match = true;
  else status->node_match = false;
  status->lqi = buf[5];
}

//Configures the OOK detection. This API is for RX only. Detection pattern for TX has directly to be put into the TX FIFO
void LR112X_SetOokDet(uint8_t lr,uint16_t preamble_pattern,uint8_t  pattern_length,uint8_t pattern_num_repeats,bool sw_is_raw,uint8_t sfd_kind,uint8_t sfd_length) 
{
  uint8_t buf[5];
  
  buf[0] = (preamble_pattern >> 8) & 0xff;
  buf[1] = preamble_pattern & 0xff;
  buf[2] = pattern_length & 0x0f;
  buf[3] = pattern_num_repeats & 0x1f;
  buf[4] = ((sfd_kind as u8) & 0x1) << 4;
  if (sw_is_raw) buf[4] |= 32;
  buf[4] |= sfd_length & 0x0f;
  LR2021_writeCmd(lr,LR2021_SET_OOK_DET,buf,5);
}

//Configure the whitening for OOK packets. Setting a polynomial of 0 will disable the whitening. It is recommended to have either whitening or manchester encoding enabled for OOK
void LR112X_SetOokWhiteParams(uint8_t lr,uint8_t bit_idx,uint16_t polynom,uint16_t init)
{
  uint8_t buf[4];

    buf[0] = (polynom >> 8) & 0xff;
    buf[0] |= (bit_idx & 0x0f) << 4;
    buf[1] = polynom & 0xff;
    buf[2] = (init >> 8) & 0xff;
    buf[3] = init & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_OOK_WHITE_PARAMS,buf,4);
}
