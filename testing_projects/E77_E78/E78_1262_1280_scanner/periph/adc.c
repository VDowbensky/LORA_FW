#include "adc.h"

float Vcc;
float T;

const float Vref =  1200.0f; //mV
//static uint8_t phase;

float gain_value;
float dco_value;

void myadc_init(void)
{
	rcc_set_adc_clk_source(RCC_ADC_CLK_SOURCE_PCLK1);
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_ADC, true);
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_AFEC, true);
	
	gpio_init(VBATT_MEAS_PORT,VBATT_MEAS_PIN,GPIO_MODE_OUTPUT_PP_LOW);
	//test pin
	gpio_init(VBATT_PORT, VBATT_PIN, GPIO_MODE_ANALOG);
	
	adc_get_calibration_value(false, &gain_value, &dco_value);
	dco_value *= 1000.0;
	adc_init();
	adc_config_clock_division(8);
	adc_config_ref_voltage(ADC_INTERNAL_REF_VOLTAGE);
	//It not works now. Temp sensor and Vcc divider must be enabled in AFEC!
	//adc_enable_vbat31(true);
	adc_config_sample_sequence(0, ADC_SAMPLE_CHAN_1);
	adc_config_conv_mode(ADC_CONV_MODE_CONTINUE);
  adc_enable(true);
	//enable interrupt
	adc_config_interrupt(ADC_IER_EOS,true); //ADC_IER_EOS
	NVIC_ClearPendingIRQ(ADC_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);
	vbatt_meas_enable();
	adc_start(true);
}

void ADC_IRQHandler(void)
{
	uint16_t adc_val;
	float mv;
	
	if(adc_get_interrupt_status(ADC_ISR_EOS))
	{
		adc_clear_interrupt_status(ADC_ISR_EOS);//ADC_ISR_EOS
		adc_start(false);
		adc_val = adc_get_data();
		mv = ((Vref/4096.0) * adc_val - dco_value) / gain_value;
		Vcc = (mv * 11.45) / 1000.0;
		//vbatt_meas_disable();
	}
}

void vbatt_meas_enable(void)
{
	gpio_write(VBATT_MEAS_PORT,VBATT_MEAS_PIN,GPIO_LEVEL_HIGH);
}

void vbatt_meas_disable(void)
{
	//adc_start(false);
	gpio_write(VBATT_MEAS_PORT,VBATT_MEAS_PIN,GPIO_LEVEL_LOW);
}

void kickADC(void)
{
	//vbatt_meas_enable();
	//adc_config_sample_sequence(0, ADC_SAMPLE_CHAN_1);
	//adc_config_sample_sequence(1, ADC_SAMPLE_CHAN_1);
	//adc_enable(true);
	adc_start(true);
}
