#include "lr112x_interface.h"

#define LR112X_NOP    0

void LR112X_interface_init(void)
{
	LL_GPIO_InitTypeDef  gpio_init_structure;
	
  gpio_init_structure.Mode = LL_GPIO_MODE_OUTPUT;
	gpio_init_structure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	gpio_init_structure.Pin   = LR112X_RESET_PIN;
	LL_GPIO_Init(LR112X_RESET_PORT, &gpio_init_structure);
	LL_GPIO_SetOutputPin(LR112X_RESET_PORT,LR112X_RESET_PIN);
	
	gpio_init_structure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio_init_structure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	gpio_init_structure.Pin   = LR112X_NSS_PIN;
	LL_GPIO_Init(LR112X_NSS_PORT, &gpio_init_structure);
	LL_GPIO_SetOutputPin(LR112X_NSS_PORT,LR112X_NSS_PIN);

	gpio_init_structure.Pin   = LR112X_BUSY_PIN;
	gpio_init_structure.Mode  = LL_GPIO_MODE_INPUT;
	gpio_init_structure.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	gpio_init_structure.Pull  = LL_GPIO_PULL_DOWN;
	LL_GPIO_Init(LR112X_BUSY_PORT, &gpio_init_structure);
	
	gpio_init_structure.Pin   = LR112X_INT_PIN;
	LL_GPIO_Init(LR112X_INT_PORT, &gpio_init_structure);
	
	//config EXTI

  //NVIC_ClearPendingIRQ(GPIO_IRQn);
  //NVIC_EnableIRQ(GPIO_IRQn);
}

void LR112X_reset(uint8_t lr)
{
  switch(lr)
  {
    case 0:
		LL_GPIO_ResetOutputPin(LR112X_RESET_PORT, LR112X_RESET_PIN);
    delay_ms(10);
    LL_GPIO_SetOutputPin(LR112X_RESET_PORT, LR112X_RESET_PIN);
    delay_ms(10);
    break;
    
    default:
    break;
  }
}

void LR112X_Wakeup(uint8_t lr)
{
  switch(lr)
  {
    case 0:
		LL_GPIO_ResetOutputPin(LR112X_NSS_PORT, LR112X_NSS_PIN);
    delay_ms(1);
		LL_GPIO_SetOutputPin(LR112X_NSS_PORT, LR112X_NSS_PIN);
    // Wait for chip to be ready.
    LR112X_checkBusy(lr);
    break;
    
    default:
    break;
  }
}

bool LR112X_checkBusy(uint8_t lr)
{ 
  uint32_t attempts = LR112X_BUSY_TIMEOUT;
  
  switch(lr)
  {
    case 0:
    while(attempts--)
    {
			if(LL_GPIO_IsInputPinSet(LR112X_BUSY_PORT, LR112X_BUSY_PIN) == 0) return true;
    }
    return false;
    
    default:
    return false;
  }
}

void LR112X_select(uint8_t lr)
{
  switch(lr)
  {
    case 0:
		LL_GPIO_ResetOutputPin(LR112X_NSS_PORT, LR112X_NSS_PIN);
    break;
    
    default:
    break;
  }
}

void LR112X_deselect(uint8_t lr)
{
  switch(lr)
  {
    case 0:
    LL_GPIO_SetOutputPin(LR112X_NSS_PORT, LR112X_NSS_PIN);
    break;
    
    default:
    break;
  }
}

void LR112X_writeCmd(uint8_t lr,uint16_t cmd, uint8_t *buffer, uint16_t size)
{
  uint8_t i;
  
  LR112X_checkBusy(lr);
  LR112X_select(lr);
  LR112X_spi_transfer(lr,cmd >> 8);
  LR112X_spi_transfer(lr,cmd & 0xff); //big endian
  for(i = 0;i < size;i++) LR112X_spi_transfer(lr,buffer[i]);
  LR112X_deselect(lr);
  if(cmd != LR112X_SET_SLEEP) LR112X_checkBusy(lr);
}

void LR112X_readCmd(uint8_t lr,uint16_t cmd, uint8_t *args, uint8_t argslen, uint8_t *buffer, uint16_t size)
{
  uint8_t i;
  
  LR112X_checkBusy(lr);
  LR112X_select(lr);
  LR112X_spi_transfer(lr,cmd >> 8);
  LR112X_spi_transfer(lr,cmd & 0xff); //big endian
  for(i = 0; i < argslen; i++) LR112X_spi_transfer(lr,args[i]);
  LR112X_deselect(lr);
  LR112X_checkBusy(lr);
  LR112X_select(lr);
  for(i = 0;i < size;i++) buffer[i] = LR112X_spi_transfer(lr,LR112X_NOP);
  LR112X_deselect(lr);
  LR112X_checkBusy(lr);
}

void LR112X_WriteRegMem32(uint8_t lr,uint32_t addr, uint8_t *buffer, uint16_t size)
{
  uint8_t buf[4];
  uint16_t i;
  
  buf[0] = (addr >> 24) & 0xff;
  buf[1] = (addr >> 16) & 0xff;
  buf[2] = (addr >> 8) & 0xff;
  buf[3] = addr & 0xff;
    LR112X_checkBusy(lr);
    LR112X_select(lr);
  LR112X_spi_transfer(lr,LR112X_WRITE_REG_MEM32 >> 8);
  LR112X_spi_transfer(lr,LR112X_WRITE_REG_MEM32 & 0xff);
  for(i = 0; i < 4; i++) LR112X_spi_transfer(lr,buf[i]);
  for(i = 0; i < size*4; i++) LR112X_spi_transfer(lr,buffer[i]);
  LR112X_deselect(lr);
  LR112X_checkBusy(lr);
}

void LR112X_ReadRegMem32(uint8_t lr,uint32_t addr, uint8_t *buffer, uint16_t size)
{
  uint16_t i;
  uint8_t buf[5];
  
  buf[0] = (addr >> 24) & 0xff;
  buf[1] = (addr >> 16) & 0xff;
  buf[2] = (addr >> 8) & 0xff;
  buf[3] = addr & 0xff;
  buf[4] = size;
  LR112X_checkBusy(lr);
  LR112X_select(lr);
  LR112X_spi_transfer(lr,LR112X_READ_REG_MEM32 >> 8);
  LR112X_spi_transfer(lr,LR112X_READ_REG_MEM32 & 0xff);
  for(i = 0; i < 5; i++) LR112X_spi_transfer(lr,buf[i]);
	LR112X_deselect(lr);
  LR112X_checkBusy(lr);
	LR112X_select(lr);
  LR112X_spi_transfer(lr,LR112X_NOP); //first byte (Stat1) discarded
  for(i = 0; i < size*4; i++) buffer[i] = LR112X_spi_transfer(lr,LR112X_NOP);
  LR112X_deselect(lr);
  LR112X_checkBusy(lr);
}

void LR112X_WriteReg32(uint8_t lr,uint32_t addr, uint32_t regdata)
{
  uint8_t buf[8];
  uint16_t i;
  
  buf[0] = (addr >> 24) & 0xff;
  buf[1] = (addr >> 16) & 0xff;
  buf[2] = (addr >> 8) & 0xff;
  buf[3] = addr & 0xff;
	buf[4] = (regdata >> 24) & 0xff;
	buf[5] = (regdata >> 16) & 0xff;
	buf[6] = (regdata >> 8) & 0xff;
	buf[7] = regdata & 0xff;
	LR112X_writeCmd(lr,LR112X_WRITE_REG_MEM32,buf,8);
  //LR112X_checkBusy(lr);
  //LR112X_select(lr);
  //LR112X_spi_transfer(lr,LR112X_WRITE_REG_MEM32 >> 8);
  //LR112X_spi_transfer(lr,LR112X_WRITE_REG_MEM32 & 0xff);
  //for(i = 0; i < 8; i++) LR112X_spi_transfer(lr,buf[i]);
  //LR112X_deselect(lr);
  //LR112X_checkBusy(lr);
}

uint32_t LR112X_ReadReg32(uint8_t lr,uint32_t addr)
{
  uint16_t i;
  uint8_t buf[5];
  
  buf[0] = (addr >> 24) & 0xff;
  buf[1] = (addr >> 16) & 0xff;
  buf[2] = (addr >> 8) & 0xff;
  buf[3] = addr & 0xff;
  buf[4] = 1;
  LR112X_checkBusy(lr);
  LR112X_select(lr);
  LR112X_spi_transfer(lr,LR112X_READ_REG_MEM32 >> 8);
  LR112X_spi_transfer(lr,LR112X_READ_REG_MEM32 & 0xff);
  for(i = 0; i < 5; i++) LR112X_spi_transfer(lr,buf[i]);
	LR112X_deselect(lr);
  LR112X_checkBusy(lr);
	LR112X_select(lr);
  LR112X_spi_transfer(lr,LR112X_NOP); //first byte (Stat1) discarded
  for(i = 0; i < 4; i++) buf[i] = LR112X_spi_transfer(lr,LR112X_NOP);
  LR112X_deselect(lr);
  LR112X_checkBusy(lr);
	return ((uint32_t)buf[0] << 24) + ((uint32_t)buf[1] << 16) + 	((uint32_t)buf[2] << 16) + buf[3];
}

void LR112X_writeRegMemMask32(uint8_t lr,uint32_t addr, uint32_t mask, uint32_t regdata)
{
  uint8_t buf[12];
  
  buf[0] = (addr >> 24) & 0xff;
  buf[1] = (addr >> 16) & 0xff;
  buf[2] = (addr >> 8) & 0xff;
  buf[3] = addr & 0xff;
  buf[4] = (mask >> 24) & 0xff;
  buf[5] = (mask >> 16) & 0xff;
  buf[6] = (mask >> 8) & 0xff;
  buf[7] = mask & 0xff;
  buf[8] = (regdata >> 24) & 0xff;
  buf[9] = (regdata >> 16) & 0xff;
  buf[10] = (regdata >> 8) & 0xff;
  buf[11] = regdata & 0xff;
  LR112X_writeCmd(lr,LR112X_WRITE_REG_MEM_MASK32,buf,12);
}

void LR112X_writeBuffer8(uint8_t lr,uint8_t *data, uint8_t len)
{
  uint8_t i;
  
  LR112X_checkBusy(lr);
  LR112X_select(lr);
  LR112X_spi_transfer(lr,LR112X_WRITE_BUFFER8 >> 8);
  LR112X_spi_transfer(lr,LR112X_WRITE_BUFFER8 & 0xff);
  for(i = 0; i < len;i++) LR112X_spi_transfer(lr,data[i]);
  LR112X_deselect(lr);
  LR112X_checkBusy(lr);
}

void LR112X_readBuffer8(uint8_t lr,uint8_t offset, uint8_t *data, uint8_t len)
{
  uint8_t i;
  
  LR112X_checkBusy(lr);
  LR112X_select(lr);
  LR112X_spi_transfer(lr,LR112X_READ_BUFFER8 >> 8);
  LR112X_spi_transfer(lr,LR112X_READ_BUFFER8 & 0xff);
  LR112X_spi_transfer(lr,offset);
  LR112X_spi_transfer(lr,len);
  LR112X_checkBusy(lr);
  LR112X_spi_transfer(lr,LR112X_NOP); //first byte (Stat1) discarded
  for(i = 0;i < len; i++) data[i] = LR112X_spi_transfer(lr,LR112X_NOP);
  LR112X_deselect(lr);
  LR112X_checkBusy(lr);
}

uint8_t LR112X_spi_transfer(uint8_t lr,uint8_t b)
{
  switch(lr)
  {
    case 0:
		return auxspi_transfer(b);
		
    default:
    return 0;
  }
}
