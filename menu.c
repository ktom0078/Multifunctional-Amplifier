#include "menu.h"
#include "attributes.h"
#include "tm_stm32f4_rotary_encoder.h"
#include "preamp.h"
#include "bt.h"
#include "gpio.h"
#include "timer.h"
#include "glcd.h"
#include "mp3.h"
#include "str.h"

/* Local variables  */
static eMenuScreen MenuScreen;
static eMenuStates ListState;
static eMenuStates MainState;
static TM_RE_t RE1_Data;
static bool RotSwPress = false;
static bool ButtonPress = false;
static bool overflow = true;
static bool MmenuSelected = false;
static unsigned char MmenuIndex;
static char PageIndex;
static tMenuAudioSettings MenuAudioSettings[MENU_AUDIO_SETTINGS_NUM] =
{
/* 		value										callback */
		{&AudioSettings.volume, 					PreampSetVol},
		{&AudioSettings.treble,						PreampSetTreble},
		{&AudioSettings.bass, 						PreampSetBass},
		{(unsigned char*)&AudioSettings.input,		PreampSetSourceWrapper}
};
unsigned char presscnt = 0;
volatile unsigned char x = 0;

#define ROTARY_RIGHT 	(RE1_Data.Diff > 0)
#define ROTARY_LEFT 	(RE1_Data.Diff < 0)
#define PAGE_INDEX_MAX	(Mp3Count != 0 ? (Mp3Count / 8) :0)
#define PAGE_INDEX_MIN  0
#define MMENU_INDEX_MIN 0
#define MMENU_INDEX_MAX 2


#define SourceString 	"Source:"
#define VolumeString  	"Attenuation:"
#define BassString  	"Bass:"
#define TrebleString  	"Treble:"

#define DAC_STRING		"DAC"
#define RCA_STRING		"RCA"
#define BT_STRING		"Bluetooth"

char* MenuStrings[] =
{
	VolumeString,TrebleString,BassString,SourceString
};

char* SourceStrings[] =
{
	DAC_STRING,RCA_STRING,BT_STRING
};


void MenuInit()
{
	/* Init ro enc, pin A = PC13, pin B = PC15 */
	TM_RE_Init(&RE1_Data, GPIOC, GPIO_PIN_13, GPIOC, GPIO_PIN_15);

	/* init timer for debouncing */
	Tim2Init(40);
	Tim4Init(40);

	/* Start with the list screen */
	MenuScreen = Main;
	ListState = StMenuInit;
	MainState = StMenuInit;

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
		MainMenuProc();
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
    bool redraw = false;


	switch(ListState)
	{
	case StMenuInit:
		GLCD_Clear();

		if(overflow)
		{
			ListIndex = 0;
		}
		else
		{
			ListIndex = 7;
		}

		ListState = StMenuMain;

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
	case StMenuMain:
		if(ROTARY_LEFT)
		{
			if(ListIndex == 0 && PageIndex > PAGE_INDEX_MIN)
			{
				/* Draw a new page */
				PageIndex--;
				ListState = StMenuInit;
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
				ListState = StMenuInit;
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

void RemoveFromIndex(unsigned char index)
{
	char buff[12],outbuff[12];
	if(index == 3)
	{
		sprintf(buff,"%s",SourceStrings[AudioSettings.input]);
	}
	else
	{
		sprintf(buff,"%d",(*MenuAudioSettings[index].value));
	}

	StrFillSpace(strlen(buff),outbuff);
	GLCD_WriteString(outbuff,6 + (6 * strlen(MenuStrings[index])),index);
}
void PutToIndex(unsigned char index)
{
	char buff[10];
	if(index == 3)
	{
		sprintf(buff,"%s",SourceStrings[AudioSettings.input]);
	}
	else
	{
		sprintf(buff,"%d",(*MenuAudioSettings[index].value));
	}
	GLCD_WriteString(buff,6 + (6 * strlen(MenuStrings[index])),index);
}

void MainMenuProc()
{
	bool redraw = false;
	char buff[12];
	unsigned char i;

	switch(MainState)
	{
	case StMenuInit:
		GLCD_Clear();
		MmenuIndex = 0;
		MmenuSelected = false;
		GLCD_WriteString(">",0,0);
		for(i=0;i<4;i++)
		{
			GLCD_WriteString(MenuStrings[i],6,i);
			if(i == 3)
			{
				sprintf(buff,"%s",SourceStrings[AudioSettings.input]);
			}
			else
			{
				sprintf(buff,"%d",*((char*)(((char*)(&AudioSettings))+i)));
			}
			GLCD_WriteString(buff,6 * strlen(MenuStrings[i]) + 6,i);

		}

		MainState = StMenuMain;


		break;
	case StMenuMain:
		if(MmenuSelected)
		{
			if(ROTARY_LEFT)
			{
				RemoveFromIndex(MmenuIndex);
				MenuAudioSettings[MmenuIndex].Cb((MenuAudioSettings[MmenuIndex].value),Increase);
				PutToIndex(MmenuIndex);
			}
			else if(ROTARY_RIGHT)
			{
				RemoveFromIndex(MmenuIndex);
				MenuAudioSettings[MmenuIndex].Cb((MenuAudioSettings[MmenuIndex].value),Decrease);
				PutToIndex(MmenuIndex);
			}

			if(ButtonPRessed())
			{
				MmenuSelected = false;
				/* Remove the old icon */
				GLCD_WriteString(" ",122,MmenuIndex);
				/* Draw the New */
				GLCD_WriteString(">",0,MmenuIndex);
			}
		}
		else
		{
			if(ROTARY_LEFT)
			{
				if(MmenuIndex > 0)
				{
					GLCD_WriteString(" ",0,MmenuIndex);
					MmenuIndex--;
					redraw = true;
				}
			}
			else if(ROTARY_RIGHT)
			{
				if(MmenuIndex < 3)
				{
					GLCD_WriteString(" ",0,MmenuIndex);
					MmenuIndex++;
					redraw = true;
				}

			}

			if(redraw)
			{
				redraw = false;
				GLCD_WriteString(">",0,MmenuIndex);
			}

			if(RotSwPRessed())
			{
				/* Remove the old icon */
				GLCD_WriteString(" ",0,MmenuIndex);
				/* Draw the New */
				GLCD_WriteString("<",122,MmenuIndex);

				MmenuSelected = true;
			}
		}
		break;
	}
}
