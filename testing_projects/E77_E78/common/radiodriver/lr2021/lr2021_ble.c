// Ble commands API
#include "lr2021_ble.h"

//Configure the modulation parameters for BLE packets
void LR2021_SetBleModParams(uint8_t lr,uint8_t ble_mode)
{
  ble_mode &= 0x03;
  LR2021_writeCmd(lr,LR2021_SET_BLE_MOD_PARAMS,&ble_mode,1);
}

//Configure the modulation parameters for BLE packets
void LR2021_SetBleModParamsAdv(uint8_t lr,uint8_t ble_mode,uint8_t rx_bw) 
{
  uint8_t buf[2];
  
  buf[0] = ble_mode & 0x3;
  buf[1] = rx_bw;
  LR2021_writeCmd(lr,LR2021_SET_BLE_MOD_PARAMS,buf,2);
}

//Sets the BLE channel/packet dependent parameters
void LR2021_SetBleChannelParams(uint8_t lr,bool crc_in_fifo,uint8_t channel_type,uint8_t whit_init,uint32_t crc_init,uint32_t syncword) 
{
  uint8_t buf[9];
  
  buf[0] = channel_type & 0x0f;
  if(crc_in_fifo) buf[0] |= 16;
  buf[1] = whit_init;
  buf[2] = (crc_init >> 16) & 0xff;
  buf[3] = (crc_init >> 8) & 0xff;
  buf[4] = crc_init & 0xff;
  buf[5] = (syncword >> 24) & 0xff;
  buf[6] = (syncword >> 16) & 0xff;
  buf[7] = (syncword >> 8) & 0xff;
  buf[8] = syncword & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_BLE_CH_PARAMS,buf,9);
}

//Configure PDU length to transmit and send a BLE packet. This command is a concatenation of SetBlePduLen(pld_len) and SetTx(0)
void LR2021_SetBleTx(uint8_t lr,uint8_t pld_len)
{
  LR2021_writeCmd(lr,LR2021_SET_BLE_TX,&pld_len,1);
}

/// Gets the status of the last received packet. Status is updated at the end of a reception (RxDone irq), but rssi_sync is already updated on SyncWordValid irq
void LR2021_GetBlePacketStatus(uint8_t lr,BlePacketStatus_t *status) //to check!!!
{
  uint8_t buf[6];
  
  LR2021_readCmd(lr,LR2021_GET_BLE_PKT_STATUS,NULL,0,buf,6);
  status->length = ((uint16_t)buf[0] << 8) | buf[1];
  status->rssi_avg = ((int16_t)(buf[4] >> 2) & 0x01) | ((int16_t)buf[2] << 1);
  status->rssi_sync = ((int16_t)buf[4] & 0x01) | ((int16_t)buf[3] << 1);
  status->lqi = buf[5];
}

//Gets the internal statistics of the received packets. Statistics are reset on a POR, sleep without memory retention and the command ResetRxStats
void LR2021_GetBleRxStats(uint8_t lr,BleRxStats_t *stats)
{
  uint8_t buf[6];
  
  LR2021_readCmd(lr,LR2021_GET_BLE_RX_STATS,NULL,0,buf,6);
  stats->packetcount = ((uint16_t)buf[0] << 8) | buf[1];
  stats->crcerrors = ((uint16_t)buf[2] << 8) | buf[3];
  stats->lenerrors = ((uint16_t)buf[4] << 8) | buf[5];
}

void LR2021_GetBleRxStatsAdv(uint8_t lr,BleRxStats_t *stats)
{
  uint8_t buf[16];
  
  LR2021_readCmd(lr,LR2021_GET_BLE_RX_STATS,NULL,0,buf,16);
  stats->packetcount = ((uint16_t)buf[0] << 8) | buf[1];
  stats->crcerrors = ((uint16_t)buf[2] << 8) | buf[3];
  stats->lenerrors = ((uint16_t)buf[4] << 8) | buf[5];
  stats->numdetections = ((uint16_t)buf[6] << 8) | buf[7];
  stats->syncok = ((uint16_t)buf[8] << 8) | buf[9];
  stats->syncfail = ((uint16_t)buf[10] << 8) | buf[11];
  stats->rtctimeouts = ((uint16_t)buf[12] << 8) | buf[13];
  stats->crcok = ((uint16_t)buf[14] << 8) | buf[15];
}

//Sets PDU length for TX
void LR2021_SetBlePduLen(uint8_t lr,uint8_t pdu_len)
{
  LR2021_writeCmd(lr,LR2021_SET_BLE_TX_PDU_LEN,&pdu_len,1);
}
