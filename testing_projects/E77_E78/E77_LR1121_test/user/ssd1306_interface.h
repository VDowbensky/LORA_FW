#ifndef _SSD1306_INTERFACE_H_
#define _SSD1306_INTERFACE_H_
#include "bsp.h"
#include "aux_spi.h"

void SSD1306_interface_init(void);
void SSD1306_Select(void);
void SSD1306_Deselect(void);
uint8_t SSD1306_spi_transfer(uint8_t b);
void SSD1306_WriteByte(uint8_t b);
void SSD1306_Command(bool cmd);
void SSD1306_Reset(void);
void SSD1306_WR_Byte(uint8_t dat,uint8_t cmd); 

extern char strbuffer[];

#endif
