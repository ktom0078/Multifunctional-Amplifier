#include "dma.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"

volatile bool DmaAudioDone = true;

void DMA_Audio_Init_Single(short int * buff,unsigned int len) {
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // init dma1 clock

	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Tx, ENABLE);
	DMA_AUDIO_STREAM->CR = (0 * DMA_SxCR_CHSEL_0 ) | // Channel 0
			(1 * DMA_SxCR_PL_0 ) | // Priority 1
			(1 * DMA_SxCR_PSIZE_0 ) | // PSIZE = 16 bit
			(1 * DMA_SxCR_MSIZE_0 ) | // MSIZE = 16 bit
			DMA_SxCR_MINC | // Increase memory address
			(1 * DMA_SxCR_DIR_0 ) | // Memory to peripheral
			DMA_SxCR_TCIE; // Transfer complete interrupt
	DMA_AUDIO_STREAM->NDTR = len;
	DMA_AUDIO_STREAM->PAR = (uint32_t) &SPI3->DR;
	DMA_AUDIO_STREAM->M0AR = (uint32_t) buff;
	DMA_AUDIO_STREAM->FCR = DMA_SxFCR_DMDIS;

	//DMA_AUDIO_STREAM ->CR |= DMA_SxCR_EN;

	DMA_ClearFlag(DMA_AUDIO_STREAM,DMA_AUDIO_TCFLAG);
	DMA_ITConfig(DMA_AUDIO_STREAM, DMA_IT_TC | DMA_IT_TE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	 // DMA interrupt for sending data to speaker
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;
	NVIC_Init(&NVIC_InitStructure);


}

void DMA_Audio_Send(short int * buff,unsigned int len)
{
	DMA_AUDIO_STREAM->M0AR = (uint32_t)buff;
	DMA_AUDIO_STREAM->NDTR = len;
	DMA_AUDIO_STREAM->CR  |= DMA_SxCR_EN;
}

void DMA1_Stream7_IRQHandler()
{
	if (DMA_AUDIO->HISR & DMA_AUDIO_TC_SET)
	{
		DMA_ClearITPendingBit(DMA_AUDIO_STREAM, DMA_AUDIO_TCFLAG);
		DMA_ClearFlag(DMA_AUDIO_STREAM, DMA_AUDIO_TCFLAG);
		DmaAudioDone = true;
	}
	else if (DMA_GetITStatus(DMA_AUDIO_STREAM, DMA_FLAG_HTIF7))
	{
		DMA_ClearITPendingBit(DMA_AUDIO_STREAM, DMA_FLAG_HTIF7);
		DMA_ClearFlag(DMA_AUDIO_STREAM, DMA_FLAG_HTIF7);
	}
	else if (DMA_GetITStatus(DMA_AUDIO_STREAM, DMA_FLAG_TEIF7))
	{
		DmaAudioDone = false;
	}
}

