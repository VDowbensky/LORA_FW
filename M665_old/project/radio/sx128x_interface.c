#include "sx128x_interface.h"

uint8_t SX128X_spi_transfer(uint8_t sx,uint8_t b);

void SX128X_reset(uint8_t sx)
{
	delay_ms(20); 
	gpio_bits_reset(RF_RST_GPIO_PORT,RF_RST_PIN);
	delay_ms(50);   
	gpio_bits_set(RF_RST_GPIO_PORT,RF_RST_PIN);
	delay_ms(20);
}

void SX128X_select(uint8_t sx)
{
	gpio_bits_reset(RF_CS_GPIO_PORT,RF_CS_PIN);
}

void SX128X_deselect(uint8_t sx)
{
	gpio_bits_set(RF_CS_GPIO_PORT,RF_CS_PIN);
}

void SX128X_spi_init(uint8_t sx)
{

}

uint8_t SX128X_spi_transfer(uint8_t sx,uint8_t b)
{
	return spi1_transfer(b);
}

bool SX128X_checkBusy(uint8_t sx)
{
  uint32_t attempts = SX128X_BUSY_TIMEOUT;
	while(attempts--)
	{
		if(gpio_input_data_bit_read(RF_BUSY_GPIO_PORT,RF_BUSY_PIN) == RESET) return true;
	}
	return false;
}



void SX128X_writeCmd(uint8_t sx,uint8_t cmd, uint8_t *buffer, uint16_t size)
{
  uint8_t i;
  
  SX128X_checkBusy(sx);
  SX128X_select(sx);
  SX128X_spi_transfer(sx,cmd);
  for(i = 0;i < size;i++) SX128X_spi_transfer(sx,buffer[i]);
  SX128X_deselect(sx);
  if(cmd != SX128X_SET_SLEEP) SX128X_checkBusy(sx);
}

void SX128X_readCmd(uint8_t sx,uint8_t cmd, uint8_t *buffer, uint16_t size)
{
  uint8_t i;
  
  SX128X_checkBusy(sx);
  SX128X_select(sx);
  SX128X_spi_transfer(sx,cmd);
  for(i = 0;i < size;i++) buffer[i] = SX128X_spi_transfer(sx,0xff);
  SX128X_deselect(sx);
  SX128X_checkBusy(sx);
}

void SX128X_writeRegs(uint8_t sx,uint16_t reg, uint8_t *buffer, uint16_t size)
{
  uint8_t addr_l,addr_h;
  uint8_t i;
  
  addr_l = reg & 0xff;
  addr_h = reg >> 8;
  SX128X_checkBusy(sx);
  SX128X_select(sx);
  SX128X_spi_transfer(sx,SX128X_WRITE_REGISTER);
  SX128X_spi_transfer(sx,addr_h);//MSB
  SX128X_spi_transfer(sx,addr_l);//LSB
  for(i = 0;i < size; i++) SX128X_spi_transfer(sx,buffer[i]);
  SX128X_deselect(sx);
  SX128X_checkBusy(sx);
}

void SX128X_readRegs(uint8_t sx,uint16_t reg, uint8_t *buffer, uint16_t size)
{
  uint16_t i;
  uint8_t addr_l,addr_h;
  
  addr_h = reg >> 8;
  addr_l = reg & 0x00FF;
  
  SX128X_checkBusy(sx);
  SX128X_select(sx);
  SX128X_spi_transfer(sx,SX128X_READ_REGISTER);
  SX128X_spi_transfer(sx,addr_h);//MSB
  SX128X_spi_transfer(sx,addr_l);//LSB
  SX128X_spi_transfer(sx,0xff);
  for(i = 0; i < size; i++) buffer[i] = SX128X_spi_transfer(sx,0xff);
  SX128X_deselect(sx);
  SX128X_checkBusy(sx);
}

void SX128X_writeReg(uint8_t sx,uint16_t reg, uint8_t value)
{
  SX128X_writeRegs(sx,reg,&value,1);
}

uint8_t SX128X_readReg(uint8_t sx,uint16_t reg)
{
  uint8_t value;
  
  SX128X_readRegs(sx,reg,&value,1);
  return value;
}


void SX128X_writeBuffer(uint8_t sx,uint8_t offset, uint8_t *data, uint8_t length)
{
  uint16_t i;
  
  SX128X_checkBusy(sx);
  SX128X_select(sx);
  SX128X_spi_transfer(sx,SX128X_WRITE_BUFFER);
  SX128X_spi_transfer(sx,offset);
  for(i = 0; i < length;i++) SX128X_spi_transfer(sx,data[i]);
  SX128X_deselect(sx);
  SX128X_checkBusy(sx);
}

void SX128X_readBuffer(uint8_t sx,uint8_t offset, uint8_t *data, uint8_t length)
{
  uint16_t i;
  
  SX128X_checkBusy(sx);
  SX128X_select(sx);
  SX128X_spi_transfer(sx,SX128X_READ_BUFFER);
  SX128X_spi_transfer(sx,offset);
  SX128X_spi_transfer(sx,0xff);
  for(i = 0;i < length; i++) data[i] = SX128X_spi_transfer(sx,0xff);
  SX128X_deselect(sx);
  SX128X_checkBusy(sx);
}




