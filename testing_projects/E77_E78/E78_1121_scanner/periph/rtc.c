#include "rtc.h"

rtc_calendar_t timestamp;
bool rtc_flag;


void rtc_init(void)
{
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_AFEC, true);
  // enable the clk
  rcc_enable_oscillator(RCC_OSC_XO32K, true);
  rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, true);
	rtc_set(2024,10,24,23,59,00);
	rtc_config_interrupt(RTC_SEC_IT, ENABLE);
	rtc_calendar_cmd(true);
	NVIC_ClearPendingIRQ(RTC_IRQn);
	NVIC_EnableIRQ(RTC_IRQn);
}

void RTC_IRQHandler(void)
{
	//uint8_t intr_stat;

	//intr_stat = rtc_get_status(RTC_ALARM0_SR);
	//if (intr_stat == true)
	//{
	//	rtc_config_interrupt(RTC_SEC_IT, DISABLE);
	//	rtc_set_status(RTC_ALARM0_SR, false);
	//	rtc_config_interrupt(RTC_SEC_IT, ENABLE);
  //}
	rtc_clear_sec_it_status();
	rtc_get_calendar(&timestamp);
	rtc_flag = true;
}

void rtc_set(uint16_t y,uint8_t mon,uint8_t d,uint8_t h,uint8_t min,uint8_t s)
{
	//disable interrupts
	NVIC_DisableIRQ(RTC_IRQn);
	//set date/time
	timestamp.year = y;
	timestamp.month = mon;
	timestamp.day = d;
	timestamp.hour = h;
	timestamp.minute = min;
	timestamp.second = s;
	rtc_set_calendar(&timestamp);
	NVIC_EnableIRQ(RTC_IRQn);
}