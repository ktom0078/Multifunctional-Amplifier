#include "pcm.h"
#include "dma.h"
#include "tm_stm32f4_fatfs.h"
#include "menu.h"

char dma_buff[DMA_AUDIO_BUFFER_SIZE];
char dma_buff2[DMA_AUDIO_BUFFER_SIZE];

FATFS FatFs;
FIL fil;



unsigned int PcmPlay(char *filename) {


	enum act_buff_enum {fb,sb};
	enum act_buff_enum act_buff;
	act_buff = fb;

   unsigned int btr = DMA_AUDIO_BUFFER_SIZE;
   unsigned int br = btr + 1;
   	   DMA_Audio_Init_Single(dma_buff, DMA_AUDIO_BUFFER_SIZE / 2);
	if (f_open(&fil, filename, FA_OPEN_ALWAYS | FA_READ | FA_WRITE ) == FR_OK) {
		while(br >= btr)
		{
			if (RotSwPRessed())
				break;
			if (act_buff == fb)
				f_read(&fil, dma_buff, btr, &br);
			else
				f_read(&fil, dma_buff2, btr, &br);

			while (!DmaAudioDone) asm("nop");

			DmaAudioDone = false;
			if (act_buff == fb)
				DMA_Audio_Send(dma_buff, DMA_AUDIO_BUFFER_SIZE / 2);
			else
				DMA_Audio_Send(dma_buff2, DMA_AUDIO_BUFFER_SIZE / 2);

			if (act_buff == fb)
				act_buff = sb;
			else
				act_buff = fb;

		}

		 f_close(&fil);
		 return 1;
	}
	else return 0;
}
