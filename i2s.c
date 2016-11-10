#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "i2s.h"
#include "gpio.h"


void Cs43InitGpios()
{
	GPIO_InitTypeDef PinInitStruct;
	GPIO_InitTypeDef PinInitStruct2;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

	//i2s pins					mclk		sclk		sdin
	PinInitStruct.GPIO_Pin = GPIO_Pin_7 |GPIO_Pin_10 | GPIO_Pin_12;
	PinInitStruct.GPIO_Mode = GPIO_Mode_AF;
	PinInitStruct.GPIO_OType = GPIO_OType_PP;
	PinInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	PinInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &PinInitStruct);

	//i2s pin ws
	PinInitStruct2.GPIO_Pin = GPIO_Pin_15;
	PinInitStruct2.GPIO_Mode = GPIO_Mode_AF;
	PinInitStruct2.GPIO_OType = GPIO_OType_PP;
	PinInitStruct2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	PinInitStruct2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &PinInitStruct2);

	//i2s alternate conf
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

}


void Cs43Init() {
	Cs43InitGpios();

	SPI3->I2SCFGR = 0;

	// I2S clock configuration
	RCC->CFGR &= ~RCC_CFGR_I2SSRC; // PLLI2S clock used as I2S clock source.
	RCC->PLLI2SCFGR = (I2S_PLLR << 28) | (I2S_PLLN << 6);

	// Enable PLLI2S and wait until it is ready.
	RCC->CR |= RCC_CR_PLLI2SON;
	while (!(RCC->CR & RCC_CR_PLLI2SRDY ));

	// Configure I2S.
	SPI3->I2SPR = I2S_DIV | (I2S_ODD << 8) | SPI_I2SPR_MCKOE;
	SPI3->I2SCFGR = SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_I2SCFG_1
			| SPI_I2SCFGR_I2SE;

}
