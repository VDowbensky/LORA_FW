#ifndef _LR2021_BLE_H_
#define _LR2021_BLE_H_

//BLE
#define LR2021_SET_BLE_MOD_PARAMS     0x0260
#define LR2021_SET_BLE_CH_PARAMS      0x0261
#define LR2021_SET_BLE_TX             0x0262
#define LR2021_GET_BLE_RX_STATS       0x0264
#define LR2021_GET_BLE_PKT_STATUS     0x0265
#define LR2021_SET_BLE_TX_PDU_LEN     0x0266

/// BLE PHY mode selection
#define LR2021_BLE_MODE_LE1MB                         0
#define LR2021_BLE_MODE_LE2MB                         1
#define LR2021_BLE_MODE_LECODED500K                   2
#define LR2021_BLE_MODE_LECODED125K                   3
/// BLE channel type selection
#define LR2021_BLE_CHANNEL_TYPE_ADVERTIZER            0
#define LR2021_BLE_CHANNEL_TYPE_DATA16bitHEADER       1
#define LR2021_BLE_CHANNEL_TYPE_DATA24bitHEADER       2

typedef struct
{
  uint16_t length;
  int16_t rssi_avg;
  int16_t rssi_sync;
  uint8_t lqi;
}BlePacketStatus_t;

typedef struct
{
  uint16_t packetcount;// Total number of received packets
  uint16_t crcerrors;
  uint16_t lenerrors;
  uint16_t numdetections;
  uint16_t syncok;
  uint16_t syncfail;
  uint16_t rtctimeouts;
  uint16_t crcok;
}BleRxStats_t;

//Configure the modulation parameters for BLE packets
void LR2021_SetBleModParams(uint8_t lr,uint8_t ble_mode);
//Configure the modulation parameters for BLE packets
void LR2021_SetBleModParamsAdv(uint8_t lr,uint8_t ble_mode,uint8_t rx_bw);
//Sets the BLE channel/packet dependent parameters
void LR2021_SetBleChannelParams(uint8_t lr,bool crc_in_fifo,uint8_t channel_type,uint8_t whit_init,uint32_t crc_init,uint32_t syncword);
//Configure PDU length to transmit and send a BLE packet. This command is a concatenation of SetBlePduLen(pld_len) and SetTx(0)
void LR2021_SetBleTx(uint8_t lr,uint8_t pld_len);
/// Gets the status of the last received packet. Status is updated at the end of a reception (RxDone irq), but rssi_sync is already updated on SyncWordValid irq
void LR2021_GetBlePacketStatus(uint8_t lr,BlePacketStatus_t *status);
//Gets the internal statistics of the received packets. Statistics are reset on a POR, sleep without memory retention and the command ResetRxStats
void LR2021_GetBleRxStats(uint8_t lr,BleRxStats_t *stats);
void LR2021_GetBleRxStatsAdv(uint8_t lr,BleRxStats_t *stats);
//Sets PDU length for TX
void LR2021_SetBlePduLen(uint8_t lr,uint8_t pdu_len);

#endif

