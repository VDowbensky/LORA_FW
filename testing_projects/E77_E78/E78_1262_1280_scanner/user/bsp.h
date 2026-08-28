#ifndef _BSP_H_
#define _BSP_H_

#include <stdio.h>
#include <string.h>
#include "tremo_system.h"
#include "tremo_rcc.h"
#include "tremo_flash.h"
#include "tremo_gpio.h"
#include "tremo_uart.h"
#include "tremo_adc.h"
#include "tremo_delay.h"
#include "retargetserial.h"
//#include "aes_proc.h"

#define HW_VERSION    1
#define FW_VERSION		0
#define FW_REVISION		1

#define TXD0_PORT						GPIOB
#define TXD0_PIN						GPIO_PIN_1
#define RXD0_PORT						GPIOB
#define RXD0_PIN						GPIO_PIN_0

#define SCL0_PORT						GPIOA
#define SCL0_PIN						GPIO_PIN_14
#define SDA0_PORT						GPIOA
#define SDA0_PIN						GPIO_PIN_15


//Keys
#define K_UP_PORT						GPIOA
#define K_UP_PIN						GPIO_PIN_9
#define K_DOWN_PORT					GPIOA
#define K_DOWN_PIN					GPIO_PIN_4
#define K_ENTER_PORT				GPIOA
#define K_ENTER_PIN					GPIO_PIN_5

#define K_NONE							255
#define K_UP								0
#define K_DOWN							1
#define K_ENTER							2
#define KEY_COUNT	          3

//BATT measuring switch
#define VBATT_MEAS_PORT			GPIOA
#define VBATT_MEAS_PIN			GPIO_PIN_8
#define VBATT_PORT					GPIOA
#define VBATT_PIN						GPIO_PIN_11
#define VBATT_ADC_CHANNEL		ADC_SAMPLE_CHAN_1//PA11

//Buzzer
#define BUZZER_PORT					GPIOB
#define BUZZER_PIN					GPIO_PIN_15


//AUX RF power
#define EXTRF_POWER_PORT		GPIOB
#define EXTRF_POWER_PIN			GPIO_PIN_13
//Main RF switch
#define RFSW_CTRL_PORT			GPIOA
#define RFSW_CTRL_PIN				GPIO_PIN_10 //GPIO10
#define RFSW_TXRX_PORT			GPIOD
#define RFSW_TXRX_PIN				GPIO_PIN_11 //GPIO59

//SX1262(AUX) SSP
#define AUX_SSP							SSP1
#define AUX_RCC_PERIPHERAL	RCC_PERIPHERAL_SSP1
#define AUX_SCK_PORT				GPIOB
#define AUX_SCK_PIN					GPIO_PIN_8
#define AUX_MOSI_PORT				GPIOB
#define AUX_MOSI_PIN				GPIO_PIN_10
#define AUX_MISO_PORT				GPIOB
#define AUX_MISO_PIN				GPIO_PIN_11
#define SX1262_SSP					AUX_SSP
#define SX1262_NSS_PORT			GPIOC
#define SX1262_NSS_PIN			GPIO_PIN_3
#define SX1262_RESET_PORT		GPIOC
#define SX1262_RESET_PIN		GPIO_PIN_2
#define SX1262_BUSY_PORT		GPIOC
#define SX1262_BUSY_PIN			GPIO_PIN_5
#define SX1262_INT_PORT			GPIOC
#define SX1262_INT_PIN			GPIO_PIN_4

//#define SX1262_SCK_PORT 		AUX_SCK_PORT 
//#define SX1262_SCK_PIN			AUX_SCK_PIN
//#define SX1262_MOSI_PORT		AUX_MOSI_PORT	
//#define SX1262_MOSI_PIN			AUX_MOSI_PIN
//#define SX1262_MISO_PORT		AUX_MISO_PORT
//#define SX1262_MISO_PIN			AUX_MISO_PIN

//SX1280 SSP
#define SX1280_SSP					AUX_SSP
#define SX1280_NSS_PORT			GPIOC
#define SX1280_NSS_PIN			GPIO_PIN_0
#define SX1280_RESET_PORT		GPIOC
#define SX1280_RESET_PIN		GPIO_PIN_1
#define SX1280_BUSY_PORT		GPIOC
#define SX1280_BUSY_PIN			GPIO_PIN_9
#define SX1280_INT_PORT			GPIOC
#define SX1280_INT_PIN			GPIO_PIN_10
#define SX1280_TCXO_PORT		GPIOC
#define SX1280_TCXO_PIN			GPIO_PIN_8

//#define SX1280_SCK_PORT 		GPIOB 
//#define SX1280_SCK_PIN			GPIO_PIN_8
//#define SX1280_MOSI_PORT		GPIOB	
//#define SX1280_MOSI_PIN			GPIO_PIN_10
//#define SX1280_MISO_PORT		GPIOB
//#define SX1280_MISO_PIN			GPIO_PIN_11

//SSD1306
#define SSD1306_INTERFACE_SOFT_SPI	0
#define SSD1306_INTERFACE_HARD_SPI	1
#define SSD1306_INTERFACE_I2C				0

#if SSD1306_INTERFACE_SOFT_SPI
#define SSD1306_SCK_PORT    GPIOA
#define SSD1306_SCK_PIN			GPIO_PIN_5
#define SSD1306_MOSI_PORT		GPIOA
#define SSD1306_MOSI_PIN		GPIO_PIN_4
#define SSD1306_RST_PORT		GPIOA
#define SSD1306_RST_PIN			GPIO_PIN_14
#define SSD1306_DC_PORT			GPIOA
#define SSD1306_DC_PIN			GPIO_PIN_15
#define SSD1306_CS_PORT			GPIOD
#define SSD1306_CS_PIN			GPIO_PIN_12

#else //hard SPI
#define SSD1306_SSP					SSP2	
#define SSD1306_RCC_PERIPHERAL RCC_PERIPHERAL_SSP2
#define SSD1306_SCK_PORT    GPIOB
#define SSD1306_SCK_PIN			GPIO_PIN_12
#define SSD1306_MOSI_PORT		GPIOB
#define SSD1306_MOSI_PIN		GPIO_PIN_14
#define SSD1306_RST_PORT		GPIOB
#define SSD1306_RST_PIN			GPIO_PIN_9
#define SSD1306_DC_PORT			GPIOB
#define SSD1306_DC_PIN			GPIO_PIN_7
#define SSD1306_CS_PORT			GPIOA
#define SSD1306_CS_PIN			GPIO_PIN_15

#endif

#define UART0_BR						460800
#define UART1_BR						115200

void init_power_clk(void);
void init_peripherals(void);
void init_radio_specific(void);
void oled_test(void);
#endif
