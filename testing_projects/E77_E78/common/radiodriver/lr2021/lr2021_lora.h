#ifndef _LR2021_LORA_H_
#define _LR2021_LORA_H_

//LoRa commands
#define LR2021_SET_LORA_TX_SYNC           0x021d
#define LR2021_SET_LORA_MOD_PARAMS        0x0220
#define LR2021_SET_LORA_PKT_PPARAMS       0x0221
#define LR2021_SET_LORA_SYNCH_TIMEOUT     0x0222
#define LR2021_SET_LORA_SYNC              0x0223
#define LR2021_SET_LORA_SIDE_DET_CONFIG   0x0224
#define LR2021_SET_LORA_SIDE_DET_SYNC     0x0225
#define LR2021_CONFIG_LORA_PRE_MOD        0x0226
#define LR2021_SET_LORA_CAD_PARAMS        0x0227
#define LR2021_SET_LORA_CAD               0x0228
#define LR2021_GET_LORA_RX_STATS          0x0229
#define LR2021_GET_LORA_PKT_STATUS        0x022a
#define LR2021_SET_LORA_ADDR              0x022b
#define LR2021_SET_LORA_SYNC_EXT          0x022d
#define LR2021_SET_LORA_SIDE_DET_SYNC_EXT 0x022e

//Spreading factor
#define LR2021_LORA_SF5                   5
#define LR2021_LORA_SF6                   6
#define LR2021_LORA_SF7                   7
#define LR2021_LORA_SF8                   8
#define LR2021_LORA_SF9                   9
#define LR2021_LORA_SF10                  10
#define LR2021_LORA_SF12                  12

//Bandwidth selection
#define LR2021_LORA_BW7                   0
#define LR2021_LORA_BW15                  1
#define LR2021_LORA_BW31                  2
#define LR2021_LORA_BW62                  3
#define LR2021_LORA_BW125                 4
#define LR2021_LORA_BW250                 5
#define LR2021_LORA_BW500                 6
#define LR2021_LORA_BW1000                7
#define LR2021_LORA_BW10                  8
#define LR2021_LORA_BW20                  9
#define LR2021_LORA_BW41                  10
#define LR2021_LORA_BW83                  11
#define LR2021_LORA_BW100                 12
#define LR2021_LORA_BW200                 13
#define LR2021_LORA_BW400                 14
#define LR2021_LORA_BW800                 15

//Coding rate
#define LR2021_LORA_NOCODING              0
#define LR2021_LORA_CR1HAM45SI            1
#define LR2021_LORA_CR2HAM23SI            2
#define LR2021_LORA_CR3HAM75SI            3
#define LR2021_LORA_CR4HAM12SI            4
#define LR2021_LORA_CR5HAM45LI            5
#define LR2021_LORA_CR6HAM23LI            6
#define LR2021_LORA_CR7HAM12LI            7
#define LR2021_LORA_CR8CC23               8
#define LR2021_LORA_CR9CC12               9

/// Low Data Rate Optimisation. Enable for high Spreading factor to increase tolerance to clock drift.
#define LR2021_LORA_LDRO_OFF              0
#define LR2021_LORA_LDRO_ON               1

//Header type selection
#define LR2021_LORA_HEADER_EXPLICIT       0
#define LR2021_LORA_HEADER_IMPLICIT       1

//Format selection for symbols parameter: either an integer number of symbol or a floating point representation (exponent on 3 MSB bits with mantissa on 5 LSB bits) When Exponent = 0, mantissa is multiplied by 2, and for exponent different from from 0 it is multipplied by 2^(n+2) (TBC)
#define LR2021_LORA_TIMEOUT_INTEGER       0
#define LR2021_LORA_TIMEOUT_FLOAT         1

//Action taken after CAD
#define LR2021_LORA_CAD_EXIT_CADONLY      0
#define LR2021_LORA_CAD_EXIT_CADRX        1
#define LR2021_LORA_CAD_EXIT_CADLBT       16

//TX Sync function
#define LR2021_LORA_TXSYNC_DISABLED       0
#define LR2021_LORA_TXSYNC_MASTER         1
#define LR2021_LORA_TXSYNC_SLAVE          2

typedef struct
{
  uint16_t pktcount;
  uint16_t crcerrors;
  uint16_t headererrors;
  uint16_t predetections;
  uint16_t falsesync;
}LoraRxStats_t;

typedef struct
{
  bool crcstatus;
  uint8_t cr;
  uint8_t length;
  int8_t snrpkt;
  int16_t rssi_pkt;
  int16_t rssi_signal;
  uint8_t detector;
  int32_t freq_offset;
  uint8_t agc_gain;
}LoraPktStatus_t;

//Sets the LoRa modulation parameters. FW configures respective modem registers. Will return CMD_FAIL in the status of the next command, if the packet type is not LoRa
void LR2021_SetLoraModParams(uint8_t lr,uint8_t sf,uint8_t lora_bw,uint8_t lora_cr,uint8_t ldro);
//Sets the packet parameters for the LoRa packets. FW configures according modem registers
void LR2021_SetLoraPktParams(uint8_t lr,uint8_t payload_len,uint8_t header_type,bool crc_en,bool invert_iq);
//Configure LoRa modem to search for a detect for N symbols. N can be given as number, or as mantissa/exponent. SymbolNum 0x00 means no timeout
void LR2021_SetLoRaSynchTimeout(uint8_t lr,uint8_t symbols,uint8_t timeout_format);
//Sets the LoRa syncword. Default value is 0x12. Examples: Public Network: 0x34, Private Network: 0x12
void LR2021_SetLoraSync(uint8_t lr,uint8_t syncword);
//Configure the LoRa RX multi SF (side detectors) config. The multi-sf can listen to multiple SF in parallel and automatically switch to RX mode to demodulate the right SF. Calling SetLoraModulationParams will disable all side detectors
void LR2021_SetLoraSideDetConfig(uint8_t lr);
//Configure the LoRa RX multi SF (side detectors) config. The multi-sf can listen to multiple SF in parallel and automatically switch to RX mode to demodulate the right SF. Calling SetLoraModulationParams will disable all side detectors
void LR2021_SetLoraSideDetConfigAdv(uint8_t lr,uint8_t sd1_sf,uint8_t sd1_ldro,bool sd1_inv,uint8_t sd2_sf,uint8_t sd2_ldro,bool sd2_inv,uint8_t sd3_sf,uint8_t sd3_ldro, bool sd3_inv);
//Configure the LoRa RX multi SF (side detectors) syncwords
void LR2021_SetLoraSideDetSync(uint8_t lr);
//Configure the LoRa RX multi SF (side detectors) syncwords
void LR2021_SetLoraSideDetSyncAdv(uint8_t lr,uint8_t sd1_sw,uint8_t sd2_sw,uint8_t sd3_sw);
//Configures the LoRa phase modulation demodulation during preamble feature. If enabled, for TX the preamble will have a phase modulation overlayed to the LoRa modulation
void LR2021_ConfigLoraPreMod(uint8_t lr,bool pmod_en,uint8_t dram_ret,uint16_t wakeup_time,uint32_t min_sleep_time);
//Configures the LoRa phase modulation demodulation during preamble feature. If enabled, for TX the preamble will have a phase modulation overlayed to the LoRa modulation
void LR2021_ConfigLoRaPreModAdv(uint8_t lr,bool pmod_en,uint8_t dram_ret,uint16_t wakeup_time,uint32_t min_sleep_time,uint8_t err_thr,
                                uint8_t min_sym,uint8_t detect_time_sym,uint8_t start_offset,uint8_t end_offset);
//Configure LoRa CAD mode parameters. Sets up Channel Activity Detection which searches for presence of LoRa preamble symbols
void LR2021_SetLoraCadParams(uint8_t lr,uint8_t nb_symbols,bool pbl_any,uint8_t pnr_delta,uint8_t exit_mode,uint32_t timeout,uint8_t det_peak);
//Set device into RX CAD mode (LoRa). The Channel Activity Detection searches for the presence of LoRa preamble symbols. Parameters must be previously set using SetLoraCadParams
void LR2021_SetLoraCad(uint8_t lr);
//Gets the internal statistics of the received packets. Statistics are reset on a POR, sleep without memory retention and the command ResetRxStats
void LR2021_GetLoraRxStats(uint8_t lr, LoraRxStats_t *stats);
//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone or CadDone irqs)
void LR2021_GetLoraPktStatus(uint8_t lr,LoraPktStatus_t *status);
//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone or CadDone irqs)
void LR2021_GetLoraPktStatusAdv(uint8_t lr,LoraPktStatus_t *status);
//Sets the address for LoRa RX address filtering
void LR2021_SetLoraAddr(uint8_t lr,uint8_t addr_comp_len,uint8_t addr_comp_pos,uint64_t addr);
//Extended version of the SetLoraSyncword command to set all 10bits of the syncword
void LR2021_SetLoraSyncExt(uint8_t lr,uint8_t sync1,uint8_t sync2);
//Extended version of the SetLoraSideDetSyncword command to set all 10bits of the syncwords
void LR2021_SetLoraSideDetSyncExt(uint8_t lr,uint8_t  sd1_sw1,uint8_t sd1_sw2,uint8_t sd2_sw1,uint8_t sd2_sw2,uint8_t sd3_sw1,uint8_t sd3_sw2);
//Configures the LoRa Tx synchronization using dio
void LR2021_SetLoraTxSync(uint8_t lr,uint8_t function,uint8_t dio_num);


#endif

