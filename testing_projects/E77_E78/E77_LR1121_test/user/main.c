#include "bsp.h"
#include "radio.h"
#include "app_cli.h"
#include "gui.h"
#include "test.h"

int main(void)
{
	init_power_clk();
	init_peripherals();
	delay_ms(100);
	printf("\r\nDouble radio test\r\n");
	SSD1306_Init();
	SSD1306_Clear(0);
	init_radio_specific();
	delay_ms(100);
	cli_init();
	//oled_test();
	SSD1306_Clear(0);
	GUI_ShowString(0,0,"IDLE MODE  ",16,1);
	updatescreen();
	//GUI_DrawLine(64, 64, 64, 16,1);
	
	while(1)
	{
		radio_proc();
		cli_proc();
	}
}
