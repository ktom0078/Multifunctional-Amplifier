#include "pcm.h"
#include "dma.h"
#include "tm_stm32f4_fatfs.h"
#include "menu.h"
#include <stdio.h>
#include <string.h>

char dma_buff[DMA_AUDIO_BUFFER_SIZE];
char dma_buff2[DMA_AUDIO_BUFFER_SIZE];
enum state_enum {init_st,play_st,end_st};
enum state_enum state = init_st;

FATFS FatFs;
FIL fil;
TM_FATFS_Partition partition;

bool PcmOpenFile(char *filename,TM_FATFS_Partition apartition)
{
	bool retval = false;
	char fullname[15];

	if(apartition == partition_usb)
	{
		fullname[0] = '1';
	}
	else
	{
		fullname[0] = '0';
	}
	fullname[1] = ':';

	memcpy(&fullname[2],filename,strlen(filename));
	fullname[strlen(filename)+ 2] = 0;

	if (f_open(&fil, fullname, FA_READ ) == FR_OK)
	{
		retval = true;
		state = init_st;
	}

	return retval;
}

/* retval: false if playback finished, else true */
bool PcmPlay()
{
	enum act_buff_enum {fb,sb};
	static enum act_buff_enum act_buff;
	static unsigned int btr;
	static unsigned int br;
	static bool loadneeded;
	bool retval = true;

	if(state == init_st)
	{
		act_buff = fb;
		btr = DMA_AUDIO_BUFFER_SIZE;
		br = btr + 1;
		DMA_Audio_Init_Single(dma_buff, DMA_AUDIO_BUFFER_SIZE / 2);
		loadneeded = true;
		state = play_st;
	}
	else if(state == play_st)
	{
		if(br >= btr)
		{
			if (RotSwPRessed())
			{
				f_close(&fil);
				state = end_st;
				retval = false;
			}
			if(loadneeded)
			{
				if (act_buff == fb)
					f_read(&fil, dma_buff, btr, &br);
				else
					f_read(&fil, dma_buff2, btr, &br);
				loadneeded = false;
			}

			if(DmaAudioDone)
			{
				DmaAudioDone = false;
				loadneeded = true;
				if (act_buff == fb)
					DMA_Audio_Send(dma_buff, DMA_AUDIO_BUFFER_SIZE / 2);
				else
					DMA_Audio_Send(dma_buff2, DMA_AUDIO_BUFFER_SIZE / 2);

				if (act_buff == fb)
					act_buff = sb;
				else
					act_buff = fb;
			}
		}
		else
		{
			f_close(&fil);
			state = end_st;
			retval = false;
		}
	}

	return retval;
}
