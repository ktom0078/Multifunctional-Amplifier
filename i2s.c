#include "stm32f4xx.h"
#include "i2s.h"

void Cs43Init() {
		SPI3->I2SCFGR = 0;

		// I2S clock configuration
		RCC->CFGR &= ~RCC_CFGR_I2SSRC; // PLLI2S clock used as I2S clock source.
		RCC->PLLI2SCFGR = (I2S_PLLR << 28) | (I2S_PLLN << 6);

		// Enable PLLI2S and wait until it is ready.
		RCC->CR |= RCC_CR_PLLI2SON;
		while (!(RCC->CR & RCC_CR_PLLI2SRDY ))
			;

		// Configure I2S.
		SPI3->I2SPR = I2S_DIV | (I2S_ODD << 8) | SPI_I2SPR_MCKOE;
		SPI3->I2SCFGR = SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_I2SCFG_1
				| SPI_I2SCFGR_I2SE;

}
