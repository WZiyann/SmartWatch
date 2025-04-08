/*--------------------------------------------------------------------------
led驱动程序

V1.0
(1)使用PC13驱动LED灯，作为提示程序运行


Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#include "led.h"
#include "delay.h"
	    
//LED IO初始化
void LED_Init(void)
{
	RCC->APB2ENR|=1<<4;    //使能PORTC时钟	

	GPIOC->CRH&=0XFF0FFFFF; 
	GPIOC->CRH|=0X00300000;	//PC13推挽输出   	 
	GPIOC->ODR|=1<<13;    	//PC13输出高	   
}

