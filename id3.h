#ifndef _ID3_H_
#define _ID3_H_

#include "ff.h"


unsigned int  Mp3ReadId3V2Text(FIL* pInFile, unsigned int unDataLen, char* pszBuffer, unsigned int unBufferSize);
unsigned int Mp3ReadId3V2Tag(FIL* pInFile, char* pszArtist, unsigned int unArtistSize, char* pszTitle, unsigned int unTitleSize);






#endif
