#include "bsp.h"

#include "gpio.h"
#include "uart.h"
#include "adc.h"
//#include "i2c.h"
#include "ustimer.h"
#include "retargetserial.h"
#include "lr112x_interface.h"
#include "ssd1306.h"
#include "test.h"
#include "radio.h"
#include "flash.h"
#include "stm32wlxx_it.h"


void init_power_clk(void)
{
	//HSE 32 MHz
	
	LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_HSEM);
  /* HSEM_IRQn interrupt configuration */
  NVIC_SetPriority(HSEM_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(HSEM_IRQn);
	
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSE_EnableTcxo();
  LL_RCC_HSE_Enable();
   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1);
  LL_RCC_LSI_Enable();
   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
  /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE);
	
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAHB3Prescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(32000000);
	delay_init();
	//LL_Init1msTick(32000000);
	SystemCoreClockUpdate();
	
}

void init_peripherals(void)
{
	mygpio_init();
	auxspi_init();
	//myadc_init();
	RETARGET_SerialInit();
	ustimer_init();
}

void init_radio_specific(void)
{
	radio_interface_init();
	readconfig();
	if(globalrfconfig.magicnumber != MAGIC_NUMBER) 
	{
		radio_initconfigs();
		printf("INIT CONFIG: OK\r\n");
	}
	radio_init(RADIO_L);
	radio_init(RADIO_H);
}

void oled_test(void)
{
	TEST_MainPage();         //÷˜ΩÁ√Êœ‘ æ≤‚ ‘
	delay_ms(1000);
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





