#include "bt.h"
#include "gpio.h"
#include "tm_stm32f4_usart.h"
#include "preamp.h"
#include "mp3.h"


/* Check if any new command received over bluetooth */
void BTBtnsProc()
{
	char btnbuff[BT_BUTTON_CMD_LEN];
	unsigned char MenuIndex;
	ePreampAction action = Nothing;

	if(TM_USART_Gets(UART4,btnbuff,BT_BUTTON_CMD_LEN))
	{
		/* the first char is the commmand */
		switch(btnbuff[0])
		{
			case 'v':
				/* Volume */
				MenuIndex = 0;
				if(btnbuff[1] == 'u')
				{
					/* Up */
					action = Increase;
				}
				else if(btnbuff[1] == 'd')
				{
					/* Down */
					action = Decrease;
				}
				MenuChangeSetting(MenuIndex,action);
				break;
			case 't':
				MenuIndex = 1;
				/* Treble*/
				if(btnbuff[1] == 'u')
				{
					/* Up */
					action = Increase;
				}
				else if(btnbuff[1] == 'd')
				{
					/* Down */
					action = Decrease;
				}
				MenuChangeSetting(MenuIndex,action);
				break;
			case 'b':
				MenuIndex = 2;
				/* Bass*/
				if(btnbuff[1] == 'u')
				{
					/* Up */
					action = Increase;
				}
				else if(btnbuff[1] == 'd')
				{
					/* Down */
					action = Decrease;
				}
				MenuChangeSetting(MenuIndex,action);
				break;

			case 's':
				MenuIndex = 3;
				/* Source */
				if(btnbuff[1] == 'n')
				{
					/* Next */
					action = Increase;
				}
				else if(btnbuff[1] == 'p')
				{
					/* Prev */
					action = Decrease;
				}
				MenuChangeSetting(MenuIndex,action);
				break;
			case 'S':
				/* Song */
				if(btnbuff[1] == 'n')
				{
					/* Next */
					Mp3NextTrack();
				}
				else if(btnbuff[1] == 'p')
				{
					/* Prev */
					Mp3PrevTrack();
				}
				break;
		}
	}
}
