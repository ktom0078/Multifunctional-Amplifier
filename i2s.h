#ifndef I2S_H
#define I2S_H


/* 44,1 kHZ = 135,5MHz / ((32*2*DIV+ODD)*8) */
#define I2S_PLLN	271
#define I2S_PLLR	2
#define I2S_DIV		6
#define I2S_ODD		0


void Cs43Init();

#endif
