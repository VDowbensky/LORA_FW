#ifndef _LR2021_FLRC_H_
#define _LR2021_FLRC_H_

//FLRC commands
#define LR2021_SET_FLRC_MOD_PARAMS    0x0248
#define LR2021_SET_FLRC_PKT_PARAMS    0x0249
#define LR2021_GET_FLRC_RX_STATS      0x024a
#define LR2021_GET_FLRC_PKT_STATUS    0x024b
#define LR2021_SET_FLRC_SYNC          0x024c

//Bitrate and bandwidth combination
#define LR2021_FLRC_BR2600                0
#define LR2021_FLRC_BR2080                1
#define LR2021_FLRC_BR1300                2
#define LR2021_FLRC_BR1040                3
#define LR2021_FLRC_BR0650                4
#define LR2021_FLRC_BR0520                5
#define LR2021_FLRC_BR0325                6
#define LR2021_FLRC_BR0260                7

//Coding rate selection
#define LR2021_FLRC_CR12                  0
#define LR2021_FLRC_CR34                  1
#define LR2021_FLRC_NONE                  2
#define LR2021_FLRC_CR23                  3

//AGC preamble length
#define LR2021_FLRC_LEN4BITS              0
#define LR2021_FLRC_LEN8BITS              1
#define LR2021_FLRC_LEN12BITS             2
#define LR2021_FLRC_LEN16BITS             3
#define LR2021_FLRC_LEN20BITS             4
#define LR2021_FLRC_LEN24BITS             5
#define LR2021_FLRC_LEN28BITS             6
#define LR2021_FLRC_LEN32BITS             7

//Length of syncword (unit is 2 bytes: 0/16/32 bits). Must be 0 if sync_match is OFF
#define LR2021_FLRC_SYNC0BITS             0
#define LR2021_FLRC_SYNC16BITS            1
#define LR2021_FLRC_SYNC32BITS            2

//Defines which syncword to use for TX operations
#define LR2021_FLRC_NOSYNC                0
#define LR2021_FLRC_USESYNC1              1
#define LR2021_FLRC_USESYNC2              2
#define LR2021_FLRC_USESYNC3              3

//Match syncword(s) configuration
#define LR2021_FLRC_MATCHNONE             0
#define LR2021_FLRC_MATCH1                1
#define LR2021_FLRC_MATCH2                2
#define LR2021_FLRC_MATCH12               3
#define LR2021_FLRC_MATCH3                4
#define LR2021_FLRC_MATCH13               5
#define LR2021_FLRC_MATCH23               6
#define LR2021_FLRC_MATCH123              7

//Packet format selection
#define LR2021_FLRC_DYNAMIC               0
#define LR2021_FLRC_FIXED                 1

//CRC configuration
#define LR2021_FLRC_CRCOFF                0
#define LR2021_FLRC_CRC16                 1
#define LR2021_FLRC_CRC24                 2
#define LR2021_FLRC_CRC32                 3

typedef struct
{
  uint16_t pktcount;
  uint16_t crcerrors;
  uint16_t lenerrors;
  uint16_t crcok;
  uint16_t falsesync;
}FlrcRxStats_t;

typedef struct
{
  uint16_t pkt_len;// Length of the last received packet in bytes
  int16_t rssi_avg;
  int16_t rssi_sync;
  uint8_t sw_num;
}FlrcPktStatus_t;

//Sets the modulation parameters for FLRC packets. FW configures respective modem registers. Will return CMD_FAIL in the status of the next command, if the packet type is not FLRC
void LR2021_SetFlrcModParams(uint8_t lr,uint8_t flrc_bitrate,uint8_t flrc_cr,uint8_t pulse_shape);
//Sets the packet parameters for FLRC packets. FW configures respective modem registers
void LR2021_SetFlrcPktParams(uint8_t lr,uint8_t agc_pbl_len,uint8_t sync_len,uint8_t sync_tx,uint8_t sync_match,uint8_t pkt_format,uint8_t crc,uint16_t pld_len);
//Gets the internal statistics of the received packets. Statistics are reset on a POR, sleep without memory retention and the command ResetRxStats
void LR2021_GetFlrcRxStats(uint8_t lr,FlrcRxStats_t *stats);
//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone irq), but rssi_sync is already updated on SyncWordValid irq
void LR2021_GetFlrcPktStatus(uint8_t lr, FlrcPktStatus_t *status);
//Sets the syncword for FLRC packets. 16 or 32 bits can be sent for the syncword. Bytes 5 and 6 are optional if only 16bit syncword is required
void LR2021_SetFlrcSync(uint8_t lr,uint8_t sw_num,uint8_t syncword);

#endif
