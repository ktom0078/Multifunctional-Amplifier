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
    nvicStructure.NVIC_IRQChannelSubPriority = 0;
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

void TIMER3_Configuration(int duty)
{
	TIM_TimeBaseInitTypeDef  TIMx_TimeBaseStructure;
	TIM_OCInitTypeDef  TIMx_OCInitStructure;

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/**
	 * APB1 = 42MHz
	 * fpwm = TIM3_CLK/(Period+1)
	 * TIM3_CLK = APB1_CLK / (prescaler + 1)
	 * Duty_Cycle = (TIM_Pulse / Tim_Period)*100
	 */
	TIMx_TimeBaseStructure.TIM_Prescaler = 149;
	TIMx_TimeBaseStructure.TIM_Period = 2399;
	TIMx_TimeBaseStructure.TIM_ClockDivision = 0;
	TIMx_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIMx_TimeBaseStructure);

	TIMx_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIMx_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIMx_OCInitStructure.TIM_Pulse = duty;	// (2000/2399)*100=83%
	TIMx_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC3Init(TIM3, &TIMx_OCInitStructure);

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);

	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}

void Tim4Init(unsigned short int period ) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = TIM2_PRESCALER_VAL;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = period;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &timerInitStructure);

    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM4_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 4;
    nvicStructure.NVIC_IRQChannelSubPriority = 0;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
}
