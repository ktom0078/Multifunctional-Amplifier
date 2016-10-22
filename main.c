#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "system_stm32f4xx.h"
#include "stm32f4xx.h"
#include "i2c.h"
#include "gpio.h"
#include "stm32f4xx_i2c.h"
#include "preamp.h"
int main(void)
{
	SystemInit();
	Gpio_Init();
	PreampInit();

	PreampSetVol(30);
	PreampSetSpeakAtt(FrontRight, 20);
	PreampSetInputGain(RCA,3);
	PreampSetBass(15);
	PreampSetTreble(15);

	I2C_stop(I2C1); // stop the transmission


    while(1)
    {
    }
}
