#ifndef FILE_H
#define FILE_H



#include "ff.h"
#include "diskio.h"
#include <stdio.h>

FRESULT scan_files (
    char* path        /* Start node to be scanned (also used as work area) */
);
unsigned int File_Cmp_Ext(char* fname,char* ext);


#endif
