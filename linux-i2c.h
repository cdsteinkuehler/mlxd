/* Defines for LINUX_I2C */
#ifndef LINUX_I2C_H
#define LINUX_I2C_H

#include <stdint.h>

/*! \brief linuxI2CReasonCodes
  Specifies the reason codes for the linux_i2c_write and linux_i2c_read functions.
*/
typedef enum
{
    LINUX_I2C_REASON_OK   	     = 0x00,      /*!< Success */
    LINUX_I2C_REASON_ERROR_NACK    = 0x01,      /*!< Received a NACK */
    LINUX_I2C_REASON_ERROR_CLKT    = 0x02,      /*!< Received Clock Stretch Timeout */
    LINUX_I2C_REASON_ERROR_DATA    = 0x04       /*!< Not all data is sent / received */
} linuxI2CReasonCodes;

#ifdef __cplusplus
extern "C" {
#endif

    extern int linux_i2c_init(int bus);

    /*! Start I2C operations.
      Forces RPi I2C pins P1-03 (SDA) and P1-05 (SCL)
      to alternate function ALT0, which enables those pins for I2C interface.
      You should call linux_i2c_end() when all I2C functions are complete to return the pins to
      their default functions
      \return 1 if successful, 0 otherwise (perhaps because you are not running as root)
      \sa  linux_i2c_end()
    */
    extern int linux_i2c_begin(void);

    /*! End I2C operations.
      I2C pins P1-03 (SDA) and P1-05 (SCL)
      are returned to their default INPUT behaviour.
    */
    extern void linux_i2c_end(void);

    /*! Sets the I2C slave address.
      \param[in] addr The I2C slave address.
    */
    extern void linux_i2c_setSlaveAddress(uint8_t addr);

    /*! Sets the I2C clock divider by converting the baudrate parameter to
      the equivalent I2C clock divider. ( see \sa linux_i2c_setClockDivider)
      For the I2C standard 100khz you would set baudrate to 100000
      The use of baudrate corresponds to its use in the I2C kernel device
      driver. (Of course, linux has nothing to do with the kernel driver)
    */
    extern void linux_i2c_set_baudrate(uint32_t baudrate);

    /*! Transfers any number of bytes to the currently selected I2C slave.
      (as previously set by \sa linux_i2c_setSlaveAddress)
      \param[in] buf Buffer of bytes to send.
      \param[in] len Number of bytes in the buf buffer, and the number of bytes to send.
      \return reason see \ref linuxI2CReasonCodes
    */
    extern uint8_t linux_i2c_write(const char * buf, uint32_t len);

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
    extern uint8_t linux_i2c_write_read_rs(char* cmds, uint32_t cmds_len, char* buf, uint32_t buf_len);

#ifdef __cplusplus
}
#endif

#endif /* LINUX_I2C_H */


