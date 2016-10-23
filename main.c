#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "system_stm32f4xx.h"
#include "stm32f4xx.h"
#include "i2c.h"
#include "gpio.h"
#include "stm32f4xx_i2c.h"
#include "preamp.h"
#include "tm_stm32f4_rotary_encoder.h"

TM_RE_t RE1_Data;
TM_RE_t RE2_Data;
unsigned char volume = 30;
TM_RE_Rotate_t rotate = TM_RE_Rotate_Nothing;

int main(void)
{
	SystemInit();
	Gpio_Init();
	PreampInit();

	PreampSetVol(volume);
	PreampSetSpeakAtt(FrontRight, 20);
	PreampSetInputGain(RCA,3);
	PreampSetBass(15);
	PreampSetTreble(10);

	I2C_stop(I2C1); // stop the transmission

	/* Init ro enc, pin A = PC13, pin B = PC15 */
	TM_RE_Init(&RE1_Data, GPIOC, GPIO_PIN_13, GPIOC, GPIO_PIN_15);


    while(1)
    {
    	rotate = TM_RE_Get(&RE1_Data);

    	if(RE1_Data.Diff > 0)
    	{
        	I2C_start(I2C1, TDA7318_I2C_ADDRESS, I2C_Direction_Transmitter);
        	PreampSetVol(++volume);
        	I2C_stop(I2C1); // stop the transmission
    	}
    	else if(RE1_Data.Diff < 0)
    	{
    		I2C_start(I2C1, TDA7318_I2C_ADDRESS, I2C_Direction_Transmitter);
    		PreampSetVol(--volume);
    		I2C_stop(I2C1); // stop the transmission
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
    if (GPIO_Pin == RE2_Data.GPIO_PIN_A) {
        /* Process data */
        TM_RE_Process(&RE2_Data);
    }
}
