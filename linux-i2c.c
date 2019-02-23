#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

// Use standard linux i2c calls instead of bcm2835 library
#include "linux-i2c.h"

int file = -1;
uint16_t slave_addr;


int linux_i2c_init(int bus)
{
	// Only try to open the file once
	if (file < 0)
	{
		char filename[20];

		snprintf(filename, 19, "/dev/i2c-%d", bus);
		file = open(filename, O_RDWR);
	}

	if (file < 0)
		return 0;
	else
		return 1;
};

int linux_i2c_begin(void)
{
	return 0;
};

void linux_i2c_end(void)
{
	close(file);
};

void linux_i2c_setSlaveAddress(uint8_t addr)
{
	slave_addr = addr ;
};

void linux_i2c_set_baudrate(uint32_t baudrate)
{
	// Noop
	// I2C speed is set by the Linux I2C driver when it loads
};

    /*! Transfers any number of bytes to the currently selected I2C slave.
      (as previously set by \sa linux_i2c_setSlaveAddress)
      \param[in] buf Buffer of bytes to send.
      \param[in] len Number of bytes in the buf buffer, and the number of bytes to send.
      \return reason see \ref linuxI2CReasonCodes
    */
uint8_t linux_i2c_write(const char * buf, uint32_t len)
{
	struct i2c_rdwr_ioctl_data msgset;
	struct i2c_msg msgs[1];

	msgs[0].addr = slave_addr;
	msgs[0].flags = 0;
	msgs[0].len = len;
	msgs[0].buf = (char *) buf;

	msgset.msgs = msgs;
	msgset.nmsgs = 1;

	int ret;
	ret = ioctl(file, I2C_RDWR, &msgset);
	if (ret >= 0) return LINUX_I2C_REASON_OK;
	else return LINUX_I2C_REASON_ERROR_NACK;
};

    /*! Allows sending an arbitrary number of bytes to I2C slaves before issuing a repeated
      start (with no prior stop) and reading a response.
      Necessary for devices that require such behavior, such as the MLX90620.
      Will write to and read from the slave previously set by \sa linux_i2c_setSlaveAddress
      \param[in] cmds Buffer containing the bytes to send before the repeated start condition.
      \param[in] cmds_len Number of bytes to send from cmds buffer
      \param[in] buf Buffer of bytes to receive.
      \param[in] buf_len Number of bytes to receive in the buf buffer.
      \return reason see \ref linuxI2CReasonCodes
    */
uint8_t linux_i2c_write_read_rs(char* cmds, uint32_t cmds_len, char* buf, uint32_t buf_len)
{
	struct i2c_rdwr_ioctl_data msgset;
	struct i2c_msg msgs[2];

	msgs[0].addr = slave_addr;
	msgs[0].flags = 0;
	msgs[0].len = cmds_len;
	msgs[0].buf = cmds;

	msgs[1].addr = slave_addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = buf_len;
	msgs[1].buf = buf;

	msgset.msgs = msgs;
	msgset.nmsgs = 2;

	int ret;
	ret = ioctl(file, I2C_RDWR, &msgset);
	if (ret >= 0) return LINUX_I2C_REASON_OK;
	else return LINUX_I2C_REASON_ERROR_NACK;
};

