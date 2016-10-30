#include "menu.h"
#include "attributes.h"
#include "tm_stm32f4_rotary_encoder.h"
#include "preamp.h"
#include "bt.h"

TM_RE_t RE1_Data;

void MenuInit()
{
	/* Init ro enc, pin A = PC13, pin B = PC15 */
	TM_RE_Init(&RE1_Data, GPIOC, GPIO_PIN_13, GPIOC, GPIO_PIN_15);
}


void MenuProc()
{
	/* Check if the state of the rotary enc changed */
	TM_RE_Get(&RE1_Data);

	if(RE1_Data.Diff > 0 || FlagVolDown == true)
	{
		/* Increasing volume variable lowers the volume */
    	PreampSetVol(++volume);

		/* Right direction, volume down */
		if(FlagVolDown == true)
		{
    		FlagVolDown = false;
    	}


	}
	else if(RE1_Data.Diff < 0 || FlagVolUp == true)
	{
		PreampSetVol(--volume);
    	if(FlagVolUp == true)
    	{
    		FlagVolUp = false;
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
