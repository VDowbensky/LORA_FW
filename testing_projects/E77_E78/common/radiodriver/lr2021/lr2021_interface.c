uint16_t LR2021_status[LR2021_COUNT];
uint32_t LR2021_irqstatus[LR2021_COUNT];
uint16_t LR2021_errors[LR2021_COUNT];

void LR2021_writeCmd(uint8_t lr,uint16_t cmd, uint8_t *buffer, uint16_t size)
{
  uint8_t i;
  uint8_t stat_buf[2];
  
  LR2021_checkBusy(lr);
  LR2021_select(lr);
  stat_buf[0] = LR2021_spi_transfer(lr,cmd >> 8);
  stat_buf[1] = LR2021_spi_transfer(lr,cmd & 0xff); //big endian
  for(i = 0;i < size;i++) LR2021_spi_transfer(lr,buffer[i]);
  LR2021_deselect(lr);
  if(cmd != LR2021_SET_SLEEP) LR2021_checkBusy(lr);
  LR2021_status[lr] = ((uint16_t)stat_buf[0] << 8) | stat_buf[1];
}

void LR2021_readCmd(uint8_t lr,uint16_t cmd, uint8_t *args, uint8_t argslen, uint8_t *buffer, uint16_t size)
{
  uint8_t i;
  uint8_t stat_buf[2];
  
  LR2021_checkBusy(lr);
  LR2021_select(lr);
  stat_buf[0] = LR2021_spi_transfer(lr,cmd >> 8);
  stat_buf[1] = LR2021_spi_transfer(lr,cmd & 0xff); //big endian
  for(i = 0; i < argslen; i++) LR2021_spi_transfer(lr,args[i]);
  LR2021_deselect(lr);
  LR2021_checkBusy(lr);
  LR2021_select(lr);
  for(i = 0;i < size;i++) buffer[i] = LR2021_spi_transfer(lr,LR2021_NOP);
  LR2021_deselect(lr);
  LR2021_checkBusy(lr);
  LR2021_status[lr] = ((uint16_t)stat_buf[0] << 8) | stat_buf[1];
}