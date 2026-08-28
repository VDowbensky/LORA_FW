#include "bsp.h"
#include "radio_proc.h"
#include "app_cli.h"
#include "uc1601_drv.h"
//#include "test.h"

int main(void)
{
	init_power_clk();
	init_peripherals();
	delay_ms(100);
	printf("\r\nRA08 test\r\n");
	led_on();
	UC1601Init();
	radio_init();
	delay_ms(100);
	led_off();
	cli_init();
	UC1601Clear();
	//UC1601CmdWrite(UC1601_SET_DC1_EN);
	UC1601CharDisplay(0,0,"Hello, Lamer!");
	UC1601CharDisplay(1,0,"TREMO TEST   ");
	UC1601CharDisplay(2,0,"Fuck China!");
	UC1601CharDisplay(3,0,"(c) GBL 2024");
	
	while(1)
	{
		radio_proc();
		cli_proc();
	}
}
