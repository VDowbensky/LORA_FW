#include "gpio.h"

void mygpio_init(void)
{	
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOA, true);
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOB, true);
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOC, true);
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOD, true);
	
	//Keys
	gpio_init(K_UP_PORT, K_UP_PIN, GPIO_MODE_INPUT_PULL_UP);
	gpio_init(K_DOWN_PORT, K_DOWN_PIN, GPIO_MODE_INPUT_PULL_UP);
	gpio_init(K_OK_PORT, K_OK_PIN, GPIO_MODE_INPUT_PULL_UP);
	gpio_init(BUZZER_PORT, BUZZER_PIN, GPIO_MODE_OUTPUT_PP_LOW);
}
