#ifndef PCM_H
#define PCM_H

#include "tm_stm32f4_fatfs.h"
#include "attributes.h"

extern FATFS FatFs;
extern FIL fil;

bool PcmPlay();
bool PcmOpenFile(char *filename,TM_FATFS_Partition apartition);


#endif
