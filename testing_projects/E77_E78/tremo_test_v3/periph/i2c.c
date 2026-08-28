#include "i2c.h"


void i2c_master_start(i2c_t* i2cx, uint8_t slave_addr, uint8_t bit_rw);
int32_t timeout;

int32_t i2c0_init(void)
{
	i2c_config_t config;
	
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_I2C0, true);
	gpio_set_iomux(SCL0_PORT, SCL0_PIN, 3);
  gpio_set_iomux(SDA0_PORT, SDA0_PIN, 3);
	i2c_config_init(&config);
  i2c_init(I2C0, &config);
  i2c_cmd(I2C0, true);
	
	return 0;
}

void i2c_master_start(i2c_t* i2cx, uint8_t slave_addr, uint8_t bit_rw)
{
    uint8_t data = (slave_addr) | bit_rw;

    i2cx->CR &= ~(I2C_CR_MASTER_ABORT_MASK);
    if (IS_I2C_FIFO_MODE(i2cx->CR)) {
        i2cx->CR |= I2C_CR_TRANS_BEGIN_MASK;

        i2cx->WFIFO = data | I2C_WFIFO_CONTROL_START_MASK | I2C_WFIFO_CONTROL_TB_MASK;
    } else {
        i2cx->DBR = data;
        i2cx->CR &= ~(I2C_CR_STOP_MASK);
        i2cx->CR |= I2C_CR_START_MASK | I2C_CR_TRANS_BYTE_MASK;
    }
}

int32_t i2c0_write(uint8_t addr, uint8_t *data, uint16_t len)
{
	int32_t retval = 0;
	
	i2c_master_send_start(I2C0, addr, I2C_WRITE);
  i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
	//while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET);
	retval = i2c0_waitforflag(I2C_FLAG_TRANS_EMPTY);
	if(retval != 0) goto stop;
	// send data
	for(uint32_t i=0; i < len; i++) 
	{
		i2c_send_data(I2C0, data[i]);
		i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
		//while(i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET);
		retval = i2c0_waitforflag(I2C_FLAG_TRANS_EMPTY);
		if(retval != 0) goto stop;
	}
stop:
	// stop
	i2c_master_send_stop(I2C0);
	//I2C0->CR |= I2C_CR_STOP_MASK;
	return retval;
}

int32_t i2c0_read(uint8_t addr, uint8_t *data, uint16_t len)
{
	uint32_t i;
	uint32_t retval = 0;
	// start
	//i2c_master_send_start(I2C0, addr, I2C_WRITE);
	//i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
	//while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET);
	// write data
	//i2c_send_data(I2C0, 0x90);
	//i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
	//while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET);
  // restart
	i2c_master_send_start(I2C0, addr, I2C_READ);
	//i2c_master_start(I2C0, addr, I2C_READ);
	i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
	//while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET);
	retval = i2c0_waitforflag(I2C_FLAG_TRANS_EMPTY);
	if(retval != 0) goto stop;
	//read data
	for(i = 0; i < (len-1); i++)
	{
		i2c_set_receive_mode(I2C0, I2C_ACK);
    //while (i2c_get_flag_status(I2C0, I2C_FLAG_RECV_FULL) != SET);
		retval = i2c0_waitforflag(I2C_FLAG_RECV_FULL);
	  if(retval != 0) goto stop;
    i2c_clear_flag_status(I2C0, I2C_FLAG_RECV_FULL);
    data[i] = i2c_receive_data(I2C0);
	}
	i++;
	i2c_set_receive_mode(I2C0, I2C_NAK);
	//while (i2c_get_flag_status(I2C0, I2C_FLAG_RECV_FULL) != SET);
	retval = i2c0_waitforflag(I2C_FLAG_RECV_FULL);
	if(retval != 0) goto stop;
	i2c_clear_flag_status(I2C0, I2C_FLAG_RECV_FULL);
	data[i] = i2c_receive_data(I2C0);
	// stop
stop:
	i2c_master_send_stop(I2C0);
	return retval;
}

int32_t i2c0_waitforflag(i2c_flag_t flag)
{
	int32_t retval = -1; 
	
	timeout = 10000;
	while(timeout--)
	{
		if((i2c_get_flag_status(I2C0, flag) == SET)) 
		{
			retval =  0;
			break;
		}
	}
	return retval;
}