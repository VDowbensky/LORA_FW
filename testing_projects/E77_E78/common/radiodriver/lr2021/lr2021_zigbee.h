#ifndef _LR2021_ZIGBEE_H_
#define _LR2021_ZIGBEE_H_

//Zigbee commands
#define LR2021_SET_ZIGBEE_PARAMS      0x029f
#define LR2021_GET_ZIGBEE_PKT_STATUS  0x02a1
#define LR2021_SET_ZIGBEE_PKT_LEN     0x02a2
#define LR2021_SET_ZIGBEE_ADDR        0x02a3
//The modulation and data rate to be used for RX and TX
#define LR2021_ZB_MODE_OQPSK250       0
#define LR2021_ZB_MODE_OQPSK100       1
#define LR2021_ZB_MODE_OQPSK25        2
#define LR2021_ZB_MODE_BPSK20         3
#define LR2021_ZB_MODE_BPSK40         4
//Address filtering enable/disable. Addresses are set using the SetZigbeeAddress command
#define LR2021_ZB_ADDR_DISABLE        0
#define LR2021_ZB_ADDR_ENABLE         1
//Set the Rx/Tx mode for FCS (16 bits)
#define LR2021_ZB_FCS_ON              0
#define LR2021_ZB_FCS_IN_FIFO         1

typedef struct
{
  uint8_t rx_header;
  uint16_t pkt_len;
  int16_t rssi_avg;
  int16_t rssi_sync;
  uint8_t lqi;
}ZbPktStatus_t;

//Sets the parameters for Zigbee packets
void LR2021_SetZigbeeParams(uint8_t lr,uint8_t zigbee_mode,uint8_t pld_len,uint16_t pbl_len_tx,uint8_t address_on,uint8_t fcs_mode);
//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone irq), but rssi_sync is already updated on SyncWordValid irq
void LR2021_GetZigbeePktStatus(uint8_t lr,ZbPktStatus_t *status);
//Sets the zigbee packet length without the need to call the full SetZigbeeParams command (slower to run)
void LR2021_SetZigbeePktLen(uint8_t lr,uint8_t pld_len);
//Sets the Zigbee addresses for filtering in Rx. Frames that don't match the addresses raise an address error IRQ. When a packet is received, the destination address and PAN ID are both checked. Broadcast addresses are matched as well. Multi-cast is not supported or filtered
void LR2021_SetZigbeeAddress(uint8_t lr,uint86_t long_dest_addr,uint16_t short_dest_addr,uint16_t pan_id,uint16_t trans_id);

#endif

