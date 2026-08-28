#ifndef _LR2021_COMMON_H_
#define _LR2021_COMMON_H_

//Common commands
//System
#define LR2021_GET_STATUS             0x0100
#define LR2021_GET_VERSION            0x0101
#define LR2021_GET_ERRORS             0x0110
#define LR2021_CLEAR_ERRORS           0x0111
#define LR2021_SET_DIO_FUNC           0x0112
#define LR2021_SET_DIO_RFSWITCH_CFG   0x0113
#define LR2021_CLR_FIFO_IRQ_FLAGS     0x0114
#define LR2021_SET_DIO_IRQ_PARAMS     0x0115
#define LR2021_CLEAR_IRQ              0x0116
#define LR2021_GET_CLEAR_IRQ          0x0117              
#define LR2021_CONFIG_LF_CLOCK        0x0118
#define LR2021_CONFIG_CLK_OUT         0x0119
#define LR2021_CONFIG_FIFO_IRQ        0x011a
#define LR2021_GET_FIFO_IRQ_FLAGS     0x011b
#define LR2021_GET_RX_FIFO_LEVEL      0x011c
#define LR2021_GET_TX_FIFO_LEVEL      0x011d
#define LR2021_CLEAR_RX_FIFO          0x011e
#define LR2021_CLEAR_TX_FIFO          0x011f 
#define LR2021_SET_REG_MODE           0x0121
#define LR2021_CALIBRATE              0x0122
#define LR2021_CALIB_FE               0x0123
#define LR2021_GET_VBAT               0x0124
#define LR2021_GET_TEMP               0x0125
#define LR2021_GET_RND_NUMBER         0x0126
#define LR2021_SET_SLEEP              0x0127
#define LR2021_SET_STANDBY            0x0128          
#define LR2021_SET_FS                 0x0129
#define LR2021_SET_ADD_REG_TO_RETAIN  0x012a
#define LR2021_GET_CLR_FIFO_IRQ_FLAGS 0x012e
#define LR2021_SET_EOL_CONFIG         0x0130
#define LR2021_SET_XOSC_CP_TRIM       0x0131
#define LR2021_SET_TEMP_COMP_CFG      0x0132
#define LR2021_SET_NTC_PARAMS         0x0133
//Regmem
#define LR2021_WRITE_REG_MEM32        0x0104
#define LR2021_WRITE_REG_MEM_MASK32   0x0105
#define LR2021_READ_REG_MEM32         0x0106
//Common
#define LR2021_SET_FREQ               0x0200
#define LR2021_SET_RX_PATH            0x0201
#define LR2021_SET_PA_CONFIG          0x0202
#define LR2021_SET_TX_PARAMS          0x0203
#define LR2021_SET_RXTX_FALLBACKMODE  0x0206
#define LR2021_SET_PACKET_TYPE        0x0207
#define LR2021_GET_PACKET_TYPE        0x0208
#define LR2021_STOP_TIMEOUT           0x0209
#define LR2021_RESET_RX_STATS         0x020a
#define LR2021_GET_RSSI_INST          0x020b
#define LR2021_SET_RX                 0x020c
#define LR2021_SET_TX                 0x020d
#define LR2021_SET_TX_TEST            0x020e
#define LR2021_PA_SEL                 0x020f
#define LR2021_AUTO_TX_RX             0x0211
#define LR2021_GET_RX_PKT_LEN         0x0212
#define LR2021_SET_POWER_OFFSET       0x0214
#define LR2021_SET_DEFAULT_TIMEOUTS   0x0215
#define LR2021_SET_TIMESTAMP_SRC      0x0216
#define LR2021_GET_TIMESTAMP          0x0217
#define LR2021_SET_CCA                0x0218
#define LR2021_GET_CCA_RESULT         0x0219
#define LR2021_SET_AGC_GAIN_MAN       0x021a
#define LR2021_SET_CAD_PARAMS         0x021b
#define LR2021_SET_CAD                0x021c

//Common definitions
//DIO function selection
#define LR2021_DIOFUNC_NONE                 0
#define LR2021_DIOFUNC_IRQ                  1
#define LR2021_DIOFUNC_RFSW                 2
#define LR2021_DIOFUNC_GPOUT_LOW            5
#define LR2021_DIOFUNC_GPOUT_HIGH           6
#define LR2021_DIOFUNC_HFCLKOUT             7
#define LR2021_DIOFUNC_LFCLKOUT             8
#define LR2021_DIOFUNC_TXTRIGGER            9
#define LR2021_DIOFUNC_RXTRRIGGER           10
//Pull-up/down configuration for sleep mode. DIO_PULL_AUTO means if DIO value in Standby was '1', it will be pulled-up, if '0' it will be pulled-down
#define LR2021_PULLDRIVE_NONE               0
#define LR2021_PULLDRIVE_DOWN               1
#define LR2021_PULLDRIVE_UP                 2
#define LR2021_PULLDRIVE_AUTO               3
//LF clock source selection
#define LR2021_LFCLK_RC                     0
#define LR2021_LFCLK_XTAL                   1
#define LR2021_LFCLK_DIO11                  2
//SIMO usage configuration
#define LR2021_SIMO_USAGE_OFF               0
#define LR2021_SIMO_USAGE_ALL               1
#define LR2021_SIMO_USAGE_AUTO              2
#define LR2021_SIMO_USAGE_VDCC              3
//Ramp time RC to RU resolution
//Ramp time TX to RU resolution
//Ramp time RU to RC resolution
//Ramp down time
#define LR2021_RAMPTIME_RES2U               0
#define LR2021_RAMPTIME_RES4U               1
#define LR2021_RAMPTIME_RES8U               2
#define LR2021_RAMPTIME_RES16U              3
//Format of returned value
#define LR2021_VBATTFORMAT_RAW              0
#define LR2021_VBATTFORMAT_MILLIVOLTS       1
//ADC resolution for measurement
#define LR2021_ADCRES_8B                    0
#define LR2021_ADCRES_9B                    1
#define LR2021_ADCRES_10B                   2
#define LR2021_ADCRES_11B                   3
#define LR2021_ADCRES_12B                   4
#define LR2021_ADCRES_13B                   5
//Temperature sensor source
#define LR2021_TEMPSRC_VBE                  0
#define LR2021_TEMPSRC_XOSC                 1
#define LR2021_TEMPSRC_NTC                  2
//Standby mode selection
#define LR2021_STBY_RC                      0
#define LR2021_STBY_XOSC                    1
//Threshold voltage for EOL interrupt (Trim)
#define LR2021_EOL_1P6                      0
#define LR2021_EOL_1P67                     1
#define LR2021_EOL_1P74                     2
#define LR2021_EOL_1P8                      3
#define LR2021_EOL_1P88                     4
#define LR2021_EOL_1P95                     5
#define LR2021_EOL_2                        6
#define LR2021_EOL_2P1                      7
//NTC source enable
#define LR2021_NTC_DISABLED                 0
#define LR2021_NTC_ENABLED                  1
//Temperature compensation mode
#define LR2021_COMPMODE_DISABLED            0
#define LR2021_COMPMODE_RELATIVE            1
#define LR2021_COMPMODE_ABSOLUTE            2
//RX path selection
#define LR2021_RFPATH_LF                    0
#define LR2021_RFPATH_HF                    1
//Select which PA to use
#define LR2021_PASEL_LF                     0
#define LR2021_PASEL_HF                     1
//PA LF mode (if unused set to 0)
#define LR2021_PALFMODE_FSM                 0
#define LR2021_PALFMODE_FDM                 1
#define LR2021_PALFMODE_HSMRFO1             2
#define LR2021_PALFMODE_HSMRFO2             3
//PA ramp time selection
#define LR2021_RAMPTIME_2U                  0
#define LR2021_RAMPTIME_4U                  1
#define LR2021_RAMPTIME_8U                  2
#define LR2021_RAMPTIME_16U                 3
#define LR2021_RAMPTIME_32U                 4
#define LR2021_RAMPTIME_48U                 5
#define LR2021_RAMPTIME_64U                 6
#define LR2021_RAMPTIME_80U                 7
#define LR2021_RAMPTIME_96U                 8
#define LR2021_RAMPTIME_112U                9
#define LR2021_RAMPTIME_128U                10
#define LR2021_RAMPTIME_144U                11
#define LR2021_RAMPTIME_160U                12
#define LR2021_RAMPTIME_176U                13
#define LR2021_RAMPTIME_192U                14
#define LR2021_RAMPTIME_208U                15
//Fallback mode selection
#define LR2021_FALLBACK_STBYRC              1
#define LR2021_FALLBACK_STBYXOSC            2
#define LR2021_FALLBACK_STBYFS              3
//Packet type selection
#define LR2021_PKT_TYPE_LORA                0
#define LR2021_PKT_TYPE_FSK_GENERIC         1
#define LR2021_PKT_TYPE_FSK_LEGACY          2
#define LR2021_PKT_TYPE_BLE                 3
#define LR2021_PKT_TYPE_RANGING             4
#define LR2021_PKT_TYPE_FLRC                5
#define LR2021_PKT_TYPE_BPSK                6
#define LR2021_PKT_TYPE_LR_FHSS             7
#define LR2021_PKT_TYPE_WMBUS               8
#define LR2021_PKT_TYPE_WISUN               9
#define LR2021_PKT_TYPE_OOK                 10
#define LR2021_PKT_TYPE_RAW                 11
#define LR2021_PKT_TYPE_ZWAVE               12
#define LR2021_PKT_TYPE_ZIGBEE              13
//Timeout stop condition
#define LR2021_STOPTIMEOUT_ON_PRE           0
#define LR2021_STOPTIMEOUT_ON_SYNC          1
//Test mode selection
#define LR2021_TESTMODE_NORMAL              0
#define LR2021_TESTMODE_PREAMBLE            1
#define LR2021_TESTMODE_CW                  2
#define LR2021_TESTMODE_PN9                 3
//Auto mode configuration
#define LR2021_AUTOMODE_DISABLE             0
#define LR2021_AUTOMODE_ALWAYS              1
#define LR2021_AUTOMODE_VALID               2
/// Event source selection
#define LR2021_EVENTSRC_NONE                0
#define LR2021_EVENTSRC_TXDONE              1
#define LR2021_EVENTSRC_RXDONE              2
#define LR2021_EVENTSRC_SYNC                3
#define LR2021_EVENTSRC_HEADER              4
//Action taken after the CAD
#define LR2021_CADEXIT_FALLBACK             0
#define LR2021_CADEXIT_TX                   1
#define LR2021_CADEXIT_RX                   2

//Functions

//Returns status of device. If the previous command was not a read command, returns the status register. If previous transaction was a read command, returns the result of the read command. Also clears the reset status returned in the Stat variable
void LR2021_GetStatus(uint8_t lr);
//Returns version of firmware. 
void LR2021_GetVersion(uint8_t lr,uint8_t *ver_major,uint8_t *ver_minor);
//Returns current pending errors that occurred since the last ClearErrors() call, or the startup of the circuit. It is possible to generate an Irq on DIO when an error occurs
void LR2021_GetErrors(uint8_t lr);
//Clears all error flags in the status. The flags cannot be cleared individually. Calling ClearErrors will not clear the Error IRQ - the IRQ has to be cleared explicitly with the ClearIrq command
void LR2021_ClearErrors(uint8_t lr);
//Configure the functionality of the freely configurable DIOs, as well as the pull-up/down configuration for sleep modes. On DIO5, only DIO_SLEEP_PULL_UP is accepted. DIO5/6 have pull-up by default
void LR2021_SetDioFunction(uint8_t dio,uint8_t dio_func,uint8_t pull_drive);
//Configure the value of the specified DIO pin when configured as RF switch with the SetDioFunction command
void LR2021_SetDioAsRfSwitch(uint8_t lr,uint8_t dio,bool tx_hf,bool rx_hf,bool tx_lf,bool rx_lf,bool standby);
//Clears specific fifo irq flags
void LR2021_ClearFifoIrqFlags(uint8_t lr,uint8_t rx_fifo_flags_to_clear,uint8_t tx_fifo_flags_to_clear); 
//Configure IRQs which assert DIO pin
void LR2021_SetDioIrqConfig(uint8_t lr,uint8_t dio,uint32_t irqs);
//Clear pending irqs
void LR2021_ClearIrqFlags(uint8_t lr,uint32_t irqs);
//Read out the pending IRQs and clear all pending IRQs
void LR2021_GetClearIrq(uint8_t lr);
//Configures the used Low Frequency clock
void LR2021_ConfigLfClk(uint8_t lr,uint8_t lfclock);
//Configure LF and HF clock configuration for DIO output
void LR2021_ConfigClkOutput(uint8_t lr,uint8_t hf_clk_out_scaling);
//Configure which FIFO status flags generate FIFO IRQs as well as the threshold levels
void LR2021_ConfigFifoIrq(uint8_t lr,uint8_t rx_fifo_irq_enable,uint8_t tx_fifo_irq_enable,uint16_t rx_high_threshold,uint16_t tx_low_threshold,uint16_t rx_low_threshold,uint16_t tx_high_threshold);
//Returns all FIFO flags that have been triggered since the last flags clear. It does not depend on enabled flags for the IRQs
void LR2021_GetFifoIrqFlags(uint8_t lr,uint8_t *rxflags,uint8_t *txflags)
//Read out the RX FIFO level in bytes
uint16_t LR2021_GetRxFifoLevel(uint8_t lr)
//Read out the TX FIFO level in bytes
uint16_t LR2021_GetTxFifoLevel(uint8_t lr)
//Clears the RX FIFO
void LR2021_ClearRxFifo(uint8_t lr)
//Clears the TX FIFO
void LR2021_ClearTxFifo(uint8_t lr);
//Configures the SIMO mode
void LR2021_SetRegMode(uint8_t lr,uint8_t simo_usage);
//Configures the SIMO mode and ramp times
void LR2021_SetRegModeAdv(uint8_t lr,uint8_t simo_usage,uint8_t ramp_time_rc2ru_unit,uint8_t ramp_time_rc2ru,uint8_t ramp_time_tx2ru_unit,
                          uint8_t ramp_time_tx2ru,uint8_t ramp_time_ru2rc_unit,uint8_t ramp_time_ru2rc,uint8_t ramp_time_ramp_down_unit,uint8_t ramp_time_ramp_down);
//Calibrates the requested blocks defined by the blocks_to_calibrate parameter. Will work in any mode of the chip. On exit the chip will be in Standby RC
void LR2021_Calibrate(uint8_t lr,bool aaf,bool pa_offset,bool meas_unit,bool pll,bool hf_rc,bool lf_rc);
//Launches all Front End calibrations (ADC Offset, PPF, Image) at the given frequencies. Will not work if device is in Rx or Tx mode
void LR2021_CalibrateFE(uint8_t lr,uint16_t freq1,uint16_t freq2,uint16_t freq3);
/// Measure and return current VBAT value
uint16_t LR2021_GetVbat(uint8_t lr,uint8_t vbat_format,uint8_t adc_res);
//Measure and return temperature value
float LR2021_GetTemp(uint8_t lr,uint8_t temp_src,uint8_t adc_res);
//Return a 32-bit random number
uint32_t LR2021_GetRandomNumber(uint8_t lr);
//Return a 32-bit random number
uint32_t LR2021_GetRandomNumberAdv(uint8_t lr,uint8_t source);
//Put device in sleep mode
void LR2021_SetSleep(uint8_t lr);
//Put device in sleep mode
void LR2021_SetSleepAdv(uint8_t lr,bool clk_32k_en,uint8_t ret_en,uint32_t sleep_time);
//Put device in standby mode (XOSC or RC)
void LR2021_SetStandby(uint8_t lr,uint8_t standby_mode);
//Put device in Frequency Synthesis Mode
void LR2021_SetFs(uint8_t lr);
//Specify address of additional register to save for sleep modes with retention
void LR2021_SetAssRegToRetain(uint8_t lr,uint8_t slot,uint32_t addr);
//Read out and clear FIFO flags that generated FIFO IRQs
uint32_t LR2021_GetClrFifoIrqFlags(uint8_t lr);
//Enable/disable end of life detection and setup trigger voltage
void LR2021_SetEolConfig(uint8_t lr,uint8_t trim,bool enable);
//Configure XOSC foot capacitor trim
void LR2021_SetXoscCpTrim(uint8_t lr,uint8_t xta,uint8_t xtb);
//Configure XOSC foot capacitor trim
void LR2021_SetXoscCpTrimAdv(uint8_t lr,uint8_t xta,uint8_t xtb,uint8_t delay_us);
//Configure temperature compensation mode for TX and RX
void LR2021_SetTempCompCfg(uint8_t lr,uint8_t ntc,uint8_t comp_mode);
//Configure NTC parameters
void LR2021_SetNtcParams(uint8_t lr,uint16_t ntc_r_ratio,uint16_t ntc_beta,uint8_t delay);
// Common commands API
//Sets the RF frequency for subsequent radio operations. Will not work with the chip in TX mode. All frequency dependent parameters are automatically recomputed by the FW
void LR2021_SetRfFreq(uint8_t lr,uint32_t rf_freq);
//Sets the RX path and boost configuration. If rx_boost is changed, the SRC calibration (ADC offset) is run again for G12 and G13 with the updated boost configuration
void LR2021_SetRxPath(uint8_t lr,uint8_t rx_path);
//Sets the RX path and boost configuration. If rx_boost is changed, the SRC calibration (ADC offset) is run again for G12 and G13 with the updated boost configuration
void LR2021_SetRxPathAdv(uint8_t lr,uint8_t rx_path,uint8_t rx_boost);
//Chooses which PA to use and sets the parameters of the PA
void LR2021_SetPaConfig(uint8_t lr,uint8_t pa_sel,uint8_t pa_lf_mode,uint8_t pa_lf_duty_cycle,uint8_t pa_lf_slices);
//Chooses which PA to use and sets the parameters of the PA
void LR2021_SetPaConfigAdv(uint8_t lr,uint8_t pa_sel,uint8_t pa_lf_mode,uint8_t pa_lf_duty_cycle,uint8_t pa_lf_slices,uint8_t pa_hf_duty_cycle);
//Sets the TX power and ramp time of the PA. The FW configures the corresponding registers, including OCP/OVP
void LR2021_SetTxParams(uint8_t lr,uint8_t tx_power,uint8_t ramp_time);
//Configures the fallback mode after a RX or TX operation (after transmission/reception or timeout)
void LR2021_SetRxTxFallbackMode(uint8_t lr,uint8_t fallback_mode);
//Sets the current packet type. This is the first command to be sent when configuring the radio for transceiver operation. Will only work in Standby RC, Standby XOSC or FS mode
void LR2021_SetPacketType(uint8_t lr,uint8_t packet_type);
//Returns the current packet type of the radio
uint8_t LR2021_GetPacketType(uint8_t lr);
//Defines if the RX timeout should be stopped on Syncword/Header detection or on Preamble detection
void LR2021_SetStopTimeout(uint8_t lr,uint8_t stop_timeout);
//Reset Rx Statistics
void LR2021_ResetRxStats(uint8_t lr);
//Gets the instantaneous RSSI value during reception of the packet. Returned value corresponds to -rssi/2 [dBm]
int16_t LR2021_GetRssiInst(uint8_t lr);
//Sets the device into RX mode. The RTC is started with the given value. RxTimeout is in 1/32.768kHz steps, allowing a maximum of 512 seconds timeout. If image rejection calibration was not done for current RF frequency, error RXFREQ_NO_CAL_ERR is generated
void LR2021_SetRx(uint8_t lr);
//Sets the device into RX mode. The RTC is started with the given value. RxTimeout is in 1/32.768kHz steps, allowing a maximum of 512 seconds timeout. If image rejection calibration was not done for current RF frequency, error RXFREQ_NO_CAL_ERR is generated
void LR2021_SetRxAdv(uint8_t lr,uint32_t rx_timeout);
//Sets the device into TX mode. The RTC is started with the given value. TxTimeout is in 1/32.768kHz steps, allowing a maximum of 512 seconds timeout
void LR2021_SetTx(uint8_t lr);
//Sets the device into TX mode. The RTC is started with the given value. TxTimeout is in 1/32.768kHz steps, allowing a maximum of 512 seconds timeout
void LR2021_SetTxAdv(uint8_t lr,uint32_t tx_timeout);
//Sets the device into TX test mode
void LR2021_SetTxTestMode(uint8_t lr,uint8_t test_mode);
//Select which PA to use. Configuration must have been provided beforehand using SetPaConfig. Selection cannot be changed in TX mode
void LR2021_SetPa(uint8_t lr,uint8_t pa_sel);
//Activate or deactivate the auto TX/auto RX mode. In auto RX mode, chip automatically goes from TX to RX after TxDone. In auto TX mode, chip automatically goes from RX to TX after RxDone
void LR2021_SetAutoRxTx(uint8_t lr,bool clear,uint8_t auto_txrx_mode,uint32_t timeout,uint32_t delay);
//Get the length of the last received packet
uint16_t LR2021_GetRxPktLen(uint32_t lr);
//Set the global value of the power offset
void LR2021_SetPowerOffset(uint8_t lr, uint8_t power_offset);
//Sets the default RX and TX timeouts to be used for DIO RX/TX triggers, or if the timeout parameters are not sent in the SetRx and SetTx commands
void LR2021_SetDefaultRxTxTimeout(uint8_t lr,uint32_t rx_timeout,uint32_t tx_timeout);
//Sets the source event for time-stamping different radio events. 3 sources can be configured in parallel
void LR2021_SetTimestampSrc(uint8_t lr,uint8_t index,uint8_t source);
//Get the delay in HF clk tick between the event and the SPI NSS falling edge of the request. Will not return a correct value if the event occurred before a sleep period
uint32_t LR2021_GetTimestamp(uint8_t lr);
//Set the radio into RX mode for Clear Channel Assessment measurements. The radio measures the RSSI for the given duration
void LR2021_SetCca(uint8_t lr,uint32_t duration);
//Set the radio into RX mode for Clear Channel Assessment measurements. The radio measures the RSSI for the given duration
void LR2021_SetCcaAdv(uint8_t lr,uint32_t duration,uint8_t gain);
//Get the RSSI statistics for the CCA measurement
void LR2021_GetCcaResult(uont8_t lr,int16_t *rssi_min,int16_t *rssi_max,int16_t *rssi_avg);
//Set the manual gain of the AGC. A value of 0 enables the AGC: automatic gain
void LR2021_SetAgcGainManual(uint8_t lr,uint8_t gain_step);
//Set the CAD parameters for the Channel Activity Detect for packet types other than LoRa. This CAD is based on the measured RSSI
void LR2021_SetCadParams(uint8_t lr,uint32_t cad_timeout,uint8_t threshold,uint8_t exit_mode,uint32_t trx_timeout);
//Set device into RX CAD mode (not LoRa). Parameters must have been previously set using SetCadParams command
void LR2021_SetCad(uint8_t lr);
// Commands with variable length parameters (not implemented):
// - SetRssiCalibration
//Regmem
//Writes a block of 32-bit words in register/memory space starting at a specific address. The address is auto-incremented after each data word so that data is stored in contiguous register/memory locations. Address must be 32-bit aligned and data length must be a multiple of 4. Maximum 32 words
void LR2021_WriteRegMem32(uint8_t lr,uint32_t addr,uint32_t data);
//Read-Modify-Writes data at given register/memory address. Only the masked bits (Mask bits = 1) of a single 32-bit word are modified. Address must be 32-bit aligned
void LR2021_WriteRegMemMask32(uint8_t lr,uint32_t addr,uint32_t mask,uint32_t data);
//Reads a block of 32-bit words in register/memory space starting at a specific address. The address is auto-incremented after each data word so that data is read from contiguous register locations. Address must be 32-bit aligned. Maximum 64 words
uint32_t LR2021_ReadRegMem32(uint8_t lr,uint32_t addr,uint8_t len,uint32_t *dst);




#endif
