#ifndef _UC1601_INTERFACE_H_
#define _UC1601_INTERFACE_H_

#include "bsp.h"
#include "i2c.h"

#define UC1601_CMD_ADDR           0x38
#define UC1601_DATA_ADDR          0x39

//#define UC1601_CMD_ADDR           0x3a
//#define UC1601_DATA_ADDR          0x3b

//#define UC1601_CMD_ADDR           0x3c
//#define UC1601_DATA_ADDR          0x3d

//#define UC1601_CMD_ADDR           0x3e
//#define UC1601_DATA_ADDR          0x3f

void UC1601_interface_init(void);
void UC1601DataWrite(uint8_t ucData);
void UC1601CmdWrite(uint8_t ucCmd);
void UC1601CmdBurst(uint8_t *ucCmd,uint8_t len);
void UC1601DataBurst(uint8_t *ucData,uint8_t len);
//*****************************************************************************
// \brief Send a command to the device.
// \param ucCmd The command to send.
// This function is to send a command to the device.
// \return None.
//*****************************************************************************
void UC1601DoubleCmdWrite(uint8_t ucCmd, uint8_t ucData);

#endif
