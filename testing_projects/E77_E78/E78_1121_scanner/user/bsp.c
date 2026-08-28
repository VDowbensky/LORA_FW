#include "bsp.h"

#include "gpio.h"
#include "uart.h"
#include "adc.h"
#include "i2c.h"
#include "ustimer.h"
#include "retargetserial.h"
#include "lr112x_interface.h"
#include "ssd1306.h"
#include "test.h"
#include "radio.h"
#include "flash.h"
#include "beeper.h"
#include "rtc.h"

void init_power_clk(void)
{
	rcc_enable_oscillator(RCC_OSC_XO32M, true);
	rcc_set_sys_clk_source(RCC_SYS_CLK_SOURCE_XO32M);
	rcc_set_hclk_div(RCC_HCLK_DIV_1);
	rcc_set_pclk_div(RCC_PCLK0_DIV_1,RCC_PCLK1_DIV_1);
	rcc_set_systick_source(RCC_SYSTICK_SOURCE_HCLK);
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_SYSCFG,true);
	//rcc_enable_peripheral_clk(RCC_PERIPHERAL_PWR, true);
	rtc_init();
	delay_init();
}

void init_peripherals(void)
{
	mygpio_init();
	myadc_init();
	RETARGET_SerialInit();
	ustimer_init();
	beeper_init();
}

void init_radio_specific(void)
{
	radio_interface_init();
	readconfig();
#if (_DEBUG_MODE_ == 0)
	if(globalrfconfig.magicnumber != MAGIC_NUMBER) 
	{
#endif
		radio_initconfigs();
		printf("INIT CONGIG: OK\r\n");
#if (_DEBUG_MODE_ == 0)
	}
#endif
	if(radio_init(RADIO_L) == true) printf("ASR6601: OK\r\n");
	else printf("ASR6601: ERROR\r\n");
	if(radio_init(RADIO_H) == true) printf("LR1121: OK\r\n");
	else printf("LR1121: ERROR\r\n");
}

void oled_test(void)
{
	delay_ms(1000);
	TEST_MainPage();         //÷˜ΩÁ√Êœ‘ æ≤‚ ‘
	SSD1306_Clear(0); 
	Test_Color();            //À¢∆¡≤‚ ‘
	SSD1306_Clear(0); 
	Test_Rectangular();      //æÿ–ŒªÊ÷∆≤‚ ‘
	SSD1306_Clear(0); 
	Test_Circle();           //‘≤–ŒªÊ÷∆≤‚ ‘
	SSD1306_Clear(0); 
	Test_Triangle();         //»˝Ω«–ŒªÊ÷∆≤‚ ‘
	SSD1306_Clear(0);  
	TEST_English();          //”¢Œƒœ‘ æ≤‚ ‘
	SSD1306_Clear(0); 
	TEST_Number_Character(); // ˝◊÷∫Õ∑˚∫≈œ‘ æ≤‚ ‘
	SSD1306_Clear(0); 
	TEST_BMP();              //BMPµ•…´Õº∆¨œ‘ æ≤‚ ‘
	SSD1306_Clear(0); 
	TEST_Menu1();            //≤Àµ•1œ‘ æ≤‚ ‘
	SSD1306_Clear(0); 
	TEST_Menu2();            //≤Àµ•2œ‘ æ≤‚ ‘
	SSD1306_Clear(0); 
	printf("LCD test done.\r\n");
}

