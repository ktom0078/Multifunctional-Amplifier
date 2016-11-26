#include "file.h"
#include "str.h"
#include "pcm.h"
#include "mp3.h"
#include "stdlib.h"
#include "glcd.h"


FRESULT scan_files (
    char* path        /* Start node to be scanned (also used as work area) */
)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;   /* This function assumes non-Unicode configuration */
    char buff[20];

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
						StrCopy(fn,(char*)(&(Mp3Array[Mp3Count-1].Mp3Track)));
					}
					else
					{
						Mp3Count--;
					}
				}

			}

        }
        f_closedir(&dir);

        for(i=0;i<Mp3Count;i++)
        {
        	sprintf(buff,"%s",Mp3Array[i].Mp3Track);
        	GLCD_WriteString(buff,0,i);
        }
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


