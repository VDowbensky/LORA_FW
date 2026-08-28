#ifndef _LR2021_BPSK_H_
#define _LR2021_BPSK_H_

//BPSK
#define LR2021_SET_BPSK_MOD_PARAMS    0x0250
#define LR2021_SET_BPSK_PKT_PARAMS    0x0251

//Enable Differential encoding
#define LR2021_DIFFMODE_DISABLED            0
#define LR2021_DIFFMODE_ENABLED             1
}

//BPSK mode selection
#define LR2021_BPSK_MODE_RAW                0
#define LR2021_BPSK_MODE_SIGFOX             1

//Sigfox message type (only valid in Sigfox PHY mode)
#define LR2021_SIGFOX_MSG_APP               0
#define LR2021_SIGFOX_MSG_CTRL              1

//Sigfox frame emission rank (only valid in Sigfox PHY mode)
#define LR2021_SIGFOX_RANK_FIRST            0
#define LR2021_SIGFOX_RANK_SECOND           1
#define LR2021_SIGFOX_RANK_THIRD            2

//Sets the modulation parameters for BPSK packets. FW configures respective modem registers
void LR2021_SetBpskModParamd(uint8_t lr,uint32_t bitrate,uint8_t pulse_shape,uint8_t diff_mode_en,bool diff_mode_init,bool diff_mode_parity);
//Sets the packet parameters for BPSK packets. FW configures respective modem registers
void LR2021_SetBpskPacketParams(uint8_t lr,uint8_t pld_len,uint8_t bpsk_mode,uint8_t sigfox_msg,uint8_t sigfox_rank); 


#endif

