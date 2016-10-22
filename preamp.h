#ifndef PREAMP_H
#define PREAMP_H

#define PREAMP_AUDIO_SW_CMD (1<<6)
#define PREAMP_BASS_CMD		(0x6 << 4)
#define PREAMP_TREBLE_CMD	(0x7 << 4)

/* definitions */
typedef enum
{
	FrontLeft = 0b100,
	FrontRight = 0b101,
	RearLeft = 0b110,
	RearRight = 0b111
}ePreampSpeaker;

typedef enum
{
	DAC_CS4334 = 0,
	RCA,
	BT,
}eAudioInput;


/* functions */
void PreampInit();
void PreampSetVol(unsigned char vol);
void PreampSetSpeakAtt(ePreampSpeaker spkr, unsigned char att);
void PreampSetInputGain(eAudioInput input,unsigned char gain);
void PreampSetBass(unsigned char bass);
void PreampSetTreble(unsigned char treble);

#endif
