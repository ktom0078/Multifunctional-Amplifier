#include "bt.h"
#include "gpio.h"
#include "tm_stm32f4_usart.h"


bool FlagVolUp   = false;
bool FlagVolDown = false;

/* Check if any new command received over bluetooth */
void BTBtnsProc()
{
	char btnbuff[BT_BUTTON_CMD_LEN];

	if(TM_USART_Gets(UART4,btnbuff,BT_BUTTON_CMD_LEN))
	{
		/* the first char is the commmand */
		switch(btnbuff[0])
		{
			case 'u':
				FlagVolUp   = true;
				break;
			case 'd':
				FlagVolDown = true;
				break;
		}
	}
}
