#include "file.h"
#include "str.h"
#include "pcm.h"
#include "mp3.h"
#include "stdlib.h"
#include "glcd.h"
#include "id3.h"

FRESULT scan_files (
    char* path        /* Start node to be scanned (also used as work area) */
)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    FIL tfil;
    int i;
    char *fn;   /* This function assumes non-Unicode configuration */
    char buff[20];
    char* filename;
    char* artist;
    char* title;

    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK)
    {
        i = StrLen(path);
        for (;;)
        {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fname[0] == '.') continue;             /* Ignore dot entry */

            fn = fno.fname;
            if (!(fno.fattrib & AM_DIR))
            {	//if its a file
				if(File_Cmp_Ext(fn,"MP3"))
				{
					Mp3Count++;
					tMp3Array = (tMp3Track*) realloc (Mp3Array, Mp3Count * sizeof(tMp3Track));
					if (tMp3Array!=NULL)
					{
						Mp3Array=tMp3Array;
						filename = (char*)(&(Mp3Array[Mp3Count-1].Path));
						artist   = (char*)(&(Mp3Array[Mp3Count-1].Artist));
						title    = (char*)(&(Mp3Array[Mp3Count-1].Title));
						/* Copy the drive ID, 0: - sd, 1: - usb */
						StrCopy(path,filename);
						StrCopy(fn,filename+2); // skip the 0: or 1:

						if (f_open(&tfil, filename, FA_READ ) == FR_OK)
						{
							Mp3ReadId3V2Tag(&tfil, artist, ID3_DATA_LEN, title, ID3_DATA_LEN);
						}
					}
					else
					{
						Mp3Count--;
					}
				}

			}

        }
        if(Mp3Count)
        {
        	Mp3ActIndex = 0;
        	Mp3ActTrack = (tMp3Track*)(&Mp3Array[Mp3ActIndex]);
       	}

        f_closedir(&dir);
        /*
        for(i=0;i<Mp3Count;i++)
        {
        	sprintf(buff,"%s",Mp3Array[i].Path);
        	GLCD_WriteString(buff,0,i);
        }*/
    }

    return res;
}

//Checks if the extension equal to the given,
unsigned int File_Cmp_Ext(char* fname,char* ext)
{
	int j=0; //for support long names too
	while(fname[j]!='.') j++;
	j++; //skip "."
	return StrCmp(&fname[j],ext);

}


