#ifndef MENU_H
#define MENU_H

#include "attributes.h"
#include "preamp.h"

#define MENU_AUDIO_SETTINGS_NUM	3

typedef struct
{
	unsigned char value;
	void (*Cb)(unsigned char*, ePreampAction);
}tMenuAudioSettings;

typedef enum
{
	ivolume = 0,
	itreble,
	ibass
}eMenuAudioSettings;

typedef enum
{
	Settings,
	Main,
	List
}eMenuScreen;

typedef enum
{
	ListInit,
	ListMain

}eListStates;


void MenuInit();
void MenuProc();
bool RotSwPRessed();
bool ButtonPRessed();
void ListProc();



#endif
