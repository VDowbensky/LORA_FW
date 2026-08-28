// Lrfhss commands API
#include "LR2021_lrfhss.h"

//Sets the LR-FHSS syncword. Reset value is { 0x2C, 0x0F, 0x79, 0x95 }
void LR2021_SetLrFhssSync(uint8_t lr,uint32_t syncword)
{
  uint8_t buf[4];

  buf[0] |= (syncword >> 24) & 0xff;
  buf[1] |= (syncword >> 16) & 0xff;
  buf[2] |= (syncword >> 8) & 0xff;
  buf[3] |= syncword & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_LR_FHSS_SYNC,buf,4);
}

// Commands with variable length parameters (not implemented):
// - LrFhssBuildFrame
