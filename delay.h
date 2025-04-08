/*--------------------------------------------------------------------------
systick驱动程序

V1.0
(1)提供了两个软延时函数
    void delay_ms(u16 nms);
    void delay_us(u32 nus);
(2)提供了IO口快速配置数字输入输出函数
    GPIO_Config(GPIOA,GPIO_Pin_11,INPUT_FLOAT);//数字输入
(3)提供全局时基变量
    u32 GetTick(void);
(4)提供定时中断服务
    void SysTick_Handler(void) {}

Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h" 

//定义中断频率
#define delay_ostickspersec 100//10ms


typedef enum
{ 
    Bit_RESET = 0,
    Bit_SET
}BitAction;


//INPUTorOUTPUT
typedef enum{
    INPUT_UP    = 0x01,
    INPUT_DOWN  = 0x02,
    INPUT_FLOAT = 0x00,
    OUTPUT_UP   = 0xF1,
    OUTPUT_DOWN = 0xF2,   
}INPUTorOUTPUT_e;


//GPIO_Pin_x
typedef enum {
    GPIO_Pin_0 = ((uint16_t)0x0001),
    GPIO_Pin_1 = ((uint16_t)0x0002),
    GPIO_Pin_2 = ((uint16_t)0x0004),
    GPIO_Pin_3 = ((uint16_t)0x0008), 
    GPIO_Pin_4 = ((uint16_t)0x0010), 
    GPIO_Pin_5 = ((uint16_t)0x0020), 
    GPIO_Pin_6 = ((uint16_t)0x0040), 
    GPIO_Pin_7 = ((uint16_t)0x0080), 
    GPIO_Pin_8 = ((uint16_t)0x0100),
    GPIO_Pin_9 = ((uint16_t)0x0200),
    GPIO_Pin_10= ((uint16_t)0x0400),
    GPIO_Pin_11 = ((uint16_t)0x0800),
    GPIO_Pin_12 = ((uint16_t)0x1000), 
    GPIO_Pin_13 = ((uint16_t)0x2000), 
    GPIO_Pin_14 = ((uint16_t)0x4000), 
    GPIO_Pin_15 = ((uint16_t)0x8000),  
    GPIO_Pin_All= ((uint16_t)0xFFFF),   
}GPIO_Pin_e;
    
void delay_Init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

void GPIO_Config(GPIO_TypeDef* GPIOx,GPIO_Pin_e GPIO_Pin_x,INPUTorOUTPUT_e INPUTorOUTPUT);//配置输入输出上拉下拉 

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//置位IO口
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//清零IO口

uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//读取IO口状态
u32 GetTick(void);

#endif


