#ifndef _FLASH_H_
#define _FLASH_H_

#include "bsp.h"

#define CONFIG_ADDR			0x0803f000
#define CONFIG_SIZE			0x1000

#define MAGIC_NUMBER		0x57575757

void readconfig(void);
void writeconfig(void);


#endif
