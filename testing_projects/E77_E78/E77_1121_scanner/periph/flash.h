#ifndef _FLASH_H_
#define _FLASH_H_

#include "bsp.h"

#define CONFIG_ADDR			0x0801f000
#define CONFIG_SIZE			0x1000

#define MAGIC_NUMBER		0x57575757

#define FLASH_KEY1			0x45670123
#define FLASH_KEY2			0xcdef89ab

void readconfig(void);
void writeconfig(void);


#endif
