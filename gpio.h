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

#define BUTTON_PORT			GPIOC
#define BUTTON_PIN			GPIO_Pin_0

#define NCOMMAND_PIN		GPIO_Pin_2
#define NCOMMAND_PORT		GPIOA

/* LCD PINS */
#define GLCD_EN_PIN			GPIO_Pin_1
#define GLCD_EN_PORT		GPIOC

#define GLCD_RW_PIN			GPIO_Pin_2
#define GLCD_RW_PORT		GPIOC

#define GLCD_CS1_PIN		GPIO_Pin_3
#define GLCD_CS1_PORT		GPIOC

#define GLCD_E_PIN			GPIO_Pin_4
#define GLCD_E_PORT			GPIOC

#define GLCD_DI_PIN			GPIO_Pin_5
#define GLCD_DI_PORT		GPIOC

#define GLCD_DB0_PIN		GPIO_Pin_6
#define GLCD_DB0_PORT		GPIOC

#define GLCD_DB1_PIN		GPIO_Pin_15
#define GLCD_DB1_PORT		GPIOB

#define GLCD_DB2_PIN		GPIO_Pin_14
#define GLCD_DB2_PORT		GPIOB

#define GLCD_DB3_PIN		GPIO_Pin_13
#define GLCD_DB3_PORT		GPIOB

#define GLCD_DB4_PIN		GPIO_Pin_12
#define GLCD_DB4_PORT		GPIOB

#define GLCD_DB5_PIN		GPIO_Pin_11
#define GLCD_DB5_PORT		GPIOB

#define GLCD_DB6_PIN		GPIO_Pin_1
#define GLCD_DB6_PORT		GPIOB

#define GLCD_DB7_PIN		GPIO_Pin_10
#define GLCD_DB7_PORT		GPIOB

#define GLCD_CS2_PIN		GPIO_Pin_3
#define GLCD_CS2_PORT		GPIOB

#define GLCD_PWM_PIN		GPIO_Pin_9
#define GLCD_PWM_PORT		GPIOC

#define GLCD_RESET_PIN		GPIO_Pin_0
#define GLCD_RESET_PORT		GPIOB

/* functions */

void Gpio_Init();
void Gpio_BT_Enable_Cmd();
void Gpio_BT_Disable_Cmd();

#endif
