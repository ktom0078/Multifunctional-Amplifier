#include "menu.h"
#include "attributes.h"
#include "tm_stm32f4_rotary_encoder.h"
#include "preamp.h"
#include "bt.h"
#include "gpio.h"
#include "timer.h"

/* Local variables  */
static TM_RE_t RE1_Data;
static bool RotSwPress = false;
static unsigned char menuindex;
static tMenuAudioSettings MenuAudioSettings[MENU_AUDIO_SETTINGS_NUM] =
{
/* 		value						callback */
		{PREAMP_STARTUP_VOL, 		PreampSetVol},
		{PREAMP_STARTUP_TREBLE,		PreampSetTreble},
		{PREAMP_STARTUP_BASS, 		PreampSetBass}
};
unsigned char presscnt = 0;

volatile unsigned char x = 0;
void MenuInit()
{
	/* Init ro enc, pin A = PC13, pin B = PC15 */
	TM_RE_Init(&RE1_Data, GPIOC, GPIO_PIN_13, GPIOC, GPIO_PIN_15);

	/* init timer for debouncing */
	Tim2Init(40);
}


void MenuProc()
{

	/* Check if the state of the rotary enc changed */
	TM_RE_Get(&RE1_Data);

	if(RotSwPRessed())
	{
		menuindex++;
		if(menuindex == MENU_AUDIO_SETTINGS_NUM)
		{
			menuindex = 0;
		}
	}

	if(RE1_Data.Diff > 0)
	{
		MenuAudioSettings[menuindex].Cb(&(MenuAudioSettings[menuindex].value),Decrease);

		/* Right direction, volume down */
		if(FlagVolDown == true)
		{
    		FlagVolDown = false;
    	}
	}
	else if(RE1_Data.Diff < 0)
	{
		MenuAudioSettings[menuindex].Cb(&(MenuAudioSettings[menuindex].value),Increase);

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
    if(GPIO_Pin == ROT_SW_PIN)
    {
    	/* start the timer to debounce */
		Tim2Start();
    }
}

bool RotSwPRessed()
{
	bool retval = RotSwPress;
	/* reset its value when we used it */
	if(RotSwPress) RotSwPress = false;

	return retval;
}

void TIM2_IRQHandler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if(GPIO_ReadInputDataBit(ROT_PORT,ROT_SW_PIN) == Bit_RESET)
		{
			RotSwPress = true;
		}
		Tim2Stop();
	}
}
