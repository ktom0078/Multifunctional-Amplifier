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
/* Notes */
/* - needs to add USB drivers
 * - types.h  -> select.h commented out !! */
/* - use of 0: and 1: for partionions! */

int main(void)
{
	FATFS USB_Fs;
	FIL USB_Fil;
	char buffer[50];
	unsigned char usb_write = 1, sd_write = 1;
	unsigned int free, total;

	SystemInit();
	Gpio_Init();
	PreampInit();
	TM_USART_Init(UART4, TM_USART_PinsPack_1, 115200);
	MenuInit();
	Cs43Init();
	TM_DELAY_Init();
	TM_USB_MSCHOST_Init();

    /*
    if (f_mount(&FatFs, "", 1) == FR_OK)
    {
    	PcmPlay("a.raw");
        //Unmount drive, don't forget this!
        f_mount(0, "", 1);
    }*/

    while(1)
    {
    	TM_USB_MSCHOST_Process();
    	BTBtnsProc();
    	MenuProc();

        if (TM_USB_MSCHOST_Device() == TM_USB_MSCHOST_Result_Connected) {
            /* If we didn't write data already */
            if (usb_write) {
				/* Try to mount USB device */
				/* USB is at 1: */
				if (f_mount(&USB_Fs, "1:", 1) == FR_OK) {
					/* Mounted ok */
					/* Try to open USB file */
					if (f_open(&USB_Fil, "1:asd.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {
						/* We want to write only once */
						usb_write = 0;

						/* Get total and free space on USB */
						TM_FATFS_USBDriveSize(&total, &free);

						/* Put data */
						f_puts("This is my first file with USB and FatFS\n", &USB_Fil);
						f_puts("with USB MSC HOST library from stm32f4-discovery.com\n", &USB_Fil);
						f_puts("----------------------------------------------------\n", &USB_Fil);
						f_puts("USB total and free space:\n\n", &USB_Fil);
						f_puts("----------------------------------------------------\n", &USB_Fil);
						/* Close USB file */
						f_close(&USB_Fil);
					}
				}
				/* Unmount USB */
				f_mount(0, "1:", 1);
            }
        } else {
            /* Ready to write next time */
            usb_write = 1;
        }
    }
}


