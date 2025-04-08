/*--------------------------------------------------------------------------
TIM驱动程序
Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#include "timer.h"
#include "buzzer.h"
#include "led.h"

void TIM1_Cap_PA11_Init(void)
{
    //配置TIM1_CH4,浮空输入，映射到PA11
    RCC->APB2ENR |= 1U<<11;     //TIM1 时钟使能
	GPIOA->CRH &= 0XFFFF0FFF;   //PA11 清除之前设置  
	GPIOA->CRH |= 0X00008000;	//PA11 浮空输入
	GPIOA->ODR |= 1<<11;		//PA11 上拉    

 	//配置TIM1时基，1MHz频率计数
    TIM1->ARR=0XFFFF;  		//设定计数器自动重装值   


    //配置时钟源    
    TIM1->SMCR &= (0x7<<0);//SMS[2:0]=000;//内部时钟CK_INT
    
    //配置捕获通道
    TIM1->CCMR2 &= 0x00FF;      //复位通道4上的配置
	TIM1->CCMR2 |= 1U<<(0+8);   //CC4S=01 	选择输入端 IC1映射到TI1上
 	TIM1->CCMR2 |= 0U<<(4+8);   //IC4F=0000 配置输入滤波器 不滤波
    TIM2->CCMR2 |= (0xC<<(4+8));   //IC4F=1100,f_SAMPLING=f_DTS/16, N=8
 	TIM1->CCMR2 |= 0U<<(2+8);   //IC4PSC=00 	配置输入分频,不分频 

    TIM1->CCER &= ~(0U<<13); 		//CC1P=0	上升沿捕获   
	TIM1->CCER |= 1U<<12; 		//CC4E=1 	允许捕获计数器的值到捕获寄存器中

    TIM1->DIER = 0X0000;        //复位，关闭所有DMA请求和中断请求			
	TIM1->DIER |= 1U<<0;        //UIE = 1;//开更新中断服务请求
	TIM1->DIER |= 1U<<4;        //CC4IE = 1;//开通道4的捕获/比较中断服务请求	

	MY_NVIC_Init(2,0,TIM1_UP_IRQn,2);//抢占2，子优先级0，组2
    TIM1->CR1 |= (0x1<<0);    	//CEN = 1; 启动定时器
}

//____      _____________      ________________
//    |    |             |    |     
//    |____|             |____| 
//
//计算2次下降沿的周期
static u8  TIM1CH4_CAPTURE_STA=0;	//输入捕获状态		    				
static u16	TIM1CH4_CAPTURE_VAL;	//输入捕获值
static u32 TIM1CH4_CAPTURE_T;
u8 heartRate=0;
u8 hr_trig=0;
//定时器5中断服务程序
void TIM1_UP_IRQHandler(void)
{
    if(TIM1->SR & 0X0001)//更新中断if(UIF=1)
    {
        TIM1->SR &= ~(1U<<0);//清除更新中断标志位UIF=0;
        TIM1CH4_CAPTURE_STA ++;//记录溢出的次数
    } 
    //TIM1->SR=0;//清除中断标志位
}
void TIM1_TRG_COM_IRQHandler(void)
{
    if(TIM1->SR&0x0020)//COMIF = 1;
    {
        TIM1->SR &= ~(1U<<6);
    }
    if(TIM1->SR&0x0040)//TIF = 1
    {
        TIM1->SR &= ~(1U<<5);
    }
}
void TIM1_CC_IRQHandler(void)
{ 		    
    if(TIM1->SR&0X0010)//比较/捕获通道4中断if(CC4IF=1)
    {
        TIM1->SR &= ~(1U<<4);//清除触发中断标志位CC4IF=0;
        TIM1->SR &= ~(1U<<12);//清除重复捕获标记CC4OF=0
        
        //记录一次捕获时间
        TIM1CH4_CAPTURE_VAL = TIM1->CCR4;
        TIM1CH4_CAPTURE_T = TIM1CH4_CAPTURE_STA*0XEEEE + TIM1CH4_CAPTURE_VAL;
        
        TIM1CH4_CAPTURE_STA = 0;
        TIM1->CNT = 0;
         
        //计算相邻下降沿时长
        TIM1CH4_CAPTURE_T +=5;
        TIM1CH4_CAPTURE_T /=10;//单位ms
        if(TIM1CH4_CAPTURE_T >= 500 && TIM1CH4_CAPTURE_T <= 3000)
        {

            heartRate = (u8)( ( (double)60000.0/TIM1CH4_CAPTURE_T)+0.5 );
            hr_trig++;

        }
    }    
}

//////////////////////////////////////////////////////////////////////////////////////////////
void TIM2_Cap_PA3_Init(void)
{
    //配置TIM2_CH4,浮空输入，映射到PA11
    //TIM1_REMAP[1:0]=00(没有重映射) TIM1_CH1~CH4 --> PA8~PA11
    RCC->APB1ENR |= 1U<<1;     //TIM
	GPIOA->CRL &= 0XFFFFFF0F;   //PA3清除之前设置  
	GPIOA->CRL |= 0X00008000;	//PA3 浮空输入
	GPIOA->ODR |= 1<<3;		//PA3 上拉    

 	//配置TIM1时基，1MHz频率计数
    TIM2->ARR=0XFFFF;  		//设定计数器自动重装值   

    //配置时钟源    
    TIM2->SMCR &= ~(0x7<<0);//SMS[2:0]=000;//内部时钟CK_INT
    
    //配置捕获通道
    TIM2->CCMR2 &= 0x00FF;      //复位通道4上的配置
	TIM2->CCMR2 |= 1U<<(0+8);   //CC4S=01 	选择输入端 IC1映射到TI1上
 	TIM2->CCMR2 |= 0U<<(4+8);   //IC4F=0000 配置输入滤波器 不滤波
    TIM2->CCMR2 |= (0xC<<(4+8));   //IC4F=1100,f_SAMPLING=f_DTS/16, N=8
 	TIM2->CCMR2 |= 0U<<(2+8);   //IC4PSC=00 	配置输入分频,不分频 
    TIM1->CCER &= ~(0U<<13); 		//CC1P=0	上升沿捕获   
	TIM2->CCER |= 1U<<12; 		//CC4E=1 	允许捕获计数器的值到捕获寄存器中		
	TIM2->DIER |= 1U<<0;        //UIE = 1;//开更新中断服务请求
	TIM2->DIER |= 1U<<4;        //CC4IE = 1;//开通道4的捕获/比较中断服务请求	
	
	MY_NVIC_Init(2,0,TIM2_IRQn,2);//抢占2，子优先级0，组2
    TIM2->CR1 |= (0x1<<0);    	//CEN = 1; 启动定时器
}
static u8  TIM2CH4_CAPTURE_STA=0;	//输入捕获状态		    				
static u32	TIM2CH4_CAPTURE_VAL;	//输入捕获值
u32 TIM2CH4_T=0;
u8 TIM2CH4_trig=0;
void TIM2_IRQHandler(void)
{
    if(TIM2->SR & 0X0001)//更新中断if(UIF=1)
    {
        TIM2->SR &= ~(1U<<0);//清除更新中断标志位UIF=0;
        TIM2CH4_CAPTURE_STA++;
    }
    if(TIM2->SR&0X0010)//比较/捕获通道4中断if(CC4IF=1)
    {
        TIM2->SR &= ~(1U<<4);//清除触发中断标志位CC4IF=0;
     
        TIM2CH4_CAPTURE_VAL = (TIM2->CNT) + (TIM2CH4_CAPTURE_STA*0xFFFF);
        
      
    
        //计算相邻下降沿时长
        TIM2CH4_CAPTURE_VAL +=5;
        TIM2CH4_CAPTURE_VAL /=10;//单位ms
        
        if(TIM2CH4_CAPTURE_VAL)          
        TIM2CH4_T = 6000/TIM2CH4_CAPTURE_VAL;
        
        TIM2CH4_trig ++;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TIM3_PWM_PB1_Init(void)
{
    //配置TIM3_CH4,复用输出，映射到PB1
    //TIM1_REMAP[1:0]=00(没有重映射) TIM1_CH1~CH4 --> PA8~PA11
    RCC->APB1ENR |= 1U<<1;     //TIM3 时钟使能
    RCC->APB2ENR |= 1U<<3;  	//PORTB时钟使能
	GPIOB->CRL &= 0XFFFFFF0F;   //PB1 清除之前设置  
	GPIOB->CRL |= 0X000000B0;	//PB1 复用推挽输出

    //配置时钟源    
    TIM3->SMCR &= ~(0x7<<0);//SMS[2:0]=000;//内部时钟CK_INT    
    
    //10KHz
	TIM3->ARR = 100-1;	    //设定计数器自动重装值 
    //配置Compare Output通道4
    TIM3->CCER &= (uint16_t)(~((uint16_t)TIM_CCER_CC4E));//CC4E = 0;
    TIM3->CCMR2 &= (uint16_t)(~((uint16_t)TIM_CCMR2_OC4M));
    TIM3->CCMR2 &= (uint16_t)(~((uint16_t)TIM_CCMR2_CC4S));//CC4S[1:0] = 00 配置为CC4为输出模式
    TIM3->CCMR2 |= (uint16_t)(((uint16_t)0x0070) << 8);//OC4M[2:0]=111 配置为PWM模式二
    TIM3->CCER &= (uint16_t)(~((uint16_t)TIM_CCER_CC4P));//CC4P = 0;
    TIM3->CCER |= (uint16_t)(((uint16_t)TIM_CCER_CC4P));//CC4P = 1;
    TIM3->CCER |= (uint16_t)(((uint16_t)0x0001) << 12);//CC4E = 1;
    TIM3->CR2 &= (uint16_t)(~((uint16_t)TIM_CR2_OIS4));
    TIM3->CR2 |= (uint16_t)((uint16_t)0x0 << 6);//OIS4 = 0
    TIM3->BDTR |= TIM_BDTR_MOE;
    TIM3->CCMR2 |= (uint16_t)((uint16_t)0x8<<8); //OC4CE = 1;
    TIM3->CR1 |= TIM_CR1_ARPE;//ARPE = 1;使能ARR预装载
    TIM3->DIER &= 0x0000;//复位，关闭所有DMA请求和中断请求
    TIM1->DIER |= 1U<<0;        //UIE = 1;//开更新中断服务请求
	MY_NVIC_Init(2,1,TIM3_IRQn,2);//抢占2，子优先级0，组2

   
}

void setPWM(u8 _pwm)
{
    if(_pwm > 100)
        _pwm = 100;
    
    TIM3->CCR4 = _pwm;
}








