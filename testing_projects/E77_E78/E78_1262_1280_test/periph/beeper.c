#include "beeper.h"

uint16_t beep_duration;
bool beeper;

void beeper_init(void)
{
	bstimer_init_t bstimer_init_config;

	rcc_enable_peripheral_clk(RCC_PERIPHERAL_BSTIMER0,true);
	//bstimer_init_config.bstimer_mms        = BSTIMER_MMS_ENABLE;
  bstimer_init_config.period             = 499;  //initial (2 kHz) - 500 us
  bstimer_init_config.prescaler          = 31;  //sysclock defaults to 24M, is divided by (prescaler + 1) to 2.4k  
  bstimer_init_config.autoreload_preload = true;
  bstimer_init(BSTIMER0, &bstimer_init_config);
	
	bstimer_config_overflow_update(BSTIMER0,ENABLE);
	bstimer_config_interrupt(BSTIMER0,ENABLE);
	NVIC_ClearPendingIRQ(BSTIMER0_IRQn);
	NVIC_EnableIRQ(BSTIMER0_IRQn);
	//bstimer_cmd(BSTIMER0,ENABLE);
	bstimer_cmd(BSTIMER0,DISABLE);
}

void beep(uint16_t freq,uint16_t dur)
{
	beep_duration = dur;
	uint32_t period = (1000000 / (2 * freq));
	bstimer_cmd(BSTIMER0,DISABLE);
	BSTIMER0->ARR = period;
	//bstimer_cmd(BSTIMER0,ENABLE);
	beeper_on();
}

void beeper_on(void)
{
	bstimer_cmd(BSTIMER0,ENABLE);
	beeper = true;
}

void beeper_off(void)
{
	bstimer_cmd(BSTIMER0,DISABLE);
	gpio_write(BUZZER_PORT,BUZZER_PIN,GPIO_LEVEL_LOW);
	beeper = false;
}

void BSTIMER0_IRQHandler(void)
{
	if (bstimer_get_status(BSTIMER0, BSTIMER_SR_UIF)) 
	{
		gpio_toggle(BUZZER_PORT,BUZZER_PIN);
	}
}