#include "mp3.h"
#include "gpio.h"
#include <stdio.h>
#include "menu.h"
#include "tm_stm32f4_usb_msc_host.h"


short int mp3_buffer[MP3_BUFF_SIZE];
short int mp3_buffer2[MP3_BUFF_SIZE];
char audio_buffer[AUDIO_BUFF_SIZE];
char audio_help_buff[AUDIO_BUFF_SIZE];
int bytes_left;
char *read_ptr;
bool loadneeded = true;

MP3FrameInfo mp3FrameInfo;
HMP3Decoder hMP3Decoder;

FIL fil;
FATFS USB_Fs;
FATFS SD_Fs;


Mp3_Status_e Mp3_Status = st_end;
enum act_buff_enum {fb,sb};
enum act_buff_enum act_buff;
int errbr;

bool Mp3OpenFile(char *filename,TM_FATFS_Partition apartition)
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
	}

	return retval;
}

bool Mp3Play(char *filename,TM_FATFS_Partition apartition) {
	int offset, err;

	unsigned int h;

	static unsigned int btr;
	static unsigned int br;
	unsigned int sum_bytes_read = 0;

	int k,l;

	bool retval = true;

	switch(Mp3_Status)
	{
	case st_init:
		if(Mp3OpenFile(filename,apartition) == true)
		{
			act_buff = fb;
			btr = AUDIO_BUFF_SIZE;
			br = btr + 1;
			DMA_Audio_Init_Single(mp3_buffer, MP3_BUFF_SIZE);
			loadneeded = true;
			Mp3_Status = st_play;
			hMP3Decoder = MP3InitDecoder();
			read_ptr = audio_buffer;
			bytes_left = AUDIO_BUFF_SIZE;
		}
		else
		{
			/* TODO: do something when file open fails */
			retval = false;
		}

		break;
	case st_play:
		if(br >= btr)
		{
			if(loadneeded)
			{
				if (bytes_left == AUDIO_BUFF_SIZE)
				{
					f_read(&fil, audio_buffer, btr, &br);
					if (br < btr)
						break;
					sum_bytes_read+=br;

				}
				else
				{
					l = 0;
					for (k = AUDIO_BUFF_SIZE - bytes_left; k < AUDIO_BUFF_SIZE;
							k++) {
						audio_help_buff[l++] = audio_buffer[k];
					}
					f_read(&fil, &audio_buffer[bytes_left],
							AUDIO_BUFF_SIZE - bytes_left, &errbr);
					if (errbr < AUDIO_BUFF_SIZE - bytes_left)
						break;

					sum_bytes_read+=errbr;

					for (k = 0; k < bytes_left; k++) {
						audio_buffer[k] = audio_help_buff[k];
					}

					bytes_left = AUDIO_BUFF_SIZE;

				}
				//mindket esetben az elejérõl olvasunk utána
				read_ptr = audio_buffer;

				for (h = 0; h < MP3_OVER_RATE; h++)
				{
					offset = MP3FindSyncWord((unsigned char*) read_ptr,
							bytes_left);

					bytes_left -= offset;
					read_ptr += offset;

					err = MP3Decode(hMP3Decoder, (unsigned char**)&read_ptr, (int*)&bytes_left,
							act_buff == fb ?
									(short*) mp3_buffer + h * MP3_FRAME_SIZE :
									(short*) mp3_buffer2 + h * MP3_FRAME_SIZE
							, 0);

				}
				MP3GetLastFrameInfo(hMP3Decoder, &mp3FrameInfo);
				//MP3_Debug(err);*/

				loadneeded = false;

				/* Buffer sending is done in the IT routine */
			}

		}
		else
		{
			Mp3_Status = st_stopped;
			retval = false;
		}
		break;
	case st_stopped:
		f_close(&fil);
		retval = false;
		Mp3_Status = st_end;
		break;
	case st_end:
		/* Do nothing */
		retval = false;
		break;
	}
return retval;
}

void MP3_Debug(int err)
{
	if (err)
	{
		/* error occurred */
		switch (err)
		{
		case ERR_MP3_INDATA_UNDERFLOW:
			break;
		case ERR_MP3_MAINDATA_UNDERFLOW:
			break;
		case ERR_MP3_FREE_BITRATE_SYNC:
			break;
		default:
			break;
		}
	} else {

		/* no error */
	}
}

/* Sends the content of the audio buffer when its needed  */
void Mp3SendBuffer()
{
	if(DmaAudioDone && loadneeded == false)
	{
		DmaAudioDone = false;
		loadneeded = true;
		if (act_buff == fb)
			DMA_Audio_Send(mp3_buffer, MP3_BUFF_SIZE);
		else
			DMA_Audio_Send(mp3_buffer2, MP3_BUFF_SIZE);
		if (act_buff == fb)
			act_buff = sb;
		else
			act_buff = fb;
	}
}

unsigned char Mp3MountDevices()
{
	unsigned int i;
	unsigned char mountcntr = 0;

	/* Put some delay to USB to work */
	for(i=0;i<0xffff;i++)
	{
		if (f_mount(&USB_Fs, "1:", 1) == FR_OK)
		{
			mountcntr++;
			break;
		}
		TM_USB_MSCHOST_Process();
	}
	if (f_mount(&SD_Fs, "0:", 1) == FR_OK)
	{
		mountcntr++;
	}

	if(mountcntr )Mp3_Status = st_init;

	return mountcntr;
}
