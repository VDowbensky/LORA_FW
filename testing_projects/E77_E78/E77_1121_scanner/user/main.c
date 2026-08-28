#include "bsp.h"
#include "radio.h"
#include "app_cli.h"
#include "gui.h"
#include "test.h"
//#include "beeper.h"
//#include "rtc.h"
#include "menu.h"

int main(void)
{
	init_power_clk();
	init_peripherals();
	delay_ms(100);
	cli_init();
	SSD1306_Init();
	SSD1306_Clear(0);
	printf("\r\nMULTIBAND RADIO PLATFORM\r\n");
	printf("HW=%d,FW=%d,%d\r\n",HW_VERSION,FW_VERSION,FW_REVISION);
	init_radio_specific();
	delay_ms(100);
	//oled_test();
	GUI_ShowString(0,0,"DUAL BAND SCAN",16,1);
	delay_ms(1000);
	//updatescreen();
	//GUI_DrawLine(64, 64, 64, 16,1);
	//beep(2000,200);
	radio_startscan();
	while(1)
	{
		radio_proc();
		cli_proc();
		if (Key == K_ENTER)
		{
			//beep(3000,200);
			Key = K_NONE;
			radio_stopscan();
			menu_proc();
		}
		//if(rtc_flag)
		//{
		//	rtc_flag = false;
		//	updatescreen();
		//}
	}
}
