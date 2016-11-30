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
#define 	ID3_DATA_LEN		20

typedef enum  {
	st_init,st_play,st_stopped,st_pause,st_changetr,st_end
}Mp3_Status_e;

typedef struct
{
	char Path[15]; /* Store path for the drive also */
	char Artist[ID3_DATA_LEN];
	char Title[ID3_DATA_LEN];
}tMp3Track;

extern Mp3_Status_e Mp3_Status;
extern FIL fil;
extern unsigned int Mp3Count;
extern tMp3Track* Mp3Array;
extern tMp3Track* tMp3Array;
extern tMp3Track* Mp3ActTrack;
extern unsigned int Mp3ActIndex;
extern bool Mp3TrackChanged;


bool Mp3Play(char *filename);
bool Mp3OpenFile(char *filename);
void MP3_Debug(int err);
void Mp3SendBuffer();
unsigned char Mp3MountDevices();
void Mp3ChangeTrack(unsigned int index);
void Mp3PauseTrack();
void Mp3StartResumeTrack();
void Mp3NextTrack();
void Mp3PrevTrack();
unsigned int Mp3GetActTrackInd();

#endif
