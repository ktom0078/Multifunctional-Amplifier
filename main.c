#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "system_stm32f4xx.h"
#include "stm32f4xx.h"
#include "i2c.h"
#include "gpio.h"
#include "stm32f4xx_i2c.h"
#include "preamp.h"
#include "tm_stm32f4_rotary_encoder.h"
#include "tm_stm32f4_usart.h"
#include "menu.h"
#include "bt.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_fatfs.h"
#include <stdio.h>
#include <string.h>
#include "i2s.h"
#include "pcm.h"
#include "tm_stm32f4_usb_msc_host.h"
#include <stdio.h>
#include "timer.h"
#include "glcd.h"
#include "mp3.h"
#include "file.h"
/* Notes */
/* - needs to add USB drivers
 * - types.h  -> select.h commented out !! */
/* - use of 0: and 1: for partionions! */



int main(void)
{
	int i;
	unsigned int redraw = 0;
	char buff[20];
	char x = 0;
	SystemInit();
	Gpio_Init();
	PreampInit();
	TM_USART_Init(UART4, TM_USART_PinsPack_1, 115200);
	MenuInit();
	Cs43Init();
	TM_DELAY_Init();
	TM_USB_MSCHOST_Init();
	TIMER3_Configuration(2398);
	GLCD_Init();

	Mp3MountDevices();

	scan_files("0:");

    while(1)
    {
    	TM_USB_MSCHOST_Process();
    	BTBtnsProc();
    	MenuProc();
    	redraw++;


    	/*if(Mp3Play("a.mp3",partition_sd) == false)
    	{
    		//Unmount drive, don't forget this!
    		f_mount(0, "0:", 1);
    	}*/

    }
}


