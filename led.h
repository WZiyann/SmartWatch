/*--------------------------------------------------------------------------
led驱动程序

V1.0
(1)使用PC13驱动LED灯，作为提示程序运行


Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#ifndef __LED_H
#define __LED_H	 
#include "sys.h"


//LED端口定义
#define LED PCout(13)	// DS0

void LED_Init(void);	//初始化	

#endif

















