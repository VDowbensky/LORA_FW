#ifndef _LR2021_WISUN_H_
#define _LR2021_WISUN_H_

//Wi-Sun commands
#define LR2021_SET_WISUN_MODE         0x0270
#define LR2021_SET_WISUN_PKT_PARAMS   0x0271
#define LR2021_GET_WISUN_PKT_STATUS   0x0273
#define LR2021_SET_WISUN_PKT_LEN      0x0274
//WISun mode selection
#define LR2021_WISUN_MODE1A           0
#define LR2021_WISUN_MODE1B           1
#define LR2021_WISUN_MODE2A           2
#define LR2021_WISUN_MODE2B           3
#define LR2021_WISUN_MODE3            4
#define LR2021_WISUN_MODE4A           5
#define LR2021_WISUN_MODE4B           6
#define LR2021_WISUN_MODE5            7
//FCS selection for TX
#define LR2021_WISUN_FCS4BYTE         0
#define LR2021_WISUN_FCS2BYTE         1
//Whitening enable
#define LR2021_WISUN_NOWHITE          0
#define LR2021_WISUN_WHITE            1
//CRC calculation mode
#define LR2021_WISUN_CRC_PROVIDED     0
#define LR2021_WISUN_CRC_AUTO         1
//Payload type selection for TX
#define LR2021_WISUN_NORMAL_PAYLOAD   0
#define LR2021_WISUN_MODE_SW_PAYLOAD  1
//FEC encoding selection for TX packet
#define LR2021_WISUN_FEC_NONE         0
#define LR2021_WISUN_FEC_NRNSC        1
#define LR2021_WISUN_FEC_RSC          2
#define LR2021_WISUN_FEC_RSCINTLVR    3

typedef struct
{
  uint16_t header;// Raw 16bit received header
  uint16_t pktlen;
  int16_t rssi_avg;
  int16_t rssi_sync;
  bool syncword_idx;// Index of detected syncword (0/1)
  uint8_t lqi;// Link quality indicator (0.25dB)
}WisunPktStatus_t;

//Configure the wisun mode (1a, 1b, 2a, 2b, 3, 4a, 4b, 5)
void LR2021_SetWisunMode(uint8_t lr,uint8_t wisun_mode,uint8_t rx_bw);
//Configure the wisun packet parameters
void LR2021_SetWisunPktParams(uint8_t lr,uint8_t whitening,uint8_t crc_on,uint8_t mode_switch_tx,uint8_t fec_tx,uint16_t frame_len_tx,uint8_t pbl_len_tx)
//Configure the wisun packet parameters
void LR2021_SetWisunPktParamsAdv(uint8_t lr,uint8_t whitening,uint8_t crc_on,uint8_t mode_switch_tx,uint8_t fec_tx,uint16_t frame_len_tx,uint8_t pbl_len_tx,uint8_t pbl_detect);
//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone irq), but syncword_idx and rssi_sync are already updated on SyncWordValid irq
void LR2021_GetWisunPktStatus(uint8_t lr,WisunPktStatus_t *status);
//Sets length of frame for TX for normal packets, or header value for mode_switch packets
void LR2021_SetWisunPktLen(uint8_t lr,uint16_t frame_len_tx);

#endif