#ifndef _LR2021_LRFHSS_H_
#define _LR2021_LRFHSS_H_

//LR-FHSS
#define LR2021_SET_LR_FHSS_SYNC       0x0257

//Sets the LR-FHSS syncword. Reset value is { 0x2C, 0x0F, 0x79, 0x95 }
void LR2021_SetLrFhssSync(uint8_t lr,uint32_t syncword);

// Commands with variable length parameters (not implemented):
// - LrFhssBuildFrame
#endif
