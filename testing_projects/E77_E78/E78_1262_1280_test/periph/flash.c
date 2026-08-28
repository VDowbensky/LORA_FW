#include "flash.h"
#include "rfconfig.h"

uint8_t flashbuffer[CONFIG_SIZE];

void readconfig(void)
{
	memcpy((void *)&globalrfconfig,(uint8_t*)(CONFIG_ADDR),sizeof(globalrfconfig));
}

void writeconfig(void)
{
	memcpy((void *)(&flashbuffer[0]),(uint8_t*)&globalrfconfig,sizeof(globalrfconfig));
	flash_erase_page(CONFIG_ADDR);
	flash_program_bytes(CONFIG_ADDR, flashbuffer, CONFIG_SIZE);
}