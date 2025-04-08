/*--------------------------------------------------------------------------
buzzer蜂鸣器驱动程序

V1.0
(1)使用PB7驱动蜂鸣器


Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#ifndef __BUZZER_H
#define __BUZZER_H	 
#include "sys.h"	
#include "delay.h"

//LED端口定义
#define buzzer PBout(7)

void Buzzer_Init(void);	//初始化	

void SetBeep(u8 _sta);
void Beep(u16 _ms);


#endif

