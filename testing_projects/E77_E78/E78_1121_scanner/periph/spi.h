#ifndef _SPI_H_
#define _SPI_H_

#include "bsp.h"
#include "tremo_spi.h"

uint8_t spi_transfer(ssp_typedef_t* SSP, uint8_t b);

#endif
