#ifndef _PAN_INTERFACE_H_
#define _PAN_INTERFACE_H_

#include "bsp.h"

#define PAN_COUNT       2 //set actual value here

uint8_t __ctz(uint8_t Value);
uint8_t PAN_ReadReg(uint8_t pan,uint8_t Addr);
PAN_Err_t PAN_WriteReg(uint8_t pan, uint8_t Addr, uint8_t Value);
void PAN_ReadRegs(uint8_t pan, uint8_t Addr, uint8_t *Buffer, uint8_t Size);
void PAN_WriteRegs(uint8_t pan,uint8_t Addr, uint8_t *Buffer, uint8_t Size);
void PAN_Reset(uint8_t pan);
void PAN_DelayUs(uint32_t us);
void PAN_DelayMs(uint32_t ms);

#endif
