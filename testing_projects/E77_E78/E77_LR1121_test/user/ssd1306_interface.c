#include "ssd1306_interface.h"

char strbuffer[64];

void SSD1306_interface_init(void)
{
	LL_GPIO_InitTypeDef  gpio_init_structure;

  LL_GPIO_SetOutputPin(SSD1306_CS_PORT,SSD1306_CS_PIN);
	LL_GPIO_SetOutputPin(SSD1306_RST_PORT,SSD1306_RST_PIN);
	LL_GPIO_ResetOutputPin(SSD1306_DC_PORT,SSD1306_DC_PIN);
	
	gpio_init_structure.Pin   = SSD1306_CS_PIN;
  gpio_init_structure.Mode  = LL_GPIO_MODE_OUTPUT;
	gpio_init_structure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  gpio_init_structure.Pull  = LL_GPIO_PULL_NO;
  gpio_init_structure.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  LL_GPIO_Init(SSD1306_CS_PORT, &gpio_init_structure);
	
	gpio_init_structure.Pin   = SSD1306_RST_PIN;
	LL_GPIO_Init(SSD1306_RST_PORT, &gpio_init_structure);
	
	gpio_init_structure.Pin   = SSD1306_DC_PIN;
	LL_GPIO_Init(SSD1306_DC_PORT, &gpio_init_structure);
	
	
	//auxspi_init();
}

void SSD1306_Select(void)
{
	LL_GPIO_ResetOutputPin(SSD1306_CS_PORT, SSD1306_CS_PIN);
}

void SSD1306_Deselect(void)
{
	LL_GPIO_SetOutputPin(SSD1306_CS_PORT, SSD1306_CS_PIN);
}

void SSD1306_WriteByte(uint8_t b)
{
	SSD1306_spi_transfer(b);
}

void SSD1306_Command(bool cmd)
{
	if(cmd) LL_GPIO_SetOutputPin(SSD1306_DC_PORT, SSD1306_DC_PIN); 
	else LL_GPIO_ResetOutputPin(SSD1306_DC_PORT, SSD1306_DC_PIN); 
}

void SSD1306_Reset(void)
{
	LL_GPIO_SetOutputPin(SSD1306_RST_PORT, SSD1306_RST_PIN);
	delay_ms(100);
	LL_GPIO_ResetOutputPin(SSD1306_RST_PORT, SSD1306_RST_PIN);
	delay_ms(100);
	LL_GPIO_SetOutputPin(SSD1306_RST_PORT, SSD1306_RST_PIN);
	delay_ms(200);
}

uint8_t SSD1306_spi_transfer(uint8_t b)
{
	return auxspi_transfer(b);
}

void SSD1306_WR_Byte(uint8_t dat,uint8_t cmd)
{
	if(cmd) SSD1306_Command(true);
	else SSD1306_Command(false);
	SSD1306_Select();
	SSD1306_WriteByte(dat);
	SSD1306_Deselect();
}


