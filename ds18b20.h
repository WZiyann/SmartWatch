/*--------------------------------------------------------------------------
�¶ȴ�����DS18B20

V1.0
(1)ֻҪ����ӿڼ���(DS18B20_PORT DS18B20_PIN)
(2)ʹ��ǰ�����ʼ��u8 DS18B20_Init(void);
(3)��ȡ�¶�ֵshort DS18B20_Get_Temp(void);


Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"
#include "delay.h"


//����DS18B20�ӿ�
#define DS18B20_PORT    GPIOB
#define DS18B20_PIN     GPIO_Pin_3


//////////////////////////////////////////////////////////////////////////////////

//IO��������
#define DS18B20_IO_IN()  {GPIO_Config(DS18B20_PORT,DS18B20_PIN,INPUT_UP);}
#define DS18B20_IO_OUT() {GPIO_Config(DS18B20_PORT,DS18B20_PIN,OUTPUT_UP);}

//����1:�����ڣ�����0:����
u8 DS18B20_Init(void);			//��ʼ��DS18B20
short DS18B20_Get_Temp(void);	//��ȡ�¶�(���ȣ�0.1C)
   
#endif















