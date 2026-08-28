#include "lr2021_common.h"

extern uint16_t LR2021_status[];
extern uint32_t LR2021_irqstatus[];
extern uint16_t LR2021_errors[];



//Returns status of device. If the previous command was not a read command, returns the status register. If previous transaction was a read command, returns the result of the read command. Also clears the reset status returned in the Stat variable
void LR2021_GetStatus(uint8_t lr)
{
  uint8_t buf[4];
  
  LR2021_readCmd(lr,LR2021_GET_STATUS,NULL,0,buf,4);
  LR2021_irqstatus[lr] = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | buf[3];
}

//Returns version of firmware. Check!!!
void LR2021_GetVersion(uint8_t lr,uint8_t *ver_major,uint8_t *ver_minor) 
{
  uint8_t buf[2];
    
  LR2021_readCmd(lr,LR2021_GET_VERSION,NULL,0,buf,2);
  *ver_major = buf[0];
  *ver_minor = buf[1];
}

//Returns current pending errors that occurred since the last ClearErrors() call, or the startup of the circuit. It is possible to generate an Irq on DIO when an error occurs
void LR2021_GetErrors(uint8_t lr)
{
  uint8_t buf[4];
    
  LR2021_readCmd(lr,LR2021_GET_ERRORS,NULL,0,buf,4);
  LR2021_errors[lr] = ((uint16_t)buf[0] << 8) | buf[1];
}

//Clears all error flags in the status. The flags cannot be cleared individually. Calling ClearErrors will not clear the Error IRQ - the IRQ has to be cleared explicitly with the ClearIrq command
void LR2021_ClearErrors(uint8_t lr)
{
  LR2021_writeCmd(lr,LR2021_CLEAR_ERRORS,NULL,0);
}

//Configure the functionality of the freely configurable DIOs, as well as the pull-up/down configuration for sleep modes. On DIO5, only DIO_SLEEP_PULL_UP is accepted. DIO5/6 have pull-up by default
void LR2021_SetDioFunction(uint8_t dio,uint8_t dio_func,uint8_t pull_drive)
{
  uint8_t buf[2];
  
  buf[0] = dio & 0x0f;
  buf[1] = (dio_func & 0x0f) << 4;
  buf[1] |= pull_drive & 0x0f;
  LR2021_writeCmd(lr,LR2021_SET_DIO_FUNC,buf,2);
}

//Configure the value of the specified DIO pin when configured as RF switch with the SetDioFunction command
void LR2021_SetDioAsRfSwitch(uint8_t lr,uint8_t dio,bool tx_hf,bool rx_hf,bool tx_lf,bool rx_lf,bool standby) 
{
  uint8_t buf[2] = {0,0};
  
  buf[0] = dio & 0x0f;
  if(tx_hf) buf[1] |= 16; //check!!!
  if(rx_hf) buf[1] |= 8;
  if(tx_lf) buf[1] |= 2;
  if(rx_lf) buf[1] |= 1;
  LR2021_writeCmd(lr,LR2021_SET_DIO_RFSWITCH_CFG,buf,2);
}

//Clears specific fifo irq flags
void LR2021_ClearFifoIrqFlags(uint8_t lr,uint8_t rx_fifo_flags_to_clear,uint8_t tx_fifo_flags_to_clear) 
{
  uint8_t buf[2];

  buf[0] = rx_fifo_flags_to_clear;
  buf[1] = tx_fifo_flags_to_clear;
  LR2021_writeCmd(lr,LR2021_CLR_FIFO_IRQ_FLAGS,buf,2);
}

//Configure IRQs which assert DIO pin
void LR2021_SetDioIrqConfig(uint8_t lr,uint8_t dio,uint32_t irqs) 
{
  uint8_t buf[5];

  buf[0] = dio & 0x0f;
  buf[1] = (irqs >> 24) & 0xff;
  buf[2] = (irqs >> 16) & 0xff;
  buf[3] = (irqs >> 8) & 0xff;
  buf[4] = irqs & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_DIO_IRQ_PARAMS,buf,5);
}

//Clear pending irqs
void LR2021_ClearIrqFlags(uint8_t lr,uint32_t irqs) 
{
  uint8_t buf[4];

  buf[0] = (irqs >> 24) & 0xff;
  buf[1] = (irqs >> 16) & 0xff;
  buf[2] = (irqs >> 8) & 0xff;
  buf[3] = irqs & 0xff;
  LR2021_writeCmd(lr,LR2021_CLEAR_IRQ,buf,4);
}

//Read out the pending IRQs and clear all pending IRQs
void LR2021_GetClearIrq(uint8_t lr)
{
  uint8_t buf[4];
  
  LR2021_readCmd(lr,LR2021_GET_CLEAR_IRQ,NULL,0,buf,4);
  LR2021_irqstatus = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | buf[3];
}

//Configures the used Low Frequency clock
void LR2021_ConfigLfClk(uint8_t lr,uint8_t lfclock)
{
  lfclock &= 0x03;
  LR2021_writeCmd(lr,LR2021_CONFIG_LF_CLOCK,&lfclock,1);
}

//Configure LF and HF clock configuration for DIO output
void LR2021_ConfigClkOutput(uint8_t lr,uint8_t hf_clk_out_scaling)
{
  LR2021_writeCmd(lr,LR2021_CONFIG_CLK_OUT,&hf_clk_out_scaling,1);
}

//Configure which FIFO status flags generate FIFO IRQs as well as the threshold levels
void LR2021_ConfigFifoIrq(uint8_t lr,uint8_t rx_fifo_irq_enable,uint8_t tx_fifo_irq_enable,uint16_t rx_high_threshold,uint16_t tx_low_threshold,uint16_t rx_low_threshold,uint16_t tx_high_threshold)
{
  uint8_t buf[10];

  buf[0] = rx_fifo_irq_enable;
  buf[1] = tx_fifo_irq_enable;
  buf[2] = (rx_high_threshold >> 8) & 0xff;
  buf[3] = rx_high_threshold & 0xff;
  buf[4] = (tx_low_threshold >> 8) & 0xff;
  buf[5] = tx_low_threshold & 0xff;
  buf[6] = (rx_low_threshold >> 8) & 0xff;
  buf[7] = rx_low_threshold & 0xff;
  buf[8] = (tx_high_threshold >> 8) & 0xff;
  buf[9] = tx_high_threshold & 0xff;
  LR2021_writeCmd(lr,LR2021_CONFIG_FIFO_IRQ,buf,10);
}

//Returns all FIFO flags that have been triggered since the last flags clear. It does not depend on enabled flags for the IRQs
void LR2021_GetFifoIrqFlags(uint8_t lr,uint8_t *rxflags,uint8_t *txflags)
{
  uint8_t buf[2];
  
  LR2021_readCmd(lr,LR2021_GET_FIFO_IRQ_FLAGS,NULL,0,buf,2);
  *rxflags = buf[0];
  *txflags = buf[1];
}

//Read out the RX FIFO level in bytes
uint16_t LR2021_GetRxFifoLevel(uint8_t lr)
{
  uint8_t buf[2];
  
  LR2021_readCmd(lr,LR2021_GET_RX_FIFO_LEVEL,NULL,0,buf,2);
  return ((uint16_t)buf[0] << 8) | buf[1];
}

//Read out the TX FIFO level in bytes
uint16_t LR2021_GetTxFifoLevel(uint8_t lr)
{
  uint8_t buf[2];
  
  LR2021_readCmd(lr,LR2021_GET_TX_FIFO_LEVEL,NULL,0,buf,2);
  return ((uint16_t)buf[0] << 8) | buf[1];
}

//Clears the RX FIFO
void LR2021_ClearRxFifo(uint8_t lr)
{
  LR2021_writeCmd(lr,LR2021_CLEAR_RX_FIFO,NULL,0);
}

//Clears the TX FIFO
void LR2021_ClearTxFifo(uint8_t lr) 
{
  LR2021_writeCmd(lr,LR2021_CLEAR_TX_FIFO,NULL,0);
}

//Configures the SIMO mode
void LR2021_SetRegMode(uint8_t lr,uint8_t simo_usage)
{
  simo_usage &= 0x03;
  LR2021_writeCmd(lr,LR2021_SET_REG_MODE,&simo_usage,1);
}

//Configures the SIMO mode and ramp times
void LR2021_SetRegModeAdv(uint8_t lr,uint8_t simo_usage,uint8_t ramp_time_rc2ru_unit,uint8_t ramp_time_rc2ru,uint8_t ramp_time_tx2ru_unit,
                          uint8_t ramp_time_tx2ru,uint8_t ramp_time_ru2rc_unit,uint8_t ramp_time_ru2rc,uint8_t ramp_time_ramp_down_unit,uint8_t ramp_time_ramp_down)
{
  uint8_t buf[4] = {0,0,0,0};

  buf[0] = simo_usage & 0x3;
  buf[1] |= (ramp_time_rc2ru_unit & 0x3) << 5;
  buf[1] |= ramp_time_rc2ru & 0x1f;
  buf[2] |= (ramp_time_tx2ru_unit & 0x3) << 5;
  buf[2] |= ramp_time_tx2ru & 0x1f;
  buf[3] |= (ramp_time_ru2rc_unit & 0x3) << 5;
  buf[3] |= ramp_time_ru2rc & 0x1f;
  buf[4] |= (ramp_time_ramp_down_unit & 0x3) << 5;
  buf[4] |= ramp_time_ramp_down & 0x1f;
  LR2021_writeCmd(lr,LR2021_SET_REG_MODE,buf,4);
}

//Calibrates the requested blocks defined by the blocks_to_calibrate parameter. Will work in any mode of the chip. On exit the chip will be in Standby RC
void LR2021_Calibrate(uint8_t lr,bool aaf,bool pa_offset,bool meas_unit,bool pll,bool hf_rc,bool lf_rc) 
{
  uint8_t val;

  if (pa_offset) val |= 64;
  if (meas_unit) val |= 16;
  if (aaf) val |= 8;
  if (pll) val |= 4;
  if (hf_rc) val |= 2;
  if (lf_rc) val |= 1;
  LR2021_writeCmd(lr,LR2021_CALIBRATE,&val,4);
}

//Launches all Front End calibrations (ADC Offset, PPF, Image) at the given frequencies. Will not work if device is in Rx or Tx mode
void LR2021_CalibrateFE(uint8_t lr,uint16_t freq1,uint16_t freq2,uint16_t freq3)
{
  uint8_t buf[6];

  buf[0] |= (freq1 >> 8) & 0xff;
  buf[1] |= freq1 & 0xff;
  buf[2] |= (freq2 >> 8) & 0xff;
  buf[3] |= freq2 & 0xff;
  buf[4] |= (freq3 >> 8) & 0xff;
  buf[5] |= freq3 & 0xff;
  LR2021_writeCmd(lr,LR2021_CALIB_FE,buf,6);
}

/// Measure and return current VBAT value
uint16_t LR2021_GetVbat(uint8_t lr,uint8_t vbat_format,uint8_t adc_res)
{
  uint8_t val;
  uint8_t buf[2];
  
  val = (vbat_format & 0x1) << 3;
  val |= adc_res & 0x7;
  LR2021_readCmd(lr,LR2021_GET_VBAT,&val,1,buf,2);
  return ((uint16_t)buf[0] << 8) | buf[1];
}

//Measure and return temperature value
float LR2021_GetTemp(uint8_t lr,uint8_t temp_src,uint8_t adc_res)
{
  uint8_t val;
  uint8_t buf[2];
  
  val = (temp_src & 0x3) << 4;
  val |= 8; // Force format to Celsius
  val |= adc_res & 0x7;
  LR2021_readCmd(lr,LR2021_GET_TEMP,&val,1,buf,2);
    
/*     /// Temperature in degrees Celsius (format=1)
    pub fn temp_celsius(&self) -> i16 {
        let raw = ((self.0[3] >> 3) as u16) |
            ((self.0[2] as u16) << 5);
        raw as i16 - if (self.0[2] & 0x80) != 0 {1<<13} else {0} */
/* impl AsMut<[u8]> for TempRsp {
    fn as_mut(&mut self) -> &mut [u8] {
        &mut self.0
    } */
/* }
#[cfg(feature = "defmt")]
impl defmt::Format for TempRsp {
    fn format(&self, fmt: defmt::Formatter) {
        defmt::write!(fmt, "{}.{:02}", self.0[2] as i8, (self.0[3] as u16 * 100) >> 8);
    } */
  return -273.15;

}

//Return a 32-bit random number
uint32_t LR2021_GetRandomNumber(uint8_t lr)
{
  uint8_t buf[4];
  
  LR2021_readCmd(lr,LR2021_GET_RND_NUMBER,NULL,0,buf,4);
  return ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | buf[3];
}

//Return a 32-bit random number
uint32_t LR2021_GetRandomNumberAdv(uint8_t lr,uint8_t source)
{
  uint8_t buf[4];
  
  source &= 0x03;
  LR2021_readCmd(lr,LR2021_GET_RND_NUMBER,&source,1,buf,4);
  return ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | buf[3];
}

//Put device in sleep mode
void LR2021_SetSleep(uint8_t lr)
{
  uint8_t val = 0;

  if (clk_32k_en) val |= 1; 
  val |= (ret_en & 0x0f) << 1;
  LR2021_writeCmd(lr,LR2021_SET_SLEEP,&val,1);
}

//Put device in sleep mode
void LR2021_SetSleepAdv(uint8_t lr,bool clk_32k_en,uint8_t ret_en,uint32_t sleep_time)
{
  uint8_t buf[5] = {0,0,0,0,0};

  if (clk_32k_en) buf[0] |= 1;
  buf[0] |= ret_en & 0x0f) << 1;
  buf[1] = (sleep_time >> 24) & 0xff;
  buf[2] = (sleep_time >> 16) & 0xff;
  buf[3] = (sleep_time >> 8) & 0xff;
  buf[4] = sleep_time & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_SLEEP,buf,5);
}

//Put device in standby mode (XOSC or RC)
void LR2021_SetStandby(uint8_t lr,uint8_t standby_mode)
{
  standby_mode &= 1;
  LR2021_writeCmd(lr,LR2021_SET_STANDBY,&standby_mode,1);
}

//Put device in Frequency Synthesis Mode
void LR2021_SetFs(uint8_t lr)
{
  LR2021_writeCmd(lr,LR2021_SET_FS,NULL,0);
}

//Specify address of additional register to save for sleep modes with retention
void LR2021_SetAssRegToRetain(uint8_t lr,uint8_t slot,uint32_t addr)
{
  uint8_t buf[4];

  buf[0] |= slot & 0x1f;
  buf[1] |= (addr >> 16) & 0xff;
  buf[2] |= (addr >> 8) & 0xff;
  buf[3] |= addr & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_ADD_REG_TO_RETAIN,buf,4);
}

//Read out and clear FIFO flags that generated FIFO IRQs
uint32_t LR2021_GetClrFifoIrqFlags(uint8_t lr) 
{
  uint8_t buf[4];
  
  LR2021_readCmd(lr,LR2021_GET_CLR_FIFO_IRQ_FLAGS,NULL,0,buf,4);
  return ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | buf[3];
}

//Enable/disable end of life detection and setup trigger voltage
void LR2021_SetEolConfig(uint8_t lr,uint8_t trim,bool enable) 
{
  uint8_t val;

  val = (trim as u8) & 0x07;
  if (enable) val |= 8;
  LR2021_writeCmd(lr,LR2021_SET_EOL_CONFIG,&val,1);
}

//Configure XOSC foot capacitor trim
void LR2021_SetXoscCpTrim(uint8_t lr,uint8_t xta,uint8_t xtb) 
{
  uint8_t buf[2];
  
  buf[0] = xta & 0x3f;
  buf[1] = xtb & 0x3f;
  LR2021_writeCmd(lr,LR2021_SET_XOSC_CP_TRIM,buf,2);
}

//Configure XOSC foot capacitor trim
void LR2021_SetXoscCpTrimAdv(uint8_t lr,uint8_t xta,uint8_t xtb,uint8_t delay_us) 
{
  uint8_t buf[3];
  
  buf[0] = xta & 0x3f;
  buf[1] = xtb & 0x3f;
  buf[2] = delay_us;
  LR2021_writeCmd(lr,LR2021_SET_XOSC_CP_TRIM,buf,3);
}

//Configure temperature compensation mode for TX and RX
void LR2021_SetTempCompCfg(uint8_t lr,uint8_t ntc,uint8_t comp_mode)
{
  uint8_t val;

  val = ((ntc as u8) & 0x01) << 2;
  val |= comp_mode & 0x03;
  LR2021_writeCmd(lr,LR2021_SET_TEMP_COMP_CFG,&val,1);
}

//Configure NTC parameters
void LR2021_SetNtcParams(uint8_t lr,uint16_t ntc_r_ratio,uint16_t ntc_beta,uint8_t delay)
{
  uint8_t buf[5];

  buf[0] = (ntc_r_ratio >> 8) & 0xff;
  buf[1] = ntc_r_ratio & 0xff;
  buf[2] = (ntc_beta >> 8) & 0xff;
  buf[3] = ntc_beta & 0xff;
  buf[4] = delay;
  LR2021_writeCmd(lr,LR2021_SET_NTC_PARAMS,buf,5);
}

// Common commands API
//Sets the RF frequency for subsequent radio operations. Will not work with the chip in TX mode. All frequency dependent parameters are automatically recomputed by the FW
void LR2021_SetRfFreq(uint8_t lr,uint32_t rf_freq) 
{
  uint8_t buf[4];
  
  buf[2] = (rf_freq >> 24) & 0xff;
  buf[3] = (rf_freq >> 16) & 0xff;
  buf[4] = (rf_freq >> 8) & 0xff;
  buf[5] = rf_freq & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_FREQ,buf,4);
}

//Sets the RX path and boost configuration. If rx_boost is changed, the SRC calibration (ADC offset) is run again for G12 and G13 with the updated boost configuration
void LR2021_SetRxPath(uint8_t lr,uint8_t rx_path)
{
  rx_path &= 0x01;
  LR2021_writeCmd(lr,LR2021_SET_RX_PATH,&rx_path,1);
}

//Sets the RX path and boost configuration. If rx_boost is changed, the SRC calibration (ADC offset) is run again for G12 and G13 with the updated boost configuration
void LR2021_SetRxPathAdv(uint8_t lr,uint8_t rx_path,uint8_t rx_boost) 
{
  uint8_t buf[2];

  buf[0] |= rx_path & 0x01;
  buf[1] |= rx_boost & 0x07;
  LR2021_writeCmd(lr,LR2021_SET_RX_PATH,buf,2);
}

//Chooses which PA to use and sets the parameters of the PA
void LR2021_SetPaConfig(uint8_t lr,uint8_t pa_sel,uint8_t pa_lf_mode,uint8_t pa_lf_duty_cycle,uint8_t pa_lf_slices)
{
  uint8_t buf[2];

  buf[0] = (pa_sel & 0x01) << 7;
  buf[0] |= pa_lf_mode & 0x03;
  buf[0] |= (pa_lf_duty_cycle & 0x0f) << 4;
  buf[1] = pa_lf_slices & 0x0f;
  LR2021_writeCmd(lr,LR2021_SET_PA_CONFIG,buf,2);
}

//Chooses which PA to use and sets the parameters of the PA
void LR2021_SetPaConfigAdv(uint8_t lr,uint8_t pa_sel,uint8_t pa_lf_mode,uint8_t pa_lf_duty_cycle,uint8_t pa_lf_slices,uint8_t pa_hf_duty_cycle)
{
  uint8_t buf[3];

  buf[0] = (pa_sel & 0x01) << 7;
  buf[0] |= pa_lf_mode & 0x03;
  buf[0] |= (pa_lf_duty_cycle & 0x0f) << 4;
  buf[1] = pa_lf_slices & 0x0f;
  buf[2] = pa_hf_duty_cycle & 0x1f;
  LR2021_writeCmd(lr,LR2021_SET_PA_CONFIG,buf,3);
}

//Sets the TX power and ramp time of the PA. The FW configures the corresponding registers, including OCP/OVP
void LR2021_SetTxParams(uint8_t lr,uint8_t tx_power,uint8_t ramp_time)
{
  uint8_t buf[2];

  buf[0] = tx_power;
  buf[1] = ramp_time;
  LR2021_writeCmd(lr,LR2021_SET_TX_PARAMS,buf,2);
}

//Configures the fallback mode after a RX or TX operation (after transmission/reception or timeout)
void LR2021_SetRxTxFallbackMode(uint8_t lr,uint8_t fallback_mode)
{
  fallback_mode &= 0x03;
  LR2021_writeCmd(lr,LR2021_SET_RXTX_FALLBACKMODE,&fallback_mode,1);
}

//Sets the current packet type. This is the first command to be sent when configuring the radio for transceiver operation. Will only work in Standby RC, Standby XOSC or FS mode
void LR2021_SetPacketType(uint8_t lr,uint8_t packet_type)
{
  LR2021_writeCmd(lr,LR2021_SET_PACKET_TYPE,&packet_type,1);
}

//Returns the current packet type of the radio
uint8_t LR2021_GetPacketType(uint8_t lr)
{
  uint8_t val;
  LR2021_readCmd(lr,LR2021_GET_PACKET_TYPE,NULL,0,&val,1);
  return val;
}

//Defines if the RX timeout should be stopped on Syncword/Header detection or on Preamble detection
void LR2021_SetStopTimeout(uint8_t lr,uint8_t stop_timeout)
{
  stop_timeout &= 0x01;
  LR2021_writeCmd(lr,LR2021_STOP_TIMEOUT,&stop_timeout,1);
}

//Reset Rx Statistics
void LR2021_ResetRxStats(uint8_t lr)
{
  LR2021_writeCmd(lr,LR2021_RESET_RX_STATS,NULL,0);
}

//Gets the instantaneous RSSI value during reception of the packet. Returned value corresponds to -rssi/2 [dBm]
int16_t LR2021_GetRssiInst(uint8_t lr) 
{
  uint8_t buf[2];
  
  LR2021_readCmd(lr,LR2021_GET_RSSI_INST,NULL,0,buf,2);
  return ((int16_t)buf[1] & 0x01) | ((int16_t)buf[0] << 1);
}

//Sets the device into RX mode. The RTC is started with the given value. RxTimeout is in 1/32.768kHz steps, allowing a maximum of 512 seconds timeout. If image rejection calibration was not done for current RF frequency, error RXFREQ_NO_CAL_ERR is generated
void LR2021_SetRx(uint8_t lr) 
{
  LR2021_writeCmd(lr,LR2021_SET_RX,NULL,0);
}

//Sets the device into RX mode. The RTC is started with the given value. RxTimeout is in 1/32.768kHz steps, allowing a maximum of 512 seconds timeout. If image rejection calibration was not done for current RF frequency, error RXFREQ_NO_CAL_ERR is generated
void LR2021_SetRxAdv(uint8_t lr,uint32_t rx_timeout) 
{
  uint8_t buf[3];
  
  rx_timeout &= 0x00ffffff;
  buf[0] = (rx_timeout >> 16) & 0xff;
  buf[1] = (rx_timeout >> 8) & 0xff;
  buf[2] = rx_timeout & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_RX,buf,3);
}

//Sets the device into TX mode. The RTC is started with the given value. TxTimeout is in 1/32.768kHz steps, allowing a maximum of 512 seconds timeout
void LR2021_SetTx(uint8_t lr)  
{
  LR2021_writeCmd(lr,LR2021_SET_TX,NULL,0);
}

//Sets the device into TX mode. The RTC is started with the given value. TxTimeout is in 1/32.768kHz steps, allowing a maximum of 512 seconds timeout
void LR2021_SetTxAdv(uint8_t lr,uint32_t tx_timeout)  
{
  uint8_t buf[3];
  
  tx_timeout &= 0x00ffffff;
  buf[0] = (tx_timeout >> 16) & 0xff;
  buf[1] = (tx_timeout >> 8) & 0xff;
  buf[2] = tx_timeout & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_TX,buf,3);
}

//Sets the device into TX test mode
void LR2021_SetTxTestMode(uint8_t lr,uint8_t test_mode) 
{
  LR2021_writeCmd(lr,LR2021_SET_TX_TEST,&test_mode,1);
}

//Select which PA to use. Configuration must have been provided beforehand using SetPaConfig. Selection cannot be changed in TX mode
void LR2021_SetPa(uint8_t lr,uint8_t pa_sel)
{
  pa_sel &= 0x01;
  LR2021_writeCmd(lr,LR2021_PA_SEL,&pa_sel,1);
}

//Activate or deactivate the auto TX/auto RX mode. In auto RX mode, chip automatically goes from TX to RX after TxDone. In auto TX mode, chip automatically goes from RX to TX after RxDone
void LR2021_SetAutoRxTx(uint8_t lr,bool clear,uint8_t auto_txrx_mode,uint32_t timeout,uint32_t delay) 
{
  uint8_t buf[7];
    
  timeout &= 0x00ffffff;
  buf[0] = auto_txrx_mode & 0x3;
  if (clear) buf[0] |= 128;
  buf[1] = (timeout >> 16) & 0xff;
  buf[2] = (timeout >> 8) & 0xff;
  buf[3] = timeout & 0xff;
  buf[4] = (delay >> 24) & 0xff;
  buf[5] = (delay >> 16) & 0xff;
  buf[6] = (delay >> 8) & 0xff;
  buf[7] = delay & 0xff;
  LR2021_writeCmd(lr,LR2021_AUTO_TX_RX,buf,7);
}

//Get the length of the last received packet
uint16_t LR2021_GetRxPktLen(uint32_t lr) 
{
  uint8_t buf[2];
  LR2021_readCmd(lr,LR2021_GET_RX_PKT_LEN,NULL,0,buf,2);
  return ((uint16_t)buf[0] << 8) | buf[1];
}

//Set the global value of the power offset
void LR2021_SetPowerOffset(uint8_t lr, uint8_t power_offset) 
{
  power_offset &= 0x3f;
  LR2021_writeCmd(lr,LR2021_SET_POWER_OFFSET,&power_offset,1);
}

//Sets the default RX and TX timeouts to be used for DIO RX/TX triggers, or if the timeout parameters are not sent in the SetRx and SetTx commands
void LR2021_SetDefaultRxTxTimeout(uint8_t lr,uint32_t rx_timeout,uint32_t tx_timeout) 
{
  uint8_t buf[6];
  
  rx_timeout &= 0x00ffffff;
  tx_timeout &= 0x00ffffff;
  buf[0] = ((rx_timeout >> 16) & 0xFF) as u8;
  buf[1] = ((rx_timeout >> 8) & 0xFF) as u8;
  buf[2] = (rx_timeout & 0xFF) as u8;
  buf[3] = ((tx_timeout >> 16) & 0xFF) as u8;
  buf[4] = ((tx_timeout >> 8) & 0xFF) as u8;
  buf[5] = (tx_timeout & 0xFF) as u8;
  LR2021_writeCmd(lr,LR2021_SET_DEFAULT_TIMEOUTS,buf,6);
}

//Sets the source event for time-stamping different radio events. 3 sources can be configured in parallel
void LR2021_SetTimestampSrc(uint8_t lr,uint8_t index,uint8_t source)
{
  uint8_t val;
  val = (index & 0x3) << 4;
  val |= source & 0x0f;
  LR2021_writeCmd(lr,LR2021_SET_TIMESTAMP_SRC,&val,1);
}

//Get the delay in HF clk tick between the event and the SPI NSS falling edge of the request. Will not return a correct value if the event occurred before a sleep period
uint32_t LR2021_GetTimestamp(uint8_t lr)
{
  uint8_t buf[4];
  
  LR2021_readCmd(lr,LR2021_GET_TIMESTAMP,NULL,0,buf,4);
  return ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | buf[3];
}

//Set the radio into RX mode for Clear Channel Assessment measurements. The radio measures the RSSI for the given duration
void LR2021_SetCca(uint8_t lr,uint32_t duration) 
{
  uint8_t buf[3];
  
  duration &= 0x00ffffff;
  buf[0] = (duration >> 16) & 0xff;
  buf[1] = (duration >> 8) & 0xff;
  buf[2] = duration & 0xff;
  LR2021_writeCmd(lr,LR2021_SET_CCA,buf,3);
}

//Set the radio into RX mode for Clear Channel Assessment measurements. The radio measures the RSSI for the given duration
void LR2021_SetCcaAdv(uint8_t lr,uint32_t duration,uint8_t gain)  
{
  uint8_t buf[4];

  duration &= 0x00ffffff;
  buf[0] = (duration >> 16) & 0xff;
  buf[1] = (duration >> 8) & 0xff;
  buf[2] = duration & 0xff;
  buf[3] = gain;
  LR2021_writeCmd(lr,LR2021_SET_CCA,buf,4);
}

//Get the RSSI statistics for the CCA measurement
void LR2021_GetCcaResult(uont8_t lr,int16_t *rssi_min,int16_t *rssi_max,int16_t *rssi_avg)
{
  uint8_t buf[6];
  
  LR2021_readCmd(lr,LR2021_GET_CCA_RESULT,NULL,0,buf,6);
  *rssi_min = (((int16_t)buf[3] >> 2) & 0x01) | ((int16_t)buf[0] << 1); //TODO: Check!!!
  *rssi_max = (((int16_t)buf[3] >> 1) & 0x01) | ((int16_t)buf[1] << 1);
  *rssi_avg = ((int16_t)buf[3] & 0x01) | ((int16_t)buf[2] << 1);
}

//Set the manual gain of the AGC. A value of 0 enables the AGC: automatic gain
void LR2021_SetAgcGainManual(uint8_t lr,uint8_t gain_step)
{
  gain_step &= 0x0f;
  LR2021_writeCmd(lr,LR2021_SET_AGC_GAIN_MAN,&gain_step,1);
}

//Set the CAD parameters for the Channel Activity Detect for packet types other than LoRa. This CAD is based on the measured RSSI
void LR2021_SetCadParams(uint8_t lr,uint32_t cad_timeout,uint8_t threshold,uint8_t exit_mode,uint32_t trx_timeout) 
{
  uint8_t buf[8];
  
  cad_timeout &= 0x00ffffff;
  trx_timeout &= 0x00ffffff;
  buf[0] = ((cad_timeout >> 16) & 0xFF) as u8;
  buf[1] = ((cad_timeout >> 8) & 0xFF) as u8;
  buf[2] = (cad_timeout & 0xFF) as u8;
  buf[3] = threshold;
  buf[4] = (exit_mode as u8) & 0x3;
  buf[5] = ((trx_timeout >> 16) & 0xFF) as u8;
  buf[6] = ((trx_timeout >> 8) & 0xFF) as u8;
  buf[7] = (trx_timeout & 0xFF) as u8;
  LR2021_writeCmd(lr,LR2021_SET_CAD_PARAMS,buf,8);
}

//Set device into RX CAD mode (not LoRa). Parameters must have been previously set using SetCadParams command
void LR2021_SetCad(uint8_t lr) 
{
  LR2021_writeCmd(lr,LR2021_SET_CAD,NULL,0);
}

// Commands with variable length parameters (not implemented):
// - SetRssiCalibration

//Regmem
//Writes a block of 32-bit words in register/memory space starting at a specific address. The address is auto-incremented after each data word so that data is stored in contiguous register/memory locations. Address must be 32-bit aligned and data length must be a multiple of 4. Maximum 32 words
void LR2021_WriteRegMem32(uint8_t lr,uint32_t addr,uint32_t data)
{
  uint8_t buf[7];

  buf[0] = (addr >> 16) & 0xff;
  buf[1] = (addr >> 8) & 0xff;
  buf[2] = addr & 0xff;
  buf[3] = (data >> 24) & 0xff;
  buf[4] = (data >> 16) & 0xff;
  buf[5] = (data >> 8) & 0xff;
  buf[6] = data & 0xff;
  LR2021_writeCmd(lr,LR2021_WRITE_REG_MEM32,buf,7);
}

//Read-Modify-Writes data at given register/memory address. Only the masked bits (Mask bits = 1) of a single 32-bit word are modified. Address must be 32-bit aligned
void LR2021_WriteRegMemMask32(uint8_t lr,uint32_t addr,uint32_t mask,uint32_t data) 
{
  uint8_t buf[11];

  buf[0] = (addr >> 16) & 0xff;
  buf[1] = (addr >> 8) & 0xff;
  buf[2] = addr & 0xff;
  buf[3] = (mask >> 24) & 0xff;
  buf[4] = (mask >> 16) & 0xff;
  buf[5] = (mask >> 8) & 0xff;
  buf[6] = mask & 0xff;
  buf[7] = (data >> 24) & 0xff;
  buf[8] = (data >> 16) & 0xff;
  buf[9] = (data >> 8) & 0xff;
  buf[10] = data & 0xff;
  LR2021_writeCmd(lr,LR2021_WRITE_REG_MEM_MASK32,buf,11);
}

//Reads a block of 32-bit words in register/memory space starting at a specific address. The address is auto-incremented after each data word so that data is read from contiguous register locations. Address must be 32-bit aligned. Maximum 64 words
uint32_t LR2021_ReadRegMem32(uint8_t lr,uint32_t addr,uint8_t len,uint32_t *dst) 
{
  uint8_t txbuf[4];
  uint8_t rxbuf[4];
  //uint8_t rxbuf[256];

  txbuf[0] = (addr >> 16) & 0xff;
  txbuf[1] = (addr >> 8) & 0xff;
  txbuf[2] = addr & 0xff;
  txbuf[3] = len;
  LR2021_readCmd(lr,LR2021_READ_REG_MEM32,txbuf,4,rxbuf,4);
  //LR2021_readCmd(lr,LR2021_READ_REG_MEM32,txbuf,4,rxbuf,len*4);
}


