#include "sx128x_interface.h"

uint8_t SX128X_spi_transfer(uint8_t sx,uint8_t b);

void SX128X_reset(uint8_t sx)
{
  switch(sx)
  {
    case 0:
    delay_ms(20); 
    gpio_init(SX1280_RESET_PORT, SX1280_RESET_PIN, GPIO_MODE_OUTPUT_PP_LOW);
    delay_ms(50);   
    gpio_init(SX1280_RESET_PORT, SX1280_RESET_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
    delay_ms(20);
    break;
    
    default:
    break;
  } 
}

void SX128X_select(uint8_t sx)
{
  switch(sx)
  {
    case 0:
    gpio_write(SX1280_NSS_PORT, SX1280_NSS_PIN,GPIO_LEVEL_LOW);
    break;
    
    default:
    break;
  }
}

void SX128X_deselect(uint8_t sx)
{
  switch(sx)
  {
    case 0:
    gpio_write(SX1280_NSS_PORT, SX1280_NSS_PIN,GPIO_LEVEL_HIGH);
    break;
    
    default:
    break;
  }
}

void SX128X_spi_init(uint8_t sx)
{
//  switch(sx)
//  {
//    case 0:
//    //NSS
//    gpio_init(SX1280_NSS_PORT, SX1280_NSS_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
//    //SPI pins
//    gpio_set_iomux(SX1280_SCK_PORT, SX1280_SCK_PIN, 4);
//    gpio_set_iomux(SX1280_MOSI_PORT, SX1280_MOSI_PIN, 4);
//    gpio_set_iomux(SX1280_MISO_PORT, SX1280_MISO_PIN, 4);
//    //RESET
//    gpio_init(SX1280_RESET_PORT, SX1280_RESET_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
//    //TCXO
//    gpio_init(SX1280_TCXO_PORT, SX1280_TCXO_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
//    gpio_config_drive_capability(SX1280_TCXO_PORT, SX1280_TCXO_PIN, GPIO_DRIVE_CAPABILITY_8MA);
//    //BUSY
//    gpio_init(SX1280_BUSY_PORT, SX1280_BUSY_PIN, GPIO_MODE_INPUT_PULL_DOWN);
//    //DIO1
//    gpio_init(SX1280_INT_PORT, SX1280_INT_PIN, GPIO_MODE_INPUT_PULL_DOWN);
//    //DIO2
//    //gpio_init(SX128X_DIO2_PORT, SX128X_DIO2_PIN, GPIO_MODE_INPUT_PULL_DOWN);
//    //DIO3
//    //gpio_init(SX128X_DIO3_PORT, SX128X_DIO3_PIN, GPIO_MODE_INPUT_PULL_DOWN);
//    //configure DIO1 interrupt
//    gpio_config_interrupt(SX1280_INT_PORT, SX1280_INT_PIN, GPIO_INTR_RISING_EDGE);
//    NVIC_ClearPendingIRQ(GPIO_IRQn);
//    NVIC_EnableIRQ(GPIO_IRQn);
//  
//    //init SPI module
//    ssp_init_t init_struct;
//    rcc_enable_peripheral_clk(RCC_PERIPHERAL_SSP1, true);
//    //ssp_init_struct(&init_struct);
//    init_struct.ssp_sclk      = 4000000; // 4M
//    init_struct.ssp_role      = SSP_ROLE_MASTER;
//    init_struct.ssp_format    = SSP_FRAME_FORMAT_SPI;
//    init_struct.ssp_data_size = SSP_DATA_SIZE_8BIT;
//    init_struct.ssp_clk_pol   = SPI_CLK_POLARITY_HIGH;
//    init_struct.ssp_clk_phase = SPI_CLK_PHASE_2EDGE;
//    init_struct.ssp_dma_tx_en = DISABLE;
//    init_struct.ssp_dma_rx_en = DISABLE;
//    ssp_init(SSP1, &init_struct);
//    ssp_cmd(SSP1, ENABLE);
//    break;
//    
//    default:
//    break;
//	}
}

uint8_t SX128X_spi_transfer(uint8_t sx,uint8_t b)
{
  uint8_t retval = 0;
  
  switch(sx)
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

bool SX128X_checkBusy(uint8_t sx)
{
  uint32_t attempts = SX128X_BUSY_TIMEOUT;
  switch(sx)
  {
    case 0:
    while(attempts--)
    {
      if(gpio_read(SX1280_BUSY_PORT, SX1280_BUSY_PIN) == GPIO_LEVEL_LOW) return true;
    }
    return false;
    
    default:
    return false;
  }
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




