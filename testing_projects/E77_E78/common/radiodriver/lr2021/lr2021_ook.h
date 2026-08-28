#ifndef _LR2021_OOK_H_
#define _LR2021_OOK_H_

//OOK commands
#define LR2021_SET_OOK_MOD_PARAMS     0x0281
#define LR2021_SET_OOK_PKT_PARAMS     0x0282
#define LR2021_SET_OOK_CRC_PARAMS     0x0283
#define LR2021_SET_OOK_SYNC           0x0284
#define LR2021_SET_OOK_ADDR           0x0285
#define LR2021_GET_OOK_RX_STATS       0x0286
#define LR2021_GET_OOK_PKT_STATUS     0x0287
#define LR2021_SET_OOK_DET            0x0288
#define LR2021_SET_OOK_WHITE_PARAMS   0x0289
//Magnitude depth
#define LR2021_OOK_DEPTH_FULL         0
#define LR2021_OOK_DEPTH_UP_TO20DB    1
/// Address comparison mode (same as for FSK)
#define LR2021_OOK_ADDRCOMP_OFF         0
#define LR2021_OOK_ADDRCOMP_NODE        1
#define LR2021_OOK_ADDRCOMP_NODE_BCAST  2
/// Packet format
#define LR2021_OOK_PKRLEN_FIXED         0
#define LR2021_OOK_PKRLEN_VAR8BIT       1
//CRC configuration (same as FSK)
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#define LR2021_OOK_CRC_OFF                  0
#define LR2021_OOK_CRC_1B                   1
#define LR2021_OOK_CRC_2B                   2
#define LR2021_OOK_CRC_3B                   3
#define LR2021_OOK_CRC_4B                   4
#define LR2021_OOK_CRC_1BBINV               9
#define LR2021_OOK_CRC_2BINV                10
#define LR2021_OOK_CRC_3BINV                11
#define LR2021_OOK_CRC_4BINV                12
//Encoding configuration
#define LR2021_OOK_NONE                     0
#define LR2021_OOK_MANCHESTER               1
#define LR2021_OOK_MANCHESTER_INV           9
#define LR2021_OOK_BIPHASE_MARK             2
#define LR2021_OOK_BIPHASE_MARK_INV         10
//Bit order for syncword transmission
#define LR2021_OOK_LSBFIRST                 0
#define LR2021_OOK_MSBFIRST                 1
//Start of frame delimiter kind. Set to 0 for ADS-B, RTS and INOVA
#define LR2021_OOK_SOF_FAFFING_EDGE         0
#define LR2021_OOK_SOF_RISING_EDGE          1

typedef struct
{
  uint16_t pktcount;
  uint16_t crcerrors;
  uint16_t lenerrors;
  uint16_t detections;
  uint16_t syncok;
  uint16_t syncfail;
  uint16_t rtctimeouts;
}OokRxStats_t;

typedef struct
{
  uint16_t length;
  int16_t rssi_avg;
  int16_t rssi_high;
  bool bcast_match;// Indicates if the last packet received matched the broadcast address
  bool node_match;// Indicates if the last packet received matched the node address
  uint8_t lqi;
}OokPktStatus_t;

//Sets the OOK modulation parameters
void LR2021_SetOokModParams(uint8_t lr,uint32_t bitrate,uint8_t pulse_shape,uint8_t rx_bw);
//Sets the OOK modulation parameters
void LR2021_SetOokModParamsAdv(uint8_t lr,uint32_t bitrate,uint8_t pulse_shape,uint8_t rx_bw,uint8_t depth);
//Sets the OOK packet parameters. It is recommended to have either whitening or manchester encoding enabled for OOK
void LR2021_SetOokPktParams(uint8_t lr,uint16_t pre_len_tx,uint8_t addr_comp,uint8_t pkt_format,uint16_t pld_len,uint8_t crc,uint8_t encoding);
//Sets the OOK CRC parameters (same as for FSK)
void LR2021_SetOokCrcParams(uint8_t lr,uint32_t polynom,uint32_t init);
//Sets the OOK syncword. Limited to 32bits max
void LR2021_SetOokSync(uint8_t lr,uint32_t syncword,uint8_t bit_order,uint8_t nb_bits);
//Sets the OOK address (same as for FSK)
void LR112X_SetOokAddr(uint8_t lr,uint8_t addr_node,uint8_t addr_bcast);
//Gets the internal statistics of the received packets. Statistics are reset on a POR, sleep without memory retention and the command ResetRxStats
void LR112X_GetOokRxStats(uint8_t lr,OokRxStats_t *stats);
//Get OOK RX stats
void LR112X_GetOokRxStatsAdv(uint8_t lr,OokRxStats_t *stats);
//Gets the status of the last received OOK packet. Status is updated at the end of a reception (RxDone irq), but rssi_sync is already updated on SyncWordValid irq
void LR112X_GetOokPktStatus(uint8_t lr,OokPktStatus_t *status);
//Configures the OOK detection. This API is for RX only. Detection pattern for TX has directly to be put into the TX FIFO
void LR112X_SetOokDet(uint8_t lr,uint16_t preamble_pattern,uint8_t  pattern_length,uint8_t pattern_num_repeats,bool sw_is_raw,uint8_t sfd_kind,uint8_t sfd_length);
//Configure the whitening for OOK packets. Setting a polynomial of 0 will disable the whitening. It is recommended to have either whitening or manchester encoding enabled for OOK
void LR112X_SetOokWhiteParams(uint8_t lr,uint8_t bit_idx,uint16_t polynom,uint16_t init);

#endif

