#include "timer.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"

void Tim2Init(unsigned short int period ) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = TIM2_PRESCALER_VAL;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = period;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);

    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
}

void Tim2Start()
{
	TIM_Cmd(TIM2, ENABLE); //start
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void Tim2Stop()
{
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM2, DISABLE); //stop
}


