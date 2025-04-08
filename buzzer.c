/*--------------------------------------------------------------------------
buzzer��������������

V1.0
(1)ʹ��PB7����������


Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#include "buzzer.h"

void Buzzer_Init(void)
{
	//PB3��PB4��ӳ��                                                                                                                                                                                                            
	RCC->APB2ENR|= 0x00000001;//AFIOEN = 1;��������IOʱ��ʹ��
	AFIO->MAPR 	|= 0x02000000;//����AFIO_MAPR,��SWJ_CFG[2:0]λΪ010���ر�JTAG-DP������SW-DP
    
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





