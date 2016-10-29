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

TM_RE_t RE1_Data;
unsigned char volume = PREAMP_STARTUP_VOL;
char buff[30];
volatile unsigned char asd = 0;

int main(void)
{
	SystemInit();
	Gpio_Init();
	PreampInit();

	TM_USART_Init(UART4, TM_USART_PinsPack_1, 115200);

	//Gpio_BT_Enable_Cmd();

	/* Init ro enc, pin A = PC13, pin B = PC15 */
	TM_RE_Init(&RE1_Data, GPIOC, GPIO_PIN_13, GPIOC, GPIO_PIN_15);

    while(1)
    {
    	if(TM_USART_Gets(UART4,buff,sizeof(buff)))
    	{
    		/* Test code for breakpoint */
    		asd++;
    	}

    	TM_RE_Get(&RE1_Data);

    	if(RE1_Data.Diff > 0)
    	{
        	PreampSetVol(++volume);
    	}
    	else if(RE1_Data.Diff < 0)
    	{
    		PreampSetVol(--volume);
    	}
    }
}

/* TM EXTI Handler for all EXTI lines */
void TM_EXTI_Handler(uint16_t GPIO_Pin) {
    /* Check RE pin 1 */
    if (GPIO_Pin == RE1_Data.GPIO_PIN_A) {
        /* Process data */
        TM_RE_Process(&RE1_Data);
    }
}
