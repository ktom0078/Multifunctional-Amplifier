#ifndef PCM_H
#define PCM_H

#include "tm_stm32f4_fatfs.h"


unsigned int PcmPlay(char *filename);

extern FATFS FatFs;
extern FIL fil;

#endif
