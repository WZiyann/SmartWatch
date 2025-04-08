/*--------------------------------------------------------------------------
温度传感器DS18B20

V1.0
(1)只要定义接口即可(DS18B20_PORT DS18B20_PIN)
(2)使用前必须初始化u8 DS18B20_Init(void);
(3)读取温度值short DS18B20_Get_Temp(void);


Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"
#include "delay.h"


//定义DS18B20接口
#define DS18B20_PORT    GPIOB
#define DS18B20_PIN     GPIO_Pin_3


//////////////////////////////////////////////////////////////////////////////////

//IO方向设置
#define DS18B20_IO_IN()  {GPIO_Config(DS18B20_PORT,DS18B20_PIN,INPUT_UP);}
#define DS18B20_IO_OUT() {GPIO_Config(DS18B20_PORT,DS18B20_PIN,OUTPUT_UP);}

//返回1:不存在；返回0:存在
u8 DS18B20_Init(void);			//初始化DS18B20
short DS18B20_Get_Temp(void);	//获取温度(精度：0.1C)
   
#endif















