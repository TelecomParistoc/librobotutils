/* i2c functions
 * provides functions to acces the I2C bus
 */

#ifndef I2CFUNCTIONS_H
#define I2CFUNCTIONS_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ############## I2C functions ##############
 * If the i2c device is not open, they will open it before the read/write
 * but it is also possible to do it manually with i2c_init()
 */

int i2c_init(uint8_t addr);

uint8_t I2Cread8(uint8_t addr, uint8_t reg);
int I2Cwrite8(uint8_t addr, uint8_t reg, uint8_t value);

uint16_t I2Cread16(uint8_t addr, uint8_t reg);
int I2Cwrite16(uint8_t addr, uint8_t reg, uint16_t value);

// actually issues two read/write of 16 bits
uint32_t I2Cread32(uint8_t addr, uint8_t reg);
int I2Cwrite32(uint8_t addr, uint8_t reg, uint32_t value);

//actually reads a int32 and interprets the data as a float */
float I2CreadFloat(uint8_t addr, uint8_t reg);
int I2CwriteFloat(uint8_t addr, uint8_t reg, float x);

#ifdef __cplusplus
}
#endif

#endif
