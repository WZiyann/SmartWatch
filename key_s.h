/*--------------------------------------------------------------------------
key����ɨ��

V1.2
(1)�̰� --> 0x01~0x08
(2)���� --> 0xF1~0xF8


Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEYNUM 4

#define KEY1	PAin(4) 
#define KEY2	PAin(5)	 
#define KEY3	PAin(6) 
#define KEY4	PAin(7)	
#define KEY5	PBin(0) 
#define KEY6	PBin(1)	 
#define KEY7	PBin(10) 
#define KEY8	PBin(11)

void KEY_Init(void);//IO��ʼ��
u8 KeyScan(u8);  	//����ɨ�躯��					    
#endif
