#include "lr2021_zwave.h"

//Zwave commands API
//Sets the parameters for Z-Wave packets
void LR2021_SetZwaveParams(uint8_t lr,uint8_t zwave_mode,uint8_t rx_bw,uint8_t pld_len,uint16_t pbl_len_tx,uint8_t pbl_len_detect,uint8_t fcs_mode)
{
  uint8_t buf[8];

  buf[0] = zwave_mode & 0x03;
  buf[1] = rx_bw;
  buf[2] = zwave_addr_comp;;
  buf[3] = pld_len;
  buf[4] = (pbl_len_tx >> 8) & 0xff;
  buf[5] = pbl_len_tx & 0xff;
  buf[6] = pbl_len_detect;
  buf[7] = fcs_mode & 0x01;
  LR2021_writeCmd(lr,LR2021_SET_ZW_PARAMS,buf,8);
}

//Sets the HomeID address to use as a filter in Rx. The MSB is the first bit sent/received. Frames that don't match the home_id raise an address error IRQ
void LR2021_SetZwaveHomeId(uint8_t lr,uint32_t home_id)
{
  uint8_t buf[4];

  buf[0] = (home_id >> 24) & 0xff;
  buf[1] = (home_id >> 16) & 0xff;
  buf[2] = (home_id >> 8) & 0xff;
  buf[3] = home_id & 0xFff;
  LR2021_writeCmd(lr,LR2021_SET_ZW_HOME_ID,buf,4);
}

//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone irq), but rssi_sync is already updated on SyncWordValid irq
void LR2021_GetZwavePktStatus(uint8_t lr,ZwPacketStatus_t *status)
{
  uint8_t buf[7];
  
  LR2021_readCmd(lr,LR2021_GET_ZW_PKT_STATUS,NULL,0,buf,7);
  status->pkt_len = ((uint16_t)buf[0] << 8) | buf[1];
  status->rssi_avg = (((int16_t)buf[5] >> 2) & 0x01) | ((int16_t)buf[2] << 1);
  status->rssi_sync = ((int16_t)buf[5] & 0x01) | ((int16_t)buf[3] << 1);
  status->last_detect = buf[4] & 0x03;
  status->lqi = buf[6];
}

//Settings for filtering incoming beam frames in Rx
void LR2021_SetZwaveBeamFiltering(uint8_t lr,uint8_t addr_len,uint16_t node_id,uint8_t id_hash)
{
  uint8_t buf[4];

  buf[0] = beam_tag;
  buf[1] = (addr_len & 0x1) << 7;
  buf[1] |= (node_id >> 8) & 0xff;
  buf[2] = node_id & 0xff;
  buf[3] = id_hash;
  LR2021_writeCmd(lr,LR2021_SET_ZW_BEAM_FILTER,buf,4);
}

//Configure the Z-Wave scan mode. The user can configure up to 4 data rates and channels to be scanned. The rf_freq_ch and timeout parameters only need to be sent for num_ch channels
void LR2021_SetZwaveScanConfig(uint8_t lr,uint8_t num_ch,bool det4,bool det3,bool det2,bool det1,uint8_t bitrate_ch4,uint8_t bitrate_ch3,uint8_t bitrate_ch2,
                              uint8_t bitrate_ch1,uint8_t zwave_addr_comp,uint8_t fcs_mode,uint32_t rf_freq_ch1,uint8_t timeout_ch1,uint32_t rf_freq_ch2,uint8_t timeout_ch2) 
{
  uint8_t buf[14];

  buf[0] = (num_ch & 0x0f) << 4;
  if (det4) buf[0] |= 8;
  if (det3) buf[0] |= 4;
  if (det2) buf[0] |= 2;
  if (det1) buf[0] |= 1;
  buf[1] = (bitrate_ch4 & 0x03) << 6;
  buf[1] |= (bitrate_ch3 & 0x03) << 4;
  buf[1] |= (bitrate_ch2 & 0x03) << 2;
  buf[1] |= bitrate_ch1 & 0x03;
  buf[2] = zwave_addr_comp & 0x3;
  buf[3] = fcs_mode & 0x1;
  buf[4] = (rf_freq_ch1 >> 24) & 0xff;
  buf[5] = (rf_freq_ch1 >> 16) & 0xff;
  buf[6] = (rf_freq_ch1 >> 8) & 0xff;
  buf[7] = rf_freq_ch1 & 0xff;
  buf[8] = timeout_ch1;
  buf[9] = (rf_freq_ch2 >> 24) & 0xff;
  buf[10] = (rf_freq_ch2 >> 16) & 0xff;
  buf[11] = (rf_freq_ch2 >> 8) & 0xff;
  buf[12] = rf_freq_ch2 & 0xff;
  buf[13] = timeout_ch2;
  LR2021_writeCmd(lr,LR2021_SET_ZW_SCAN_CONFIG,buf,14);
}

//Configure the Z-Wave scan mode. The user can configure up to 4 data rates and channels to be scanned. The rf_freq_ch and timeout parameters only need to be sent for num_ch channels
void LR2021_SetZwaveScanConfigAdv(uint8_t lr,uint8_t num_ch,bool det4,bool det3,bool det2,bool det1,uint8_t bitrate_ch4,uint8_t bitrate_ch3,uint8_t bitrate_ch2,
                              uint8_t bitrate_ch1,uint8_t zwave_addr_comp,uint8_t fcs_mode,uint32_t rf_freq_ch1,uint8_t timeout_ch1,
                              uint32_t rf_freq_ch2,uint8_t timeout_ch2,uint32_t rf_freq_ch3,uint8_t timeout_ch3,uint32_t rf_freq_ch4,uint8_t timeout_ch4) 
{
  uint8_t buf[24];

  buf[0] = (num_ch & 0x0f) << 4;
  if (det4) buf[0] |= 8;
  if (det3) buf[0] |= 4;
  if (det2) buf[0] |= 2;
  if (det1) buf[0] |= 1;
  buf[1] = (bitrate_ch4 & 0x03) << 6;
  buf[1] |= (bitrate_ch3 & 0x03) << 4;
  buf[1] |= (bitrate_ch2 & 0x03) << 2;
  buf[1] |= bitrate_ch1 & 0x03;
  buf[2] = zwave_addr_comp & 0x3;
  buf[3] = fcs_mode & 0x1;
  buf[4] = (rf_freq_ch1 >> 24) & 0xff;
  buf[5] = (rf_freq_ch1 >> 16) & 0xff;
  buf[6] = (rf_freq_ch1 >> 8) & 0xff;
  buf[7] = rf_freq_ch1 & 0xff;
  buf[8] = timeout_ch1;
  buf[9] = (rf_freq_ch2 >> 24) & 0xff;
  buf[10] = (rf_freq_ch2 >> 16) & 0xff;
  buf[11] = (rf_freq_ch2 >> 8) & 0xff;
  buf[12] = rf_freq_ch2 & 0xff;
  buf[13] = timeout_ch2;
  buf[14] = (rf_freq_ch3 >> 24) & 0xff;
  buf[15] = (rf_freq_ch3 >> 16) & 0xff;
  buf[16] = (rf_freq_ch3 >> 8) & 0xff;
  buf[17] = rf_freq_ch3 & 0xff;
  buf[18] = timeout_ch3;
  buf[19] = (rf_freq_ch4 >> 24) & 0xff;
  buf[20] = (rf_freq_ch4 >> 16) & 0xff;
  buf[21] = (rf_freq_ch4 >> 8) & 0xff;
  buf[22] = rf_freq_ch4 & 0xf;
  buf[23] = timeout_ch4;
  LR2021_writeCmd(lr,LR2021_SET_ZW_SCAN_CONFIG,buf,24);
}

//This command is used to enter the Zwave RX scan mode. The scan mode will alternate between data rates and channels and attempt to detect an incoming packet
void LR2021_SetZwaveScan(uint8_t lr)
{
  LR2021_writeCmd(lr,LR2021_SET_ZW_SCAN,NULL,0);
}
