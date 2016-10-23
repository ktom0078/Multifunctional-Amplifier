#ifndef GPIO_H
#define GPIO_H

#include  "stm32f4xx_gpio.h"

/* Defines */
#define I2C_PORT 			GPIOB
#define I2C_OE_PIN			GPIO_Pin_5
#define I2C_SCL_PIN 		GPIO_Pin_6
#define I2C_SDA_PIN 		GPIO_Pin_7

#define ROT_PORT			GPIOC
#define ROT_SW_PIN			GPIO_Pin_14

/* functions */

void Gpio_Init();

#endif
