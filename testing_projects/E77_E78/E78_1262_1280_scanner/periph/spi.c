#include "spi.h"

uint8_t spi_transfer(ssp_typedef_t* SSP, uint8_t b)
{
	uint8_t data = 0;

  SSP->DR = b;
	while(1) 
	{
		uint32_t status = SSP->SR;
		if(((status & 0x01) == 0x01) && ((status & 0x10)==0)) break;
	}
	data = SSP->DR & 0xFF;
  return data;
}
