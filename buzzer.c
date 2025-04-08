/*--------------------------------------------------------------------------
buzzer蜂鸣器驱动程序

V1.0
(1)使用PB7驱动蜂鸣器


Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#include "buzzer.h"

void Buzzer_Init(void)
{
	//PB3、PB4重映射                                                                                                                                                                                                            
	RCC->APB2ENR|= 0x00000001;//AFIOEN = 1;辅助功能IO时钟使能
	AFIO->MAPR 	|= 0x02000000;//配置AFIO_MAPR,的SWJ_CFG[2:0]位为010：关闭JTAG-DP，启用SW-DP
    
    GPIO_Config(GPIOB,GPIO_Pin_7,OUTPUT_DOWN);
}
void SetBeep(u8 _sta)
{
    buzzer = _sta;
}
void Beep(u16 _ms)
{
	SetBeep(1);
	delay_ms(_ms);
	SetBeep(0);
	delay_ms(_ms);
}





