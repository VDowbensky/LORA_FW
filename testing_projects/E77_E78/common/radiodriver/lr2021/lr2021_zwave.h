#ifndef _LR2021_ZWAVE_H_
#define _LR2021_ZWAVE_H_

//Zwave commands
#define LR2021_SET_ZW_PARAMS          0x0297
#define LR2021_SET_ZW_HOME_ID         0x0298
#define LR2021_SET_ZW_HOME_ID_FILTER  0x0298
#define LR2021_GET_ZW_PKT_STATUS      0x029a
#define LR2021_SET_ZW_BEAM_FILTER     0x029b
#define LR2021_SET_ZW_SCAN_CONFIG     0x029c
#define LR2021_SET_ZW_SCAN            0x029d
//The data rate to be used for the RX and the TX
#define LR2021_ZW_MODE_LR1            0
#define LR2021_ZW_MODE_R1             1
#define LR2021_ZW_MODE_R2             2
#define LR2021_ZW_MODE_R3             3
//Enable or disable the filtering of the HomeID
#define LR2021_ZW_COMP_OFF            0
#define LR2021_ZW_COMP_HOME_ID        1
#define LR2021_ZW_COMP_HOME_ID_BEAM   2
//FCS mode: auto to automatically generate FCS in TX and remove it in RX. In fifo mode, the FCS is expected to be part of the payload ans is not checked in RX.
#define LR2021_ZW_FCSMODE_AUTO        0
#define LR2021_ZW_FCSMODE_FIFO        1
//Address length selection
#define LR2021_ZW_ADDR_8BIT           0
#define LR2021_ZW_ADDR_12BIT          1
}

// Response structs
typedef struct
{
  uint16_t pkt_len;
  int16_t rssi_avg;
  int16_t rssi_sync;
  uint8_t last_detect;
  uint8_t lqi;
}ZwPacketStatus_t;

//Sets the parameters for Z-Wave packets
void LR2021_SetZwaveParams(uint8_t lr,uint8_t zwave_mode,uint8_t rx_bw,uint8_t pld_len,uint16_t pbl_len_tx,uint8_t pbl_len_detect,uint8_t fcs_mode);
//Sets the HomeID address to use as a filter in Rx. The MSB is the first bit sent/received. Frames that don't match the home_id raise an address error IRQ
void LR2021_SetZwaveHomeId(uint8_t lr,uint32_t home_id);
//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone irq), but rssi_sync is already updated on SyncWordValid irq
void LR2021_GetZwavePktStatus(uint8_t lr,ZwPacketStatus_t *status);
//Settings for filtering incoming beam frames in Rx
void LR2021_SetZwaveBeamFiltering(uint8_t lr,uint8_t addr_len,uint16_t node_id,uint8_t id_hash);
//Configure the Z-Wave scan mode. The user can configure up to 4 data rates and channels to be scanned. The rf_freq_ch and timeout parameters only need to be sent for num_ch channels
void LR2021_SetZwaveScanConfig(uint8_t lr,uint8_t num_ch,bool det4,bool det3,bool det2,bool det1,uint8_t bitrate_ch4,uint8_t bitrate_ch3,uint8_t bitrate_ch2,
                              uint8_t bitrate_ch1,uint8_t zwave_addr_comp,uint8_t fcs_mode,uint32_t rf_freq_ch1,uint8_t timeout_ch1,uint32_t rf_freq_ch2,uint8_t timeout_ch2);
//Configure the Z-Wave scan mode. The user can configure up to 4 data rates and channels to be scanned. The rf_freq_ch and timeout parameters only need to be sent for num_ch channels
void LR2021_SetZwaveScanConfigAdv(uint8_t lr,uint8_t num_ch,bool det4,bool det3,bool det2,bool det1,uint8_t bitrate_ch4,uint8_t bitrate_ch3,uint8_t bitrate_ch2,
                              uint8_t bitrate_ch1,uint8_t zwave_addr_comp,uint8_t fcs_mode,uint32_t rf_freq_ch1,uint8_t timeout_ch1,
                              uint32_t rf_freq_ch2,uint8_t timeout_ch2,uint32_t rf_freq_ch3,uint8_t timeout_ch3,uint32_t rf_freq_ch4,uint8_t timeout_ch4);
//This command is used to enter the Zwave RX scan mode. The scan mode will alternate between data rates and channels and attempt to detect an incoming packet
void LR2021_SetZwaveScan(uint8_t lr);

#endif
