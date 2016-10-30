#include "preamp.h"
#include "i2c.h"
#include "gpio.h"
#include "attributes.h"
#include "tm_stm32f4_rotary_encoder.h"

#define PREAMP_GENERATE_START  I2C_start(I2C1, TDA7318_I2C_ADDRESS, I2C_Direction_Transmitter)
#define PREAMP_GENERATE_STOP   I2C_stop(I2C1) // stop the transmission

/* global variables */
unsigned char volume = PREAMP_STARTUP_VOL;
unsigned char bass   = PREAMP_STARTUP_BASS;
unsigned char treble = PREAMP_STARTUP_TREBLE;

/* Inits the tda7318 preamp ic */
void PreampInit()
{
	I2C_Init_Amp();
	GPIO_WriteBit(I2C_PORT,I2C_OE_PIN,Bit_SET); // enable i2c comms

	PreampSetVol(volume);
	PreampSetSpeakAtt(FrontRight, 0);
	PreampSetInputGain(BT,0);
	PreampSetBass(bass);
	PreampSetTreble(treble);

}

/* Sets the volume on preamp, input val max: 0 - min : 63 */
void PreampSetVol(unsigned char vol)
{
	if(vol >= 0 && vol <= 63)
	{
		PREAMP_GENERATE_START;
		I2C_write(I2C1, vol); //set main vol
		PREAMP_GENERATE_STOP;
	}
}

/* Sets attuneatation on the selected speaker, input val: speaker, attuneatation min:0 -  max: 33 */
void PreampSetSpeakAtt(ePreampSpeaker spkr, unsigned char att)
{
	if(att >=0 && att <=31)
	{
		PREAMP_GENERATE_START;
		I2C_write(I2C1, ((((unsigned char)spkr) << 5) |  att));
		PREAMP_GENERATE_STOP;
	}
}

/* Sets audio input and gain: 0 - max, 3 - min */
void PreampSetInputGain(eAudioInput input,unsigned char gain)
{
	if(gain >= 0 && gain <= 3)
	{
		PREAMP_GENERATE_START;
		I2C_write(I2C1, PREAMP_AUDIO_SW_CMD | (gain << 3) | (unsigned char)input);
		PREAMP_GENERATE_STOP;
	}
}

/* Sets the bass level, input bass : 0 - min, 15 - max */
void PreampSetBass(unsigned char bass)
{
	if(bass >= 0 && bass <= 15)
	{
		/* increasing in 0 -> 7 and next 15 -> 8 , replace it to 0 -> 15 */
		if(bass >= 8)
		{
			bass = 8 + (15 - bass);
		}
		PREAMP_GENERATE_START;
		I2C_write(I2C1, PREAMP_BASS_CMD | bass);
		PREAMP_GENERATE_STOP;
	}
}

void PreampSetTreble(unsigned char treble)
{
	if(treble >= 0 && treble <= 15)
	{
		/* increasing in 0 -> 7 and next 15 -> 8 , replace it to 0 -> 15 */
		if(treble >= 8)
		{
			treble = 8 + (15 - treble);
		}
		PREAMP_GENERATE_START;
		I2C_write(I2C1, PREAMP_TREBLE_CMD | treble);
		PREAMP_GENERATE_STOP;
	}
}

