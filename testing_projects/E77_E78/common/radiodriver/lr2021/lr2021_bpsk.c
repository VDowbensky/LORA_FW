//Bpsk commands API
#include "lr2021_bpsk.h"

//Sets the modulation parameters for BPSK packets. FW configures respective modem registers
void LR2021_SetBpskModParamd(uint8_t lr,uint32_t bitrate,uint8_t pulse_shape,uint8_t diff_mode_en,bool diff_mode_init,bool diff_mode_parity) 
{
  uint8_t buf[5];

  buf[0] = (bitrate >> 24) & 0xff;
  buf[1] = (bitrate >> 16) & 0xff;
  buf[2] = (bitrate >> 8) & 0xff;
  buf[3] = bitrate & 0xff;
  buf[4] = ((pulse_shape & 0x0f) << 4;
  buf[4] |= ((diff_mode_en & 0x1) << 2;
  if(diff_mode_init) buf[4] |= 2;
  if(diff_mode_parity) buf[4] |= 1;
  LR2021_writeCmd(lr,LR2021_SET_BPSK_MOD_PARAMS,buf,5);
}

//Sets the packet parameters for BPSK packets. FW configures respective modem registers
void LR2021_SetBpskPacketParams(uint8_t lr,uint8_t pld_len,uint8_t bpsk_mode,uint8_t sigfox_msg,uint8_t sigfox_rank) 
{
  uint8_t buf[2];

  buf[0] = pld_len;
  buf[1] = (bpsk_mode & 0x03) << 4;
  buf[1] |= (sigfox_msg & 0x01) << 1;
  buf[1] |= (sigfox_rank & 0x03) << 6;
  LR2021_writeCmd(lr,LR2021_SET_BPSK_MOD_PARAMS,buf,2);
}
