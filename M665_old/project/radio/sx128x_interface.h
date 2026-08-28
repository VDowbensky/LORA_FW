#ifndef _SX128X_INTERFACE_H_
#define _SX128X_INTERFACE_H_

#include "bsp.h"
#include "sx128x_defs.h"
//SPI interface
#include "wk_spi.h"

#define SX128X_BUSY_TIMEOUT   1000000UL;

void SX128X_reset(uint8_t sx);
void SX128X_select(uint8_t sx);
void SX128X_deselect(uint8_t sx);
bool SX128X_checkBusy(uint8_t sx);
void radiospi_init(uint8_t sx);
void SX128X_writeCmd(uint8_t sx,uint8_t cmd, uint8_t *buffer, uint16_t size);
void SX128X_readCmd(uint8_t sx,uint8_t cmd, uint8_t *buffer, uint16_t size);
void SX128X_writeRegs(uint8_t sx,uint16_t reg, uint8_t *buffer, uint16_t size);
void SX128X_readRegs(uint8_t sx,uint16_t reg, uint8_t *buffer, uint16_t size);
void SX128X_writeReg(uint8_t sx,uint16_t reg, uint8_t value);
uint8_t SX128X_readReg(uint8_t sx,uint16_t reg);
void SX128X_writeBuffer(uint8_t sx,uint8_t offset, uint8_t *data, uint8_t length);
void SX128X_readBuffer(uint8_t sx,uint8_t offset, uint8_t *data, uint8_t length);

#endif
