#include  "stm32f4xx_gpio.h"
#include  "stm32f4xx_rcc.h"
#include "gpio.h"


void Gpio_Init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN; // we are going to use PB6 and PB7
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			// set pins to alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;		// set GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// set output to open drain --> the line has to be only pulled low, not driven high
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;		// disable pull up resistors -> txs0102 has it
	GPIO_Init(I2C_PORT, &GPIO_InitStruct);				// init GPIOB

	GPIO_InitTypeDef GPIO_InitStruct2;
	GPIO_InitStruct2.GPIO_Pin = I2C_OE_PIN; // we are going to use PB6 and PB7
	GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_OUT;			// set pins to alternate function
	GPIO_InitStruct2.GPIO_Speed = GPIO_Speed_2MHz;		// set GPIO speed
	GPIO_InitStruct2.GPIO_OType = GPIO_OType_PP;			// set output to open drain --> the line has to be only pulled low, not driven high
	GPIO_InitStruct2.GPIO_PuPd = GPIO_PuPd_NOPULL;		// disable pull up resistors -> txs0102 has it
	GPIO_Init(I2C_PORT, &GPIO_InitStruct2);				// init GPIOB

	GPIO_WriteBit(I2C_PORT,I2C_OE_PIN,Bit_SET);  // disable OE

	GPIO_PinAFConfig(I2C_PORT, GPIO_PinSource6, GPIO_AF_I2C1);	// SCL
	GPIO_PinAFConfig(I2C_PORT, GPIO_PinSource7, GPIO_AF_I2C1); // SDA
}
