#include "lr112x_interface.h"

#define LR112X_NOP    0

void LR112X_reset(uint8_t lr)
{
  switch(lr)
  {
    case 0:
    gpio_write(LR112X_RESET_PORT, LR112X_RESET_PIN,GPIO_LEVEL_LOW);
    delay_ms(2);
    gpio_write(LR112X_RESET_PORT, LR112X_RESET_PIN,GPIO_LEVEL_HIGH);
    //delay_ms(10);
    break;
    
    default:
    break;
  }
}

void LR112X_Wakeup(uint8_t lr)
{
	LR112X_select(lr);
  delay_ms(1);
  // Wait for chip to be ready.
	LR112X_deselect(lr);
  LR112X_checkBusy(lr);

}

bool LR112X_checkBusy(uint8_t lr)
{ 
  uint32_t attempts = LR112X_BUSY_TIMEOUT;
  
  switch(lr)
  {
    case 0:
    while(attempts--)
    {
      if(gpio_read(LR112X_BUSY_PORT, LR112X_BUSY_PIN) == GPIO_LEVEL_LOW) return true;
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
    gpio_write(LR112X_NSS_PORT, LR112X_NSS_PIN,GPIO_LEVEL_LOW);
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
    gpio_write(LR112X_NSS_PORT, LR112X_NSS_PIN,GPIO_LEVEL_HIGH);
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


void LR112X_WriteRegMem32(uint8_t lr,uint32_t addr, uint8_t *buffer, uint16_t size) //not works correctly, hardfault fires
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


//lr11xx_status_t lr11xx_regmem_write_regmem32( const void* context, const uint32_t address, const uint32_t* buffer,
//                                              const uint8_t length )
//{
//    uint8_t cbuffer[LR11XX_REGMEM_WRITE_REGMEM32_CMD_LENGTH];
//    uint8_t cdata[LR11XX_REGMEM_BUFFER_SIZE_MAX];

//    if( length > LR11XX_REGMEM_MAX_WRITE_READ_WORDS )
//    {
//        return LR11XX_STATUS_ERROR;
//    }

//    lr11xx_regmem_fill_cbuffer_cdata_opcode_address_data( cbuffer, cdata, LR11XX_REGMEM_WRITE_REGMEM32_OC, address,
//                                                          buffer, length );

//    return ( lr11xx_status_t ) lr11xx_hal_write( context, cbuffer, LR11XX_REGMEM_WRITE_REGMEM32_CMD_LENGTH, cdata,
//                                                 length * sizeof( uint32_t ) );
//}


void LR112X_ReadRegMem32(uint8_t lr,uint32_t addr, uint8_t *buffer, uint16_t size) //not works correctly, hardfault fires
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

//lr11xx_status_t lr11xx_regmem_read_regmem32( const void* context, const uint32_t address, uint32_t* buffer,
//                                             const uint8_t length )
//{
//    uint8_t cbuffer[LR11XX_REGMEM_READ_REGMEM32_CMD_LENGTH] = { 0 };

//    if( length > LR11XX_REGMEM_MAX_WRITE_READ_WORDS )
//    {
//        return LR11XX_STATUS_ERROR;
//    }

//    lr11xx_regmem_fill_cbuffer_opcode_address_length( cbuffer, LR11XX_REGMEM_READ_REGMEM32_OC, address, length );

//    const lr11xx_status_t status = ( lr11xx_status_t ) lr11xx_hal_read(
//        context, cbuffer, LR11XX_REGMEM_READ_REGMEM32_CMD_LENGTH, ( uint8_t* ) buffer, length * sizeof( uint32_t ) );

//    if( status == LR11XX_STATUS_OK )
//    {
//        lr11xx_regmem_fill_out_buffer_from_raw_buffer( buffer, ( const uint8_t* ) buffer, length );
//    }

//    return status;
//}


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
  uint8_t retval = 0;
  
  switch(lr)
  {
    case 0:
    AUX_SSP->DR = b;
    while(1) 
    {
      uint32_t status = AUX_SSP->SR;
      if(((status & 0x01) == 0x01) && ((status & 0x10)==0)) break;
    }
    retval = AUX_SSP->DR & 0xFF;
    return retval;
  
    default:
    return 0;
  }
}

void LR112X_rfsw_tx(uint8_t lr)
{
  switch(lr)
  {
    case 0:

    break;
    
    default:
    break;
  }
}

void LR112X_rfsw_rx(uint8_t lr)
{
  switch(lr)
  {
    case 0:

    break;
    
    default:
    break;
  }
}
