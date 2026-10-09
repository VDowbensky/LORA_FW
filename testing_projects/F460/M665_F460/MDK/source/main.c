/**
 *******************************************************************************
 * @file  main.c
 * @brief Main program.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2026-09-21       CDT             First version
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022-2025, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
/* USER CODE BEGIN Header */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
#define XOUT_PORT   GPIO_PORT_H
#define XOUT_PIN    GPIO_PIN_00

#define XIN_PORT   GPIO_PORT_H
#define XIN_PIN    GPIO_PIN_01

#define MIC_PORT   GPIO_PORT_A
#define MIC_PIN    GPIO_PIN_01

#define RF_BUSY_PORT   GPIO_PORT_A
#define RF_BUSY_PIN    GPIO_PIN_02

#define RF_CS_PORT   GPIO_PORT_A
#define RF_CS_PIN    GPIO_PIN_03

#define RF_IRQ_PORT   GPIO_PORT_B
#define RF_IRQ_PIN    GPIO_PIN_00

#define C3_PORT   GPIO_PORT_B
#define C3_PIN    GPIO_PIN_01

#define C1_PORT   GPIO_PORT_B
#define C1_PIN    GPIO_PIN_02

#define C2_PORT   GPIO_PORT_B
#define C2_PIN    GPIO_PIN_10

#define R3_PORT   GPIO_PORT_B
#define R3_PIN    GPIO_PIN_12

#define R2_PORT   GPIO_PORT_B
#define R2_PIN    GPIO_PIN_13

#define R1_PORT   GPIO_PORT_B
#define R1_PIN    GPIO_PIN_14

#define R0_PORT   GPIO_PORT_B
#define R0_PIN    GPIO_PIN_15

#define BATT_MEAS_PORT   GPIO_PORT_A
#define BATT_MEAS_PIN    GPIO_PIN_15

#define PTT_PORT   GPIO_PORT_B
#define PTT_PIN    GPIO_PIN_03

#define RF_RST_PORT   GPIO_PORT_B
#define RF_RST_PIN    GPIO_PIN_04

#define OLED_CS_PORT   GPIO_PORT_B
#define OLED_CS_PIN    GPIO_PIN_05

#define OLED_DC_PORT   GPIO_PORT_B
#define OLED_DC_PIN    GPIO_PIN_06

#define OLED_RST_PORT   GPIO_PORT_B
#define OLED_RST_PIN    GPIO_PIN_07

#define RED_PORT   GPIO_PORT_B
#define RED_PIN    GPIO_PIN_08

#define GREEN_PORT   GPIO_PORT_B
#define GREEN_PIN    GPIO_PIN_09

#define X32_OUT_PORT   GPIO_PORT_C
#define X32_OUT_PIN    GPIO_PIN_14

#define X32_IN_PORT   GPIO_PORT_C
#define X32_IN_PIN    GPIO_PIN_15

#define C0_PORT   GPIO_PORT_A
#define C0_PIN    GPIO_PIN_04

#define VBATT_PORT   GPIO_PORT_A
#define VBATT_PIN    GPIO_PIN_00

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* INT_SRC_PORT_EIRQ0 Callback. */
static void INT_SRC_PORT_EIRQ0_IrqCallback(void);
/* INT_SRC_ADC1_EOCA Callback. */
static void INT_SRC_ADC1_EOCA_IrqCallback(void);
/* Configures EIRQ. */
static void App_EIRQCfg(void);
/* Configures ADC. */
static void App_ADCCfg(void);
/* Configures USARTx. */
static void App_USARTxCfg(void);
/* Configures SPIx. */
static void App_SPIxCfg(void);
/* Configures RTC. */
static void App_RTCCfg(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

//Clock Config
static void App_ClkCfg(void)
{
    /* Set bus clock div. */
    CLK_SetClockDiv(CLK_BUS_CLK_ALL, (CLK_HCLK_DIV1 | CLK_EXCLK_DIV2 | CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | \
                                   CLK_PCLK2_DIV4 | CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2));
    /* sram init include read/write wait cycle setting */
    SRAM_SetWaitCycle(SRAM_SRAM_ALL, SRAM_WAIT_CYCLE1, SRAM_WAIT_CYCLE1);
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
    /* flash read wait cycle setting */
    EFM_SetWaitCycle(EFM_WAIT_CYCLE3);
    /* XTAL config */
    stc_clock_xtal_init_t stcXtalInit;
    (void)CLK_XtalStructInit(&stcXtalInit);
    stcXtalInit.u8State = CLK_XTAL_ON;
    stcXtalInit.u8Drv = CLK_XTAL_DRV_HIGH;
    stcXtalInit.u8Mode = CLK_XTAL_MD_OSC;
    stcXtalInit.u8StableTime = CLK_XTAL_STB_2MS;
    (void)CLK_XtalInit(&stcXtalInit);
    /* MPLL config */
    stc_clock_pll_init_t stcMPLLInit;
    (void)CLK_PLLStructInit(&stcMPLLInit);
    stcMPLLInit.PLLCFGR = 0UL;
    stcMPLLInit.PLLCFGR_f.PLLM = (1UL - 1UL);
    stcMPLLInit.PLLCFGR_f.PLLN = (25UL - 1UL);
    stcMPLLInit.PLLCFGR_f.PLLP = (2UL - 1UL);
    stcMPLLInit.PLLCFGR_f.PLLQ = (2UL - 1UL);
    stcMPLLInit.PLLCFGR_f.PLLR = (2UL - 1UL);
    stcMPLLInit.u8PLLState = CLK_PLL_ON;
    stcMPLLInit.PLLCFGR_f.PLLSRC = CLK_PLL_SRC_XTAL;
    (void)CLK_PLLInit(&stcMPLLInit);
    /* 2 cycles for 42MHz ~ 126MHz */
    GPIO_SetReadWaitCycle(GPIO_RD_WAIT2);
    /* Set the system clock source */
    CLK_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);
}

//Port Config
static void App_PortCfg(void)
{
    /* GPIO initialize */
    stc_gpio_init_t stcGpioInit;
    /* PH0 set to XTAL-EXT/XTAL-OUT */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(XOUT_PORT, XOUT_PIN, &stcGpioInit);

    /* PH1 set to XTAL-IN */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(XIN_PORT, XIN_PIN, &stcGpioInit);

    /* PA1 set to ADC1-IN1 */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(MIC_PORT, MIC_PIN, &stcGpioInit);

    /* PA2 set to GPIO-Input */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(RF_BUSY_PORT, RF_BUSY_PIN, &stcGpioInit);

    /* PA3 set to GPIO-Output */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinState = PIN_STAT_SET;
    stcGpioInit.u16PinDrv = PIN_HIGH_DRV;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(RF_CS_PORT, RF_CS_PIN, &stcGpioInit);

    /* PB0 set to EIRQ0 */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExtInt = PIN_EXTINT_ON;
    (void)GPIO_Init(RF_IRQ_PORT, RF_IRQ_PIN, &stcGpioInit);

    /* PB1 set to GPIO-Input */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PullUp = PIN_PU_ON;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(C3_PORT, C3_PIN, &stcGpioInit);

    /* PB2 set to GPIO-Input */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PullUp = PIN_PU_ON;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(C1_PORT, C1_PIN, &stcGpioInit);

    /* PB10 set to GPIO-Input */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PullUp = PIN_PU_ON;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(C2_PORT, C2_PIN, &stcGpioInit);

    /* PB12 set to GPIO-Output */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(R3_PORT, R3_PIN, &stcGpioInit);

    /* PB13 set to GPIO-Output */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(R2_PORT, R2_PIN, &stcGpioInit);

    /* PB14 set to GPIO-Output */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(R1_PORT, R1_PIN, &stcGpioInit);

    /* PB15 set to GPIO-Output */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(R0_PORT, R0_PIN, &stcGpioInit);

    /* PA15 set to GPIO-Output */
    GPIO_SetDebugPort(GPIO_PIN_TDI, DISABLE);
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(BATT_MEAS_PORT, BATT_MEAS_PIN, &stcGpioInit);

    /* PB3 set to GPIO-Input */
    GPIO_SetDebugPort(GPIO_PIN_TDO, DISABLE);
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PullUp = PIN_PU_ON;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(PTT_PORT, PTT_PIN, &stcGpioInit);

    /* PB4 set to GPIO-Output */
    GPIO_SetDebugPort(GPIO_PIN_TRST, DISABLE);
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(RF_RST_PORT, RF_RST_PIN, &stcGpioInit);

    /* PB5 set to GPIO-Output */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinState = PIN_STAT_SET;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(OLED_CS_PORT, OLED_CS_PIN, &stcGpioInit);

    /* PB6 set to GPIO-Output */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(OLED_DC_PORT, OLED_DC_PIN, &stcGpioInit);

    /* PB7 set to GPIO-Output */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinState = PIN_STAT_SET;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(OLED_RST_PORT, OLED_RST_PIN, &stcGpioInit);

    /* PB8 set to GPIO-Output */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(RED_PORT, RED_PIN, &stcGpioInit);

    /* PB9 set to GPIO-Output */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(GREEN_PORT, GREEN_PIN, &stcGpioInit);

    /* PC14 set to XTAL32-OUT */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(X32_OUT_PORT, X32_OUT_PIN, &stcGpioInit);

    /* PC15 set to XTAL32-IN */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(X32_IN_PORT, X32_IN_PIN, &stcGpioInit);

    /* PA4 set to GPIO-Input */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PullUp = PIN_PU_ON;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    (void)GPIO_Init(C0_PORT, C0_PIN, &stcGpioInit);

    /* PA0 set to ADC1-IN0 */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(VBATT_PORT, VBATT_PIN, &stcGpioInit);

    GPIO_SetFunc(GPIO_PORT_A,GPIO_PIN_05,GPIO_FUNC_43);//SPI1-SCK
    
    GPIO_SetFunc(GPIO_PORT_A,GPIO_PIN_06,GPIO_FUNC_41);//SPI1-MISO
    
    GPIO_SetFunc(GPIO_PORT_A,GPIO_PIN_07,GPIO_FUNC_40);//SPI1-MOSI
    
    GPIO_SetFunc(GPIO_PORT_A,GPIO_PIN_08,GPIO_FUNC_32);//USART1-TX
    
    GPIO_SetFunc(GPIO_PORT_A,GPIO_PIN_09,GPIO_FUNC_33);//USART1-RX
    
}

//Int Config
static void App_IntCfg(void)
{
    stc_irq_signin_config_t stcIrq;

    /* IRQ sign-in */
    stcIrq.enIntSrc = INT_SRC_PORT_EIRQ0;
    stcIrq.enIRQn = INT032_IRQn;
    stcIrq.pfnCallback = &INT_SRC_PORT_EIRQ0_IrqCallback;
    (void)INTC_IrqSignIn(&stcIrq);
    /* NVIC config */
    NVIC_ClearPendingIRQ(INT032_IRQn);
    NVIC_SetPriority(INT032_IRQn, DDL_IRQ_PRIO_15);
    NVIC_EnableIRQ(INT032_IRQn);

    /* IRQ sign-in */
    stcIrq.enIntSrc = INT_SRC_ADC1_EOCA;
    stcIrq.enIRQn = INT116_IRQn;
    stcIrq.pfnCallback = &INT_SRC_ADC1_EOCA_IrqCallback;
    (void)INTC_IrqSignIn(&stcIrq);
    /* NVIC config */
    NVIC_ClearPendingIRQ(INT116_IRQn);
    NVIC_SetPriority(INT116_IRQn, DDL_IRQ_PRIO_15);
    NVIC_EnableIRQ(INT116_IRQn);

}

/**
 * @brief  Main function of the project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* USER CODE BEGIN Main0 */

    /* USER CODE END Main0 */
    /* Register write unprotected for some required peripherals. */
    LL_PERIPH_WE(LL_PERIPH_ALL);
    //Clock Config
    App_ClkCfg();
    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */
    //Port Config
    App_PortCfg();
    //Int Config
    App_IntCfg();
    //EIRQ Config
    App_EIRQCfg();
    //ADC Config
    App_ADCCfg();
    //USARTx Config
    App_USARTxCfg();
    //SPIx Config
    App_SPIxCfg();
    //RTC Config
    App_RTCCfg();
    /* Register write protected for some required peripherals. */
    LL_PERIPH_WP(LL_PERIPH_ALL);
    /* USER CODE BEGIN Main1 */

    /* USER CODE END Main1 */
    for (;;) 
    {
        /* USER CODE BEGIN FOR */

        /* USER CODE END FOR */
    }
}

/* INT_SRC_PORT_EIRQ0 Callback. */
static void INT_SRC_PORT_EIRQ0_IrqCallback(void)
{
    /* USER CODE BEGIN INT_SRC_PORT_EIRQ0_IrqCallback */

    /* USER CODE END INT_SRC_PORT_EIRQ0_IrqCallback */
}
/* INT_SRC_ADC1_EOCA Callback. */
static void INT_SRC_ADC1_EOCA_IrqCallback(void)
{
    /* USER CODE BEGIN INT_SRC_ADC1_EOCA_IrqCallback */

    /* USER CODE END INT_SRC_ADC1_EOCA_IrqCallback */
}

//EIRQ Config
static void App_EIRQCfg(void)
{
    stc_extint_init_t stcExtIntInit;

    /* EXTINT_CH00 config */
    (void)EXTINT_StructInit(&stcExtIntInit);
    stcExtIntInit.u32Filter = EXTINT_FILTER_ON;
    stcExtIntInit.u32FilterClock = EXTINT_FCLK_DIV1;
    stcExtIntInit.u32Edge = EXTINT_TRIG_RISING;
    (void)EXTINT_Init(EXTINT_CH00, &stcExtIntInit);

}

//ADC Config
static void App_ADCCfg(void)
{
    /* USER CODE BEGIN ADC 0 */

    /* USER CODE END ADC 0 */
    //independent mode
    //ADC1 config 
    stc_adc_init_t stcAdcInit;

    /* 1. Enable ADC1 peripheral clock. */
    FCG_Fcg3PeriphClockCmd(FCG3_PERIPH_ADC1, ENABLE);

    /* 2. Modify the default value depends on the application. */
    (void)ADC_StructInit(&stcAdcInit);
    stcAdcInit.u16ScanMode = ADC_MD_SEQA_SINGLESHOT;
    stcAdcInit.u16Resolution = ADC_RESOLUTION_12BIT;
    stcAdcInit.u16DataAlign = ADC_DATAALIGN_RIGHT;

    /* 3. Initializes ADC. */
    (void)ADC_Init(CM_ADC1, &stcAdcInit);

    /* 4. ADC sequence configuration. */
    /* ADC sequence A configuration. */
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH0, ENABLE);
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH1, ENABLE);
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH16, ENABLE);

    /* Configure PWR monitor and enable it. */
    FCG_Fcg3PeriphClockCmd(FCG3_PERIPH_CMP, ENABLE);
    PWC_PowerMonitorCmd(ENABLE);
    CMP_8BitDAC_AdcRefCmd(CMP_ADC_REF_VREF, ENABLE);
    /* ADC Int configuration */
    ADC_IntCmd(CM_ADC1, ADC_INT_EOCA, ENABLE);
    /* USER CODE BEGIN ADC 1 */

    /* USER CODE END ADC 1 */
}

//USARTx Config
static void App_USARTxCfg(void)
{
    /* USER CODE BEGIN USARTx 0 */

    /* USER CODE END USARTx 0 */
    stc_usart_uart_init_t stcUartInit;

    /* Enable USART1 clock */
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_USART1, ENABLE);
    /************************* Configure USART1***************************/
    USART_DeInit(CM_USART1);
    (void)USART_UART_StructInit(&stcUartInit);
    stcUartInit.u32ClockSrc = USART_CLK_SRC_INTERNCLK;
    stcUartInit.u32ClockDiv = USART_CLK_DIV1;
    stcUartInit.u32CKOutput = USART_CK_OUTPUT_DISABLE;
    stcUartInit.u32Baudrate = 115200UL;
    stcUartInit.u32DataWidth = USART_DATA_WIDTH_8BIT;
    stcUartInit.u32StopBit = USART_STOPBIT_1BIT;
    stcUartInit.u32Parity = USART_PARITY_NONE;
    stcUartInit.u32OverSampleBit = USART_OVER_SAMPLE_16BIT;
    stcUartInit.u32FirstBit = USART_FIRST_BIT_LSB;
    stcUartInit.u32StartBitPolarity = USART_START_BIT_FALLING;
    stcUartInit.u32HWFlowControl = USART_HW_FLOWCTRL_RTS;
    USART_UART_Init(CM_USART1, &stcUartInit, NULL);
    /* Enable USART_TX | USART_RX function */
    USART_FuncCmd(CM_USART1, (USART_TX | USART_RX), ENABLE);
    /* USER CODE BEGIN USARTx 1 */

    /* USER CODE END USARTx 1 */
}

//SPIx Config
static void App_SPIxCfg(void)
{
    /* USER CODE BEGIN SPIx 0 */

    /* USER CODE END SPIx 0 */
    stc_spi_init_t stcSpiInit;
    stc_spi_delay_t stcSpiDelay;

    /* Enable SPI1 clock */
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_SPI1, ENABLE);
    /************************* Configure SPI1***************************/
    SPI_StructInit(&stcSpiInit);
    stcSpiInit.u32WireMode = SPI_4_WIRE;
    stcSpiInit.u32TransMode = SPI_FULL_DUPLEX;
    stcSpiInit.u32MasterSlave = SPI_MASTER;
    stcSpiInit.u32Parity = SPI_PARITY_INVD;
    stcSpiInit.u32SpiMode = SPI_MD_1;
    stcSpiInit.u32BaudRatePrescaler = SPI_BR_CLK_DIV16;
    stcSpiInit.u32DataBits = SPI_DATA_SIZE_32BIT;
    stcSpiInit.u32FirstBit = SPI_FIRST_MSB;
    stcSpiInit.u32SuspendMode = SPI_COM_SUSP_FUNC_OFF;
    stcSpiInit.u32FrameLevel = SPI_1_FRAME;
    (void)SPI_Init(CM_SPI1, &stcSpiInit);

    SPI_DelayStructInit(&stcSpiDelay);
    stcSpiDelay.u32IntervalDelay = SPI_INTERVAL_TIME_1SCK;
    stcSpiDelay.u32ReleaseDelay = SPI_RELEASE_TIME_1SCK;
    stcSpiDelay.u32SetupDelay = SPI_SETUP_TIME_1SCK;
    (void)SPI_DelayTimeConfig(CM_SPI1, &stcSpiDelay);

    /* SPI loopback function configuration */
    SPI_SetLoopbackMode(CM_SPI1, SPI_LOOPBACK_INVD);
    /* SPI parity check error self diagnosis configuration */
    SPI_ParityCheckCmd(CM_SPI1, DISABLE);
    /* SPI valid SS signal configuration */
    SPI_SSPinSelect(CM_SPI1, SPI_PIN_SS0);
    /* SPI SS signal valid level configuration */
    SPI_SetSSValidLevel(CM_SPI1, SPI_PIN_SS0, SPI_SS_VALID_LVL_LOW);
    /* Enable SPI1 */
    SPI_Cmd(CM_SPI1, ENABLE);
    /* USER CODE BEGIN SPIx 1 */

    /* USER CODE END SPIx 1 */
}

//RTC Config
static void App_RTCCfg(void)
{
    /* USER CODE BEGIN RTC 0 */

    /* USER CODE END RTC 0 */
    stc_rtc_init_t stcRtcInit;
    stc_rtc_date_t stcRtcDate;
    stc_rtc_time_t stcRtcTime;
    stc_rtc_alarm_t stcRtcAlarm;

    /* Reset RTC counter */
    if (LL_ERR_TIMEOUT != RTC_DeInit()) {
        /* Configure structure initialization */
        (void)RTC_StructInit(&stcRtcInit);

        /* Configuration RTC structure */
        stcRtcInit.u8ClockSrc = RTC_CLK_SRC_XTAL32;
        stcRtcInit.u8HourFormat = RTC_HOUR_FMT_24H;
        stcRtcInit.u8ClockCompen = RTC_CLK_COMPEN_DISABLE;
        (void)RTC_Init(&stcRtcInit);

        /* Configuration alarm clock time */
        stcRtcAlarm.u8AlarmHour = 0x12U;
        stcRtcAlarm.u8AlarmMinute = 0x0U;
        stcRtcAlarm.u8AlarmWeekday = 0;
        stcRtcAlarm.u8AlarmAmPm = RTC_HOUR_12H_AM;
        (void)RTC_SetAlarm(RTC_DATA_FMT_BCD, &stcRtcAlarm);
        RTC_AlarmCmd(ENABLE);

        /* Update date and time */
        /* Date configuration */
        stcRtcDate.u8Year = 0U;
        stcRtcDate.u8Month = RTC_MONTH_JANUARY;
        stcRtcDate.u8Day = 1U;
        stcRtcDate.u8Weekday = RTC_WEEKDAY_SUNDAY;

        /* Time configuration */
        stcRtcTime.u8Hour = 12U;
        stcRtcTime.u8Minute = 0U;
        stcRtcTime.u8Second = 0U;
        stcRtcTime.u8AmPm = RTC_HOUR_12H_AM;

        if (LL_OK != RTC_SetDate(RTC_DATA_FMT_DEC, &stcRtcDate)) {
            return;
        }

        if (LL_OK != RTC_SetTime(RTC_DATA_FMT_DEC, &stcRtcTime)) {
            return;
        }
        /* Startup RTC count */
        RTC_Cmd(ENABLE);
    }
    /* USER CODE BEGIN RTC 1 */

    /* USER CODE END RTC 1 */
}

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
