#ifndef _LR2021_FSK_H_
#define _LR2021_FSK_H_

//FSK commands
#define LR2021_SET_FSK_MOD_PARAMS     0x0240
#define LR2021_SET_FSK_PKT_PARAMS     0x0241
#define LR2021_SET_FSK_WHITE_PARAMS   0x0242
#define LR2021_SET_FSK_CRC_PARAMS     0x0243
#define LR2021_SET_FSK_SYNC           0x0244
#define LR2021_SET_FSK_ADDR           0x0245
#define LR2021_GET_FSK_RX_STATS       0x0246
#define LR2021_GET_FSK_PKT_STATUS     0x0247

//Pulse shaping filter selection
#define LR2021_FSK_SHAPE_NONE              0
#define LR2021_FSK_SHAPE_CUSTOM             1
#define LR2021_FSK_SHAPE_BT0P3              4
#define LR2021_FSK_SHAPE_BT0P5              5
#define LR2021_FSK_SHAPE_BT0P7              6
#define LR2021_FSK_SHAPE_BT1P0              7
#define LR2021_FSK_SHAPE_BT2P0              2
#define LR2021_FSK_SHAPE_RC0P3              8
#define LR2021_FSK_SHAPE_RC0P5              9
#define LR2021_FSK_SHAPE_RC0P7              10
#define LR2021_FSK_SHAPE_RC1P0              11
#define LR2021_FSK_SHAPE_RRC0P3             12
#define LR2021_FSK_SHAPE_RRC0P4             3
#define LR2021_FSK_SHAPE_RRC0P5             13
#define LR2021_FSK_SHAPE_RRC0P7             14
#define LR2021_FSK_SHAPE_RRC1P0             15

//RX bandwidth (same format as in the SetAdvancedModulationParams command)
#define LR2021_FSK_RXBW_Auto                255
#define LR2021_FSK_RXBW_3076                0
#define LR2021_FSK_RXBW_2857                64
#define LR2021_FSK_RXBW_2666                128
#define LR2021_FSK_RXBW_2222                192
#define LR2021_FSK_RXBW_1333                136
#define LR2021_FSK_RXBW_1111                200
#define LR2021_FSK_RXBW_888                 144
#define LR2021_FSK_RXBW_769                 24
#define LR2021_FSK_RXBW_740                 208
#define LR2021_FSK_RXBW_714                 88
#define LR2021_FSK_RXBW_666                 152
#define LR2021_FSK_RXBW_615                 32
#define LR2021_FSK_RXBW_571                 96
#define LR2021_FSK_RXBW_555                 216
#define LR2021_FSK_RXBW_533                 160
#define LR2021_FSK_RXBW_512                 17
#define LR2021_FSK_RXBW_476                 81
#define LR2021_FSK_RXBW_444                 224
#define LR2021_FSK_RXBW_384                 25
#define LR2021_FSK_RXBW_370                 209
#define LR2021_FSK_RXBW_357                 89
#define LR2021_FSK_RXBW_333                 153
#define LR2021_FSK_RXBW_307                 33
#define LR2021_FSK_RXBW_285                 97
#define LR2021_FSK_RXBW_277                 217
#define LR2021_FSK_RXBW_266                 161
#define LR2021_FSK_RXBW_256                 18
#define LR2021_FSK_RXBW_238                 82
#define LR2021_FSK_RXBW_222                 225
#define LR2021_FSK_RXBW_192                 26
#define LR2021_FSK_RXBW_185                 210
#define LR2021_FSK_RXBW_178                 90
#define LR2021_FSK_RXBW_166                 154
#define LR2021_FSK_RXBW_153                 34
#define LR2021_FSK_RXBW_142                 98
#define LR2021_FSK_RXBW_138                 218
#define LR2021_FSK_RXBW_133                 162
#define LR2021_FSK_RXBW_128                 19
#define LR2021_FSK_RXBW_119                 83
#define LR2021_FSK_RXBW_111                 226
#define LR2021_FSK_RXBW_96                  27
#define LR2021_FSK_RXBW_92                  211
#define LR2021_FSK_RXBW_89                  91
#define LR2021_FSK_RXBW_83                  155
#define LR2021_FSK_RXBW_76                  35
#define LR2021_FSK_RXBW_71                  99
#define LR2021_FSK_RXBW_69                  219
#define LR2021_FSK_RXBW_66                  163
#define LR2021_FSK_RXBW_64                  20
#define LR2021_FSK_RXBW_59                  84
#define LR2021_FSK_RXBW_55                  227
#define LR2021_FSK_RXBW_48                  28
#define LR2021_FSK_RXBW_46                  212
#define LR2021_FSK_RXBW_44                  92
#define LR2021_FSK_RXBW_41                  156
#define LR2021_FSK_RXBW_38                  36
#define LR2021_FSK_RXBW_35                  100
#define LR2021_FSK_RXBW_34                  220
#define LR2021_FSK_RXBW_33                  164
#define LR2021_FSK_RXBW_32                  21
#define LR2021_FSK_RXBW_29                  85
#define LR2021_FSK_RXBW_27                  228
#define LR2021_FSK_RXBW_24                  29
#define LR2021_FSK_RXBW_23                  213
#define LR2021_FSK_RXBW_22                  93
#define LR2021_FSK_RXBW_20                  157
#define LR2021_FSK_RXBW_19                  37
#define LR2021_FSK_RXBW_17                  101
#define LR2021_FSK_RXBW_16                  165
#define LR2021_FSK_RXBW_14                  86
#define LR2021_FSK_RXBW_13                  229
#define LR2021_FSK_RXBW_12                  30
#define LR2021_FSK_RXBW_11                  94
#define LR2021_FSK_RXBW_10                  158
#define LR2021_FSK_RXBW_9P6                 38
#define LR2021_FSK_RXBW_8P9                 102
#define LR2021_FSK_RXBW_8P7                 222
#define LR2021_FSK_RXBW_8P3                 166
#define LR2021_FSK_RXBW_8                   23
#define LR2021_FSK_RXBW_7P4                 87
#define LR2021_FSK_RXBW_6P9                 230
#define LR2021_FSK_RXBW_6                   31
#define LR2021_FSK_RXBW_5P8                 215
#define LR2021_FSK_RXBW_5P6                 95
#define LR2021_FSK_RXBW_5P2                 159
#define LR2021_FSK_RXBW_4P8                 39
#define LR2021_FSK_RXBW_4P5                 103
#define LR2021_FSK_RXBW_4P3                 223
#define LR2021_FSK_RXBW_4P2                 167
#define LR2021_FSK_RXBW_3P5                 231

//Preamble detection length. 0=off (detection on syncword), others=length of preamble detection. Enables/disables PreambleDetected IRQ
#define LR2021_FSK_PREDET_NONE              0
#define LR2021_FSK_PREDET_8BITS             8
#define LR2021_FSK_PREDET_16BITS            16
#define LR2021_FSK_PREDET_24BITS            24
#define LR2021_FSK_PREDET_32BITS            32

//Payload length unit
#define LR2021_FSK_PL_UNIT_BYTES            0
#define LR2021_FSK_PL_UNIT_BITS             1

//Address filtering mode. If address comparison fails, packet reception is aborted and addrErr flag is set
#define LR2021_FSK_ADDRCOMP_OFF             0
#define LR2021_FSK_ADDRCOMP_NODE            1
#define LR2021_FSK_ADDRCOMP_NoODEBCAST      2

//Packet format selection (fixed or variable length)
#define LR2021_FSK_FIXEDLEN                 0
#define LR2021_FSK_VARLEN8BIT               1
#define LR2021_FSK_VARLEN9BIT               2
#define LR2021_FSK_VARLEN16BIT              3

//CRC mode selection
#define LR2021_FSK_CRC_OFF                  0
#define LR2021_FSK_CRC_1B                   1
#define LR2021_FSK_CRC_2B                   2
#define LR2021_FSK_CRC_3B                   3
#define LR2021_FSK_CRC_4B                   4
#define LR2021_FSK_CRC_1BBINV               9
#define LR2021_FSK_CRC_2BINV                10
#define LR2021_FSK_CRC_3BINV                11
#define LR2021_FSK_CRC_4BINV                12

//Whitening type compatibility
#define LR2021_FSK_WH_SX126X_LR11XX         0
#define LR2021_FSK_WH_SX128X                1

//Bit order for syncword transmission (over the air). Set MSB first for SX126x, LR11xx, SX1280 compatible value
#define LR2021_FSK_SYNCBITORDER_LSBFIRST    0
#define LR2021_FSK_SYNCBITORDER_MSBFIRST    1

typedef struct
{
  uint16_t pktcount;
  uint16_t crcerrors;
  uint16_t lenerrors;
  uint16_t detections;
  uint16_t syncok;
  uint16_t syncfail;
  uint16_t rtctimeouts;
}FskRxStats_t;

typedef struct
{
  uint16_t length;
  int16_t rssi_avg;
  int16_t rssi_sync;
  bool bcast_match;// Indicates if the last packet received matched the broadcast address
  bool node_match;// Indicates if the last packet received matched the node address
  uint8_t lqi;
}FskPktStatus_t;

//Sets the modulation parameters for FSK packets. FW configures respective modem registers. Will return CMD_FAIL in the status of the next command, if the packet type is not FSK
void LR2021_SetFskModParams(uint8_t lr,uint32_t bitrate,uint8_t pulse_shape,uint8_t rx_bw,uint32_t fdev);
//Sets the packet parameters for FSK packets.. Command will fail if packet type is not FSK
void LR2021_SetFskPktParams(uint8_t lr,uint16_t pbl_len_tx,uint8_t pbl_len_detect,bool pbl_long,uint8_t pld_len_unit,uint8_t addr_comp,
                            uint8_t fsk_pkt_format,uint16_t pld_len,uint8_t crc,uint8_t dc_free);
//Configure the whitening params for FSK packets, SX126x/LR11xx or SX128x compatible
void LR2021_SetFskWhiteParams(uint8_t lr,uint8_t whiten_type,uint16_t init);
//Configure the CRC params for FSK packets
void LR2021_SetFskCrcParams(uint8_t lr,uint32_t polynom,uint32_t init);
//Configure the syncword for FSK packets
void LR2021_SetFskSync(uint8_t lr,uint64_t syncword,uint8_t bit_order,uint8_t nb_bits);
//Configure the addresses for filtering for FSK packets
void LR2021_SetFskAddr(uint8_t lr,uint8_t addr_node,uint8_t addr_bcast) 
{
  uint8_t buf[2];

  buf[0] = addr_node;
  buf[1] = addr_bcast;
  LR2021_writeCmd(lr,LR2021_SET_FSK_ADDR,buf,2);
}

//Get FSK RX stats
void LR2021_GetFskRxStats(uint8_t lr,FskRxStats_t *stats);
//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone irq), but rssi_sync is already updated on SyncWordValid irq
void LR2021_GetFskPktStatus(uint8_t lr,FskPktStatus_t *status);  

#endif
