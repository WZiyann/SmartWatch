/*--------------------------------------------------------------------------
key按键扫描

V1.2
(1)短按 --> 0x01~0x08
(2)长按 --> 0xF1~0xF8


Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#include "key_s.h"
#include "delay.h"
  
								    
//按键初始化函数
void KEY_Init(void)
{
    GPIO_Config(GPIOA,GPIO_Pin_4,INPUT_UP);
    GPIO_Config(GPIOA,GPIO_Pin_5,INPUT_UP);
    GPIO_Config(GPIOA,GPIO_Pin_6,INPUT_UP);
    #if KEYNUM>=4
    GPIO_Config(GPIOA,GPIO_Pin_7,INPUT_UP);
    #endif
    #if KEYNUM>=5
    GPIO_Config(GPIOB,GPIO_Pin_0,INPUT_UP);
    #endif
    #if KEYNUM>=6
    GPIO_Config(GPIOB,GPIO_Pin_1,INPUT_UP);
    #endif
    #if KEYNUM>=7
    GPIO_Config(GPIOB,GPIO_Pin_10,INPUT_UP);
    #endif 
    #if KEYNUM>7
    GPIO_Config(GPIOB,GPIO_Pin_11,INPUT_UP);
    #endif    
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY1按下
//2，KEY2按下
//3，KEY3按下 
//4，KEY4按下
//注意此函数有响应优先级,KEY1>KEY2>KEY3>KEY4!!
#define HOLD_TIME 100
u8 KeyScan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
    static u16 t1=0;
    static u16 t2=0;
    static u16 t3=0;
    #if KEYNUM>=4
    static u16 t4=0;
    #endif
    #if KEYNUM>=5
    static u16 t5=0;
    #endif
    #if KEYNUM>=6
    static u16 t6=0;
    #endif
    #if KEYNUM>=7
    static u16 t7=0;
    #endif
    #if KEYNUM>=8
    static u16 t8=0;
    #endif    
    u8 key = 0;

    if(KEY1==1&&KEY2==1&&KEY3==1
    #if KEYNUM>=4
        &&KEY4==1
    #endif
    #if KEYNUM>=5
        &&KEY5==1
    #endif
    #if KEYNUM>=6
        &&KEY6==1
    #endif
    #if KEYNUM>=7
        &&KEY7==1
    #endif
    #if KEYNUM>=8
        &&KEY8==1
    #endif    
    )//高电平
    {
        if(key_up==0)//没有触发长按键,标记为短按键事件
        {
            if(t1>0)key = 0x01;
            if(t2>0)key = 0x02;
            if(t3>0)key = 0x03;
            #if KEYNUM>=4
            if(t4>0)key = 0x04;
            #endif
            #if KEYNUM>=5
            if(t5>0)key = 0x05;
            #endif
            #if KEYNUM>=6
            if(t6>0)key = 0x06;
            #endif
            #if KEYNUM>=7
            if(t7>0)key = 0x07;
            #endif
            #if KEYNUM>=8
            if(t8>0)key = 0x08;
            #endif            
        }            
        t1=0;
        t2=0;
        t3=0;
        #if KEYNUM>=4
        t4=0;
        #endif
        #if KEYNUM>=5
        t5=0;
        #endif
        #if KEYNUM>=6
        t6=0;
        #endif
        #if KEYNUM>=7
        t7=0;
        #endif
        #if KEYNUM>=8
        t8=0;
        #endif        
        key_up=0;
    }
    else//低电平
    {
        delay_ms(10);//去抖动
        if(KEY1==0)t1++;
        if(KEY2==0)t2++;
        if(KEY3==0)t3++;
        #if KEYNUM>=4
        if(KEY4==0)t4++;
        #endif
        #if KEYNUM>=5
        if(KEY5==0)t5++;
        #endif
        #if KEYNUM>=6
        if(KEY6==0)t6++;
        #endif
        #if KEYNUM>=7
        if(KEY7==0)t7++;
        #endif
        #if KEYNUM>=8
        if(KEY8==0)t8++;
        #endif        
        if(key_up==0)//首次识别为长按键
        {
            if(t1>HOLD_TIME){key_up=1;key = 0xF1;}
            if(t2>HOLD_TIME){key_up=1;key = 0xF2;}
            if(t3>HOLD_TIME){key_up=1;key = 0xF3;}
            #if KEYNUM>=4
            if(t4>HOLD_TIME){key_up=1;key = 0xF4;}
            #endif
            #if KEYNUM>=5
            if(t5>HOLD_TIME){key_up=1;key = 0xF5;}
            #endif
            #if KEYNUM>=6
            if(t6>HOLD_TIME){key_up=1;key = 0xF6;}
            #endif
            #if KEYNUM>=7
            if(t7>HOLD_TIME){key_up=1;key = 0xF7;}
            #endif
            #if KEYNUM>=8
            if(t8>HOLD_TIME){key_up=1;key = 0xF8;}
            #endif            
        }
    }
    
 	return key;// 无按键按下
}




















