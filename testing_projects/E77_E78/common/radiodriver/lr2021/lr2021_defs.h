#ifndef _LR2021_DEFS_H_
#define _LR2021_DEFS_H_

//Packet type selection
#define LR2021_PACKET_TYPE_LORA            0
#define LR2021_PACKET_TYPE_FSK_GENERIC     1
#define LR2021_PACKET_TYPE_FSK_LEGACY      2
#define LR2021_PACKET_TYPE_BLE             3
#define LR2021_PACKET_TYPE_RANGING         4
#define LR2021_PACKET_TYPE_FLRC            5
#define LR2021_PACKET_TYPE_BPSK            6
#define LR2021_PACKET_TYPE_LR_FHSS         7
#define LR2021_PACKET_TYPE_WMBUS           8
#define LR2021_PACKET_TYPE_WISUN           9
#define LR2021_PACKET_TYPE_OOK             10
#define LR2021_PACKET_TYPE_RAW             11
#define LR2021_PACKET_TYPE_ZWAVE           12
#define LR2021_PACKET_TYPE_ZIGBEE          13
//Fallback modes
#define LR2021_FALLBACK_STBY_RC     1
#define LR2021_FALLBACK_STBY_XOSC   2
#define LR2021_FALLBACK_STBY_FS     3
//RX path selection
//RxPath
#define LR2021_LF_PATH            0
#define LR2021_HF_PATH            1
//Select which PA to use
//PaSel 
#define LR2021_LFPA               0
#define LR2021_HFPA               1
//PA LF mode (if unused set to 0)
//PaLfMode
#define LR2021_LFPA_FSM           0
#define LR2021_LFPA_FDM           1
#define LR2021_LFPA_HSMRFO1       2
#define LR2021_LFPA_HSMRFO1       3
//PA ramp time
#define LR2021_PA_RAMP_2U         0
#define LR2021_PA_RAMP_4U         1
#define LR2021_PA_RAMP_8U         2
#define LR2021_PA_RAMP_16U        3
#define LR2021_PA_RAMP_32U        4
#define LR2021_PA_RAMP_48U        5
#define LR2021_PA_RAMP_64U        6
#define LR2021_PA_RAMP_80U        7
#define LR2021_PA_RAMP_96U        8
#define LR2021_PA_RAMP_112U       9
#define LR2021_PA_RAMP_128U       10
#define LR2021_PA_RAMP_144U       11
#define LR2021_PA_RAMP_160U       12
#define LR2021_PA_RAMP_176U       13
#define LR2021_PA_RAMP_192U       14
#define LR2021_PA_RAMP_208U       15

#endif
