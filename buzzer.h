/*--------------------------------------------------------------------------
buzzer��������������

V1.0
(1)ʹ��PB7����������


Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#ifndef __BUZZER_H
#define __BUZZER_H	 
#include "sys.h"	
#include "delay.h"

//LED�˿ڶ���
#define buzzer PBout(7)

void Buzzer_Init(void);	//��ʼ��	

void SetBeep(u8 _sta);
void Beep(u16 _ms);


#endif

