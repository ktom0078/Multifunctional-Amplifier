#ifndef MP3_H
#define MP3_H


#include "mp3dec.h"
#include "dma.h"
#include "ff.h"
#include "gpio.h"
#include "pcm.h"
#include "attributes.h"
#include "tm_stm32f4_fatfs.h"


#define		MP3_MAX_TRACK_NUM	100
#define 	MP3_FRAME_SIZE		MAX_NCHAN * MAX_NGRAN * MAX_NSAMP
#define 	MP3_OVER_RATE		4
#define 	MP3_BUFF_SIZE 		(MP3_OVER_RATE*MP3_FRAME_SIZE)
#define		AUDIO_BUFF_SIZE		(MP3_OVER_RATE*1940)

typedef enum  {
	st_init,st_play,st_stopped,st_pause,st_end
}Mp3_Status_e;

extern Mp3_Status_e Mp3_Status;
extern FIL fil;

bool Mp3Play();
bool Mp3OpenFile(char *filename,TM_FATFS_Partition apartition);
void MP3_Debug(int err);
void Mp3SendBuffer();

#endif
