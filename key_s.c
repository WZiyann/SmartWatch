/*--------------------------------------------------------------------------
key����ɨ��

V1.2
(1)�̰� --> 0x01~0x08
(2)���� --> 0xF1~0xF8


Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#include "key_s.h"
#include "delay.h"
  
								    
//������ʼ������
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
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY1����
//2��KEY2����
//3��KEY3���� 
//4��KEY4����
//ע��˺�������Ӧ���ȼ�,KEY1>KEY2>KEY3>KEY4!!
#define HOLD_TIME 100
u8 KeyScan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
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
    )//�ߵ�ƽ
    {
        if(key_up==0)//û�д���������,���Ϊ�̰����¼�
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
    else//�͵�ƽ
    {
        delay_ms(10);//ȥ����
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
        if(key_up==0)//�״�ʶ��Ϊ������
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
    
 	return key;// �ް�������
}




















