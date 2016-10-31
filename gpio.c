#include  "stm32f4xx_gpio.h"
#include  "stm32f4xx_rcc.h"
#include  "gpio.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_exti.h"

void Gpio_Init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

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

	GPIO_InitTypeDef GPIO_InitStruct3;
	GPIO_InitStruct3.GPIO_Pin = ROT_SW_PIN;
	GPIO_InitStruct3.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct3.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct3.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(ROT_PORT, &GPIO_InitStruct3);

	/* Init buttons */
	TM_EXTI_Attach(ROT_PORT, ROT_SW_PIN, TM_EXTI_Trigger_Falling);

	GPIO_WriteBit(I2C_PORT,I2C_OE_PIN,Bit_SET);  // disable OE

	GPIO_PinAFConfig(I2C_PORT, GPIO_PinSource6, GPIO_AF_I2C1);	// SCL
	GPIO_PinAFConfig(I2C_PORT, GPIO_PinSource7, GPIO_AF_I2C1); // SDA
}

void Gpio_BT_Enable_Cmd()
{
	/* Set cmd pin to output and set it to logic  0 */
	GPIO_InitTypeDef GPIO_InitStruct4;
	GPIO_InitStruct4.GPIO_Pin = NCOMMAND_PIN;
	GPIO_InitStruct4.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct4.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct4.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct4.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(NCOMMAND_PORT, &GPIO_InitStruct4);

	/* enable comman mode */
	GPIO_WriteBit(NCOMMAND_PORT,NCOMMAND_PIN,Bit_RESET);
}

void Gpio_BT_Disable_Cmd()
{
	/* Set cmd pin to input floating  */
	GPIO_InitTypeDef GPIO_InitStruct4;
	GPIO_InitStruct4.GPIO_Pin = NCOMMAND_PIN;
	GPIO_InitStruct4.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct4.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct4.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(NCOMMAND_PORT, &GPIO_InitStruct4);
}
