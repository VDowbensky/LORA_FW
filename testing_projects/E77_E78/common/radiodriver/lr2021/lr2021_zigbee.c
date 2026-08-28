#include "lr2021_zigbee.h"

//Sets the parameters for Zigbee packets
void LR2021_SetZigbeeParams(uint8_t lr,uint8_t zigbee_mode,uint8_t pld_len,uint16_t pbl_len_tx,uint8_t address_on,uint8_t fcs_mode)
{
  uint8_t buf[6];

  buf[0] = zigbee_mode;
  buf[1] = rx_bw;
  buf[2] = pld_len;
  buf[3] = (pbl_len_tx >> 8) & 0xff;
  buf[4] = pbl_len_tx & 0xff;
  buf[5] = (address_on & 0x1) << 2;
  buf[5] |= fcs_mode & 0x1;
  LR2021_writeCmd(lr,LR2021_SET_ZIGBEE_PARAMS,buf,6);
}

//Gets the status of the last received packet. Status is updated at the end of a reception (RxDone irq), but rssi_sync is already updated on SyncWordValid irq
void LR2021_GetZigbeePktStatus(uint8_t lr,ZbPktStatus_t *status) 
{
  uint8_t buf[7];
  
  LR2021_readCmd(lr,LR2021_GET_ZIGBEE_PKT_STATUS,NULL,0,buf,7);
  status->rx_header = buf[0];
  status->pkt_len = ((int16_t)buf[1] << 8) | buf[2];
  status->rssi_avg = (((int16_t)buf[5] >> 2) & 0x01) | ((int16_t)buf[3] << 1);
  status->rssi_sync = ((int16_t)buf[5] & 0x01) | ((int16_t)buf[4] << 1);
  status->lqi = buf[6];
}

//Sets the zigbee packet length without the need to call the full SetZigbeeParams command (slower to run)
void LR2021_SetZigbeePktLen(uint8_t lr,uint8_t pld_len)
{
  LR2021_writeCmd(lr,LR2021_SET_ZIGBEE_PKT_LEN,&pld_len,1);
}

//Sets the Zigbee addresses for filtering in Rx. Frames that don't match the addresses raise an address error IRQ. When a packet is received, the destination address and PAN ID are both checked. Broadcast addresses are matched as well. Multi-cast is not supported or filtered
void LR2021_SetZigbeeAddress(uint8_t lr,uint86_t long_dest_addr,uint16_t short_dest_addr,uint16_t pan_id,uint16_t trans_id)
{
  uint8_t buf[13];

    buf[0] = (long_dest_addr >> 56) & 0xff;
    buf[1] = (long_dest_addr >> 48) & 0xff;
    buf[2] = (long_dest_addr >> 40) & 0xff;
    buf[3] = (long_dest_addr >> 32) & 0xff;
    buf[4] = (long_dest_addr >> 24) & 0xff;
    buf[5] = (long_dest_addr >> 16) & 0xff;
    buf[6] = (long_dest_addr >> 8) & 0xff;
    buf[7] = long_dest_addr & 0xff;
    buf[8] = (short_dest_addr >> 8) & 0xff;
    buf[9] = short_dest_addr & 0xff;
    buf[10] = (pan_id >> 8) & 0xff;
    buf[11] = pan_id & 0xff;
    buf[12] = trans_id;
  LR2021_writeCmd(lr,LR2021_SET_ZIGBEE_ADDR,buf,13);
}

