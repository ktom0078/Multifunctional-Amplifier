#include "menu.h"
#include "attributes.h"
#include "tm_stm32f4_rotary_encoder.h"
#include "preamp.h"
#include "bt.h"
#include "gpio.h"
#include "timer.h"
#include "glcd.h"
#include "mp3.h"

/* Local variables  */
static eMenuScreen MenuScreen;
static eListStates ListState;
static TM_RE_t RE1_Data;
static bool RotSwPress = false;
static bool ButtonPress = false;
static bool overflow = true;
static unsigned char menuindex;
static char PageIndex;
static tMenuAudioSettings MenuAudioSettings[MENU_AUDIO_SETTINGS_NUM] =
{
/* 		value						callback */
		{PREAMP_STARTUP_VOL, 		PreampSetVol},
		{PREAMP_STARTUP_TREBLE,		PreampSetTreble},
		{PREAMP_STARTUP_BASS, 		PreampSetBass}
};
unsigned char presscnt = 0;
volatile unsigned char x = 0;

#define ROTARY_RIGHT 	(RE1_Data.Diff > 0)
#define ROTARY_LEFT 	(RE1_Data.Diff < 0)
#define PAGE_INDEX_MAX	(Mp3Count != 0 ? (Mp3Count / 8) :0)
#define PAGE_INDEX_MIN  0

void MenuInit()
{
	/* Init ro enc, pin A = PC13, pin B = PC15 */
	TM_RE_Init(&RE1_Data, GPIOC, GPIO_PIN_13, GPIOC, GPIO_PIN_15);

	/* init timer for debouncing */
	Tim2Init(40);
	Tim4Init(40);

	/* Start with the list screen */
	MenuScreen = List;
	ListState = ListInit;

	PageIndex = 0;
}


void MenuProc()
{

	/* Check if the state of the rotary enc changed */
	TM_RE_Get(&RE1_Data);

	switch(MenuScreen)
	{
	case List:
		ListProc();
		break;
	case Settings:
		break;
	case Main:
		break;
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
    if(GPIO_Pin == BUTTON_PIN)
    {
    	/* start the timer to debounce */
    	Tim4Start();
    }
}

bool RotSwPRessed()
{
	bool retval = RotSwPress;
	/* reset its value when we used it */
	if(RotSwPress) RotSwPress = false;

	return retval;
}

bool ButtonPRessed()
{
	bool retval = ButtonPress;
	/* reset its value when we used it */
	if(ButtonPress) ButtonPress = false;

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

void TIM4_IRQHandler()
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		if(GPIO_ReadInputDataBit(BUTTON_PORT,BUTTON_PIN) == Bit_RESET)
		{
			ButtonPress = true;
		}
		Tim4Stop();
	}
}

void ListProc()
{
	static unsigned char ListIndex;
	unsigned int i,j,page;
    char* filename;
    char* title;
    char buff[20];
    bool redraw = false;


	switch(ListState)
	{
	case ListInit:
		GLCD_Clear();

		if(overflow)
		{
			ListIndex = 0;
		}
		else
		{
			ListIndex = 7;
		}

		ListState = ListMain;

		GLCD_WriteString(">",0,ListIndex);
		/* Put the list from the tracks */
		j=0;
		page = PageIndex*8;
		for(i=page;(i < Mp3Count) && (i < page + 8);i++)
		{
			filename = (char*)(&(Mp3Array[i].Path));
			title    = (char*)(&(Mp3Array[i].Title));

			if(title[0] == 0)
			{
				/* Skip the drive data by adding 2 */
				GLCD_WriteString(&filename[2],6,j++);
			}
			else
			{
				GLCD_WriteString(title,6,j++);
			}
		}
		break;
	case ListMain:
		if(ROTARY_LEFT)
		{
			if(ListIndex == 0 && PageIndex > PAGE_INDEX_MIN)
			{
				/* Draw a new page */
				PageIndex--;
				ListState = ListInit;
				overflow = false;
			}
			else if(ListIndex > 0)
			{
				/* Remove the old */
				GLCD_WriteString(" ",0,ListIndex);
				ListIndex--;
				/* Redraw only the index icon */
				redraw = true;
			}
		}
		else if(ROTARY_RIGHT)
		{
			if(ListIndex == 7 && PageIndex < PAGE_INDEX_MAX)
			{
				/* Draw a new page */
				PageIndex++;
				ListState = ListInit;
				overflow = true;
			}
			else if(ListIndex < 7 && ((PageIndex * 8) + ListIndex) < Mp3Count)
			{
				/* Remove the old */
				GLCD_WriteString(" ",0,ListIndex);
				ListIndex++;
				/* Redraw only the index icon */
				redraw = true;
			}
		}

		if(redraw)
		{
			redraw = false;
			GLCD_WriteString(">",0,ListIndex);
		}

		if(RotSwPRessed())
		{
			Mp3ChangeTrack((PageIndex * 8) + ListIndex);
		}
		break;
	}
}
