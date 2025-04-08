#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

extern u8 heartRate;   
extern u8 hr_trig;

extern u32 TIM2CH4_T;
extern u8 TIM2CH4_trig;

void TIM1_Cap_PA11_Init(void);
void TIM2_Cap_PA3_Init(void);
void TIM3_PWM_PB1_Init(void);
void setPWM(u8 _pwm);
#endif























