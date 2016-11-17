#ifndef DMA_H
#define DMA_H

#include "misc.h"
#include "stm32f4xx_dma.h"
#include "attributes.h"

#define DMA_AUDIO				DMA1
#define	DMA_AUDIO_STREAM		DMA1_Stream7
#define	DMA_AUDIO_CHANNEL		DMA_Channel_0
#define DMA_AUDIO_BUFFER_SIZE   (1024*10)
#define DMA_AUDIO_TCFLAG 		DMA_FLAG_TCIF7
#define DMA_AUDIO_HTFLAG		DMA_FLAG_HTIF7
#define DMA_AUDIO_TC_SET		(1<<27)

extern volatile bool DmaAudioDone;


void DMA_Audio_Init_Single(short int * buff,unsigned int len);
void DMA_Audio_Send(short int * buff,unsigned int len);


#endif
