#include "lr2021_lora.h"

//Sets the LoRa modulation parameters. FW configures respective modem registers. Will return CMD_FAIL in the status of the next command, if the packet type is not LoRa
void LR2021_SetLoraModParams(uint8_t lr,uint8_t sf,uint8_t lora_bw,uint8_t lora_cr,uint8_t ldro) 
{
  uint8_t buf[2];

  buf[0] = (sf & 0x0f) << 4;
  buf[0] |= lora_bw & 0x0f;
  buf[1] = (lora_cr & 0x0f) << 4;
  buf[1] |= ldro & 0x03;
  LR2021_writeCmd(lr,LR2021_SET_LORA_MOD_PARAMS,buf,2);
}

//Sets the packet parameters for the LoRa packets. FW configures according modem registers
void LR2021_SetLoraPktParams(uint8_t lr,uint8_t payload_len,uint8_t header_type,bool crc_en,bool invert_iq)
{
  uint8_t buf[4];

  buf[0] = ((pbl_len >> 8) & 0xFF) as u8;
  buf[1] = (pbl_len & 0xFF) as u8;
  buf[2] = payload_len;
  buf[3] = ((header_type as u8) & 0x1) << 2;
  if(crc_en) buf[3] |= 2;
  if(invert_iq) buf[3] |= 1;
  LR2021_writeCmd(lr,LR2021_SET_LORA_PKT_PPARAMS,buf,4);
}

//Configure LoRa modem to search for a detect for N symbols. N can be given as number, or as mantissa/exponent. SymbolNum 0x00 means no timeout
void LR2021_SetLoRaSynchTimeout(uint8_t lr,uint8_t symbols,uint8_t timeout_format) 
{
  uint8_t buf[2];

  buf[0] = symbols;
  buf[1] = timeout_format & 0x1;
  LR2021_writeCmd(lr,LR2021_SET_LORA_SYNCH_TIMEOUT,buf,2);
}

//Sets the LoRa syncword. Default value is 0x12. Examples: Public Network: 0x34, Private Network: 0x12
void LR2021_SetLoraSync(uint8_t lr,uint8_t syncword)
{
  LR2021_writeCmd(lr,LR2021_SET_LORA_SYNC,&syncword,1);
}

//Configure the LoRa RX multi SF (side detectors) config. The multi-sf can listen to multiple SF in parallel and automatically switch to RX mode to demodulate the right SF. Calling SetLoraModulationParams will disable all side detectors
void LR2021_SetLoraSideDetConfig(uint8_t lr)
{
  LR2021_writeCmd(lr,LR2021_SET_LORA_SIDE_DET_CONFIG,NULL,0);
}

//Configure the LoRa RX multi SF (side detectors) config. The multi-sf can listen to multiple SF in parallel and automatically switch to RX mode to demodulate the right SF. Calling SetLoraModulationParams will disable all side detectors
void LR2021_SetLoraSideDetConfigAdv(uint8_t lr,uint8_t sd1_sf,uint8_t sd1_ldro,bool sd1_inv,uint8_t sd2_sf,uint8_t sd2_ldro,bool sd2_inv,uint8_t sd3_sf,uint8_t sd3_ldro, bool sd3_inv)
{
  uint8_t buf[3];

  buf[0] = (sd1_sf & 0x0f) << 4;
  buf[0] |= (sd1_ldro & 0x03) << 2;
  if (sd1_inv) buf[0] |= 1;
  buf[1] = (sd2_sf & 0x0f) << 4;
  buf[1] |= (sd2_ldro) & 0x03) << 2;
  if (sd2_inv) buf[1] |= 1;
  buf[2] = (sd3_sf & 0x0f) << 4;
  buf[2] |= (sd3_ldro & 0x03) << 2;
  if (sd3_inv) buf[2] |= 1;
  LR2021_writeCmd(lr,LR2021_SET_LORA_SIDE_DET_CONFIG,buf,3);
}

//Configure the LoRa RX multi SF (side detectors) syncwords
void LR2021_SetLoraSideDetSync(uint8_t lr)
{
  LR2021_writeCmd(lr,LR2021_SET_LORA_SIDE_DET_SYNC,NULL,0);
}

//Configure the LoRa RX multi SF (side detectors) syncwords
void LR2021_SetLoraSideDetSyncAdv(uint8_t lr,uint8_t sd1_sw,uint8_t sd2_sw,uint8_t sd3_sw)
{
  uint8_t buf[3];

  buf[0] = sd1_sw;
  buf[1] = sd2_sw;
  buf[2] = sd3_sw;
  LR2021_writeCmd(lr,LR2021_SET_LORA_SIDE_DET_SYNC,buf,3);
}

//Configures the LoRa phase modulation demodulation during preamble feature. If enabled, for TX the preamble will have a phase modulation overlayed to the LoRa modulation
void LR2021_ConfigLoraPreMod(uint8_t lr,bool pmod_en,uint8_t dram_ret,uint16_t wakeup_time,uint32_t min_sleep_time) 
{
  uint8_t buf[6];

  buf[0] = dram_ret & 0x07;
  if (pmod_en) buf[0] |= 128;
  buf[1] = (wakeup_time >> 8) & 0xff;
  buf[2] = wakeup_time & 0xff;
  buf[3] = (min_sleep_time >> 16) & 0xff;
  buf[4] = (min_sleep_time >> 8) & 0xff;
  buf[5] = min_sleep_time & 0xff;
  LR2021_writeCmd(lr,LR2021_CONFIG_LORA_PRE_MOD,buf,6);
}

//Configures the LoRa phase modulation demodulation during preamble feature. If enabled, for TX the preamble will have a phase modulation overlayed to the LoRa modulation
void LR2021_ConfigLoRaPreModAdv(uint8_t lr,bool pmod_en,uint8_t dram_ret,uint16_t wakeup_time,uint32_t min_sleep_time,uint8_t err_thr,
                                uint8_t min_sym,uint8_t detect_time_sym,uint8_t start_offset,uint8_t end_offset)
{
  uint8_t buf[10];

  buf[0] = dram_ret & 0x7;
  if (pmod_en) buf[0] |= 128;
  buf[1] = (wakeup_time >> 8) & 0xff;
  buf[2] = wakeup_time & 0xff;
  buf[3] = (min_sleep_time >> 16) & 0xff;
  buf[4] = (min_sleep_time >> 8) & 0xff;
  buf[5] = min_sleep_time & 0xff;
  buf[6] = err_thr & 0x7f;
  buf[7] = (min_sym & 0x0f) << 4;
  buf[7] |= detect_time_sym & 0x0f;
  buf[8] = start_offset;
  buf[9] = end_offset;
  LR2021_writeCmd(lr,LR2021_CONFIG_LORA_PRE_MOD,buf,10);
}

//Configure LoRa CAD mode parameters. Sets up Channel Activity Detection which searches for presence of LoRa preamble symbols
void LR2021_SetLoraCadParams(uint8_t lr,uint8_t nb_symbols,bool pbl_any,uint8_t pnr_delta,uint8_t exit_mode,uint32_t timeout,uint8_t det_peak)
{
  uint8_t buf[7];

  buf[0] = nb_symbols;
  buf[1] = pnr_delta & 0x0f;
  if (pbl_any) buf[1] |= 16;
  buf[2] = exit_mode;
  buf[3] = ((timeout >> 16) & 0xff;
  buf[4] = ((timeout >> 8) & 0xff;
  buf[5] = (timeout & 0xff;
  buf[6] = det_peak;
  LR2021_writeCmd(lr,LR2021_SET_LORA_CAD_PARAMS,buf,7);
}

//Set device into RX CAD mode (LoRa). The Channel Activity Detection searches for the presence of LoRa preamble symbols. Parameters must be previously set using SetLoraCadParams
void LR2021_SetLoraCad(uint8_t lr)
{
  LR2021_writeCmd(lr,LR2021_SET_LORA_CAD,NULL,0);
}

//Gets the internal statistics of the received packets. Statistics are reset on a POR, sleep without memory retention and the command ResetRxStats
void LR2021_GetLoraRxStats(uint8_t lr, LoraRxStats_t *stats)
{
  uint8_t buf[10];
  
  LR2021_readCmd(lr,LR2021_GET_FSK_RX_STATS,NULL,0,buf,10);
  stats->pktcount = ((uint16_t)buf[0] << 8) | buf[1];
  stats->crcerrors = ((uint16_t)buf[2] << 8) | buf[3];
  stats->headererrors = ((uint16_t)buf[4] << 8) | buf[5];
  stats->predetections = ((uint16_t)buf[6] << 8) | buf[7];
  stats->falsesync = ((uint16_t)buf[8] << 8) | buf[9];
}

//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone or CadDone irqs)
void LR2021_GetLoraPktStatus(uint8_t lr,LoraPktStatus_t *status) 
{
  uint8_t buf[5];
  
  LR2021_readCmd(lr,LR2021_GET_LORA_PKT_STATUS,NULL,0,buf,5);
  if((buf[0] >> 4) & 0x01) status->crcstatus = true;
  else status->crcstatus = false;
  status->cr = buf[0] & 0x0f;
  status->length = buf[1];
  status->snrpkt = (int8_t)buf[2];
  status->rssi_pkt = (((int16_t)buf[5] >> 1) & 0x01) | ((int16_t)buf[3] << 1);
  status->rssi_signal = ((int16_t)buf[5] & 0x01) | ((int16_t)buf[4] << 1);
  status->detector = buf[4] & 0x0f;
}

//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone or CadDone irqs)
void LR2021_GetLoraPktStatusAdv(uint8_t lr,LoraPktStatus_t *status) 
{
  uint8_t buf[10];
  
  LR2021_readCmd(lr,LR2021_GET_LORA_PKT_STATUS,NULL,0,buf,10);
  if((buf[0] >> 4) & 0x01) status->crcstatus = true;
  else status->crcstatus = false;
  status->cr = buf[0] & 0x0f;
  status->length = buf[1];
  status->snrpkt = (int8_t)buf[2];
  status->rssi_pkt = (((int16_t)buf[5] >> 1) & 0x01) | ((int16_t)buf[3] << 1);
  status->rssi_signal = ((int16_t)buf[5] & 0x01) | ((int16_t)buf[4] << 1);
  status->detector = buf[4] & 0x0f;
  status->freq_offset = ((int32_t)buf[6] << 16) | ((int32_t)buf[7] << 8) | buf[8];
  if(buf[6] & 0x80) status->freq_offset -= (1<<24);
  status->agc_gain = buf[9];
}

//Sets the address for LoRa RX address filtering
void LR2021_SetLoraAddr(uint8_t lr,uint8_t addr_comp_len,uint8_t addr_comp_pos,uint64_t addr)
{
  uint8_t buf[9];

  buf[0] = (addr_comp_len & 0x0f) << 4;
  buf[0] |= addr_comp_pos & 0x0f;
  buf[1] = (addr >> 56) & 0xff;
  buf[2] = (addr >> 48) & 0xff;
  buf[3] = (addr >> 40) & 0xff;
  buf[4] = (addr >> 32) & 0xff;
  buf[5] = (addr >> 24) & 0xff;
  buf[6] = (addr >> 16) & 0xff;
  buf[7] = (addr >> 8) & 0xff;
  buf[8] = addr & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_LORA_ADDR,buf,9);
}

//Extended version of the SetLoraSyncword command to set all 10bits of the syncword
void LR2021_SetLoraSyncExt(uint8_t lr,uint8_t sync1,uint8_t sync2)
{
  uint8_t buf[2];

  buf[0] = sync1 & 0x1f;
  buf[1] = sync2 & 0x1f;
  LR2021_writeCmd(lr,LR2021_SET_LORA_SYNC_EXT,buf,2);
}

//Extended version of the SetLoraSideDetSyncword command to set all 10bits of the syncwords
void LR2021_SetLoraSideDetSyncExt(uint8_t lr,uint8_t  sd1_sw1,uint8_t sd1_sw2,uint8_t sd2_sw1,uint8_t sd2_sw2,uint8_t sd3_sw1,uint8_t sd3_sw2)
{
  uint8_t buf[6];

  buf[0] |= sd1_sw1 & 0x1f;
  buf[1] |= sd1_sw2 & 0x1f;
  buf[2] |= sd2_sw1 & 0x1f;
  buf[3] |= sd2_sw2 & 0x1f;
  buf[4] |= sd3_sw1 & 0x1f;
  buf[5] |= sd3_sw2 & 0x1f;
  LR2021_writeCmd(lr,LR2021_SET_LORA_SIDE_DET_SYNC_EXT,buf,6);
}

//Configures the LoRa Tx synchronization using dio
void LR2021_SetLoraTxSync(uint8_t lr,uint8_t function,uint8_t dio_num)
{
  uint8_t val;

  val = (function & 0x3) << 6;
  val |= dio_num & 0x0f;
  LR2021_writeCmd(lr,LR2021_SET_LORA_TX_SYNC,&val,1);
}
