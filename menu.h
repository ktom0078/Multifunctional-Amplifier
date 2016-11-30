#ifndef MENU_H
#define MENU_H

#include "attributes.h"
#include "preamp.h"

#define MENU_AUDIO_SETTINGS_NUM	4

typedef struct
{
	unsigned char* value;
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
	StMenuInit,
	StMenuMain

}eMenuStates;


void MenuInit();
void MenuProc();
bool RotSwPRessed();
bool ButtonPRessed();
void ListProc();
void MainMenuProc();
void ChangeToList();
void ChangeToMain();
void RemoveId3();
void PutId3();


#endif
