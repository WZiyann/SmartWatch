/*--------------------------------------------------------------------------
systick��������

V1.0
(1)�ṩ����������ʱ����
    void delay_ms(u16 nms);
    void delay_us(u32 nus);
(2)�ṩ��IO�ڿ����������������������
    GPIO_Config(GPIOA,GPIO_Pin_11,INPUT_FLOAT);//��������
(3)�ṩȫ��ʱ������
    u32 GetTick(void);
(4)�ṩ��ʱ�жϷ���
    void SysTick_Handler(void) {}

Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h" 

//�����ж�Ƶ��
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

void GPIO_Config(GPIO_TypeDef* GPIOx,GPIO_Pin_e GPIO_Pin_x,INPUTorOUTPUT_e INPUTorOUTPUT);//������������������� 

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//��λIO��
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//����IO��

uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//��ȡIO��״̬
u32 GetTick(void);

#endif


