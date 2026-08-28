#include "flash.h"
#include "rfconfig.h"

bool flash_unlock(void);
bool flash_lock(void);
void flash_erase_page(uint32_t addr);
void flash_program_bytes(uint32_t addr,uint8_t *src,uint32_t len);
static void flash_wait_ready(void);
static void flash_clear_flags(void);

uint8_t flashbuffer[CONFIG_SIZE];

void readconfig(void)
{
	memcpy((void *)&globalrfconfig,(uint8_t*)(CONFIG_ADDR),sizeof(globalrfconfig));
}

void writeconfig(void)
{
	memcpy((void *)(&flashbuffer[0]),(uint8_t*)&globalrfconfig,sizeof(globalrfconfig));
	flash_unlock();
	flash_erase_page(CONFIG_ADDR);
	flash_program_bytes(CONFIG_ADDR, flashbuffer, CONFIG_SIZE);
	flash_lock();
}


bool flash_unlock(void)
{
	if(FLASH->CR & FLASH_CR_LOCK)
  {
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
    if (FLASH->CR & FLASH_CR_LOCK) return false;
  }
  return true;
}

bool flash_lock(void)
{
  /* Set the LOCK Bit to lock the FLASH Registers access */
  /* @Note  The lock and unlock procedure is done only using CR registers even from CPU2 */
  FLASH->CR |= FLASH_CR_LOCK;
  /* verify Flash is locked */
  if ((FLASH->CR & FLASH_CR_LOCK) == 0U) return false;
  return true;
}

void flash_erase_page(uint32_t addr)
{
	uint32_t pnb = (addr & 0x07ffffff) >> 11;
	
	flash_unlock();
	flash_wait_ready();
	flash_clear_flags();
	FLASH->CR = FLASH_CR_PER;      // Page erase
	//FLASH->CR |= addr;             // Set page address
	FLASH->CR |= pnb << FLASH_CR_PNB_Pos; // Set page address
	FLASH->CR |= FLASH_CR_STRT;    // Start erase
	flash_wait_ready();
	FLASH->CR &= ~FLASH_CR_PER;    // Disable erase mode
	flash_lock();
}

void flash_program_bytes(uint32_t addr,uint8_t *src,uint32_t len)
{
	flash_unlock();
	flash_wait_ready();
	flash_clear_flags();
	while (len > 0)
	{
		uint64_t dword = 0;
		/* Assemble 64-bit block */
		for (int i = 0; i < 8; i++)
		{
			uint8_t b = (len > 0) ? *src++ : 0xFF;  // pad with 0xFF
			dword |= ((uint64_t)b << (8 * i));
			if (len > 0) len--;
		}
		/* Enable programming */
		FLASH->CR |= FLASH_CR_PG;
		/* Write double-word */
		*(volatile uint32_t*)(addr)       = (uint32_t)(dword & 0xFFFFFFFF);
		*(volatile uint32_t*)(addr + 4)   = (uint32_t)(dword >> 32);
		flash_wait_ready();
		FLASH->CR &= ~FLASH_CR_PG;
		addr += 8;
	}
	flash_lock();
}

static void flash_wait_ready(void)
{
	while (FLASH->SR & FLASH_SR_BSY);
}

static void flash_clear_flags(void)
{
	FLASH->SR |= FLASH_SR_EOP;         // writing 1 clears flag
	FLASH->ECCR |= (1U << 24);         // clear ECC errors
}

