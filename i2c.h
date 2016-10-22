#ifndef I2C_H
#define I2C_H

#include  "stm32f4xx_i2c.h"

/* Defines */
#define TDA7318_I2C_ADDRESS 0x88


/* Functions */
void I2C_Init_Amp();
void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
void I2C_stop(I2C_TypeDef* I2Cx);


#endif
