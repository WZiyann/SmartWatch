/*--------------------------------------------------------------------------
led��������

V1.0
(1)ʹ��PC13����LED�ƣ���Ϊ��ʾ��������


Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#include "led.h"
#include "delay.h"
	    
//LED IO��ʼ��
void LED_Init(void)
{
	RCC->APB2ENR|=1<<4;    //ʹ��PORTCʱ��	

	GPIOC->CRH&=0XFF0FFFFF; 
	GPIOC->CRH|=0X00300000;	//PC13�������   	 
	GPIOC->ODR|=1<<13;    	//PC13�����	   
}

