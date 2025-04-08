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
#include "delay.h"

static u32 _t_ms = 0;//10ms

//////////////////////////////////////////////////////////////////////////////////	 
static u8  fac_us=0;//
//static u16 fac_ms=0;//


u32 GetTick(void)
{
	return _t_ms;//us数
}


void delay_Init(u8 SYSCLK)
{
	u32 reload;
 	SysTick->CTRL&=~(1<<2);					//SYSTICK使用外部时钟源	 
	fac_us=SYSCLK/8;						//不论是否使用OS,fac_us都需要使用
	reload=SYSCLK/8;						//每秒钟的计数次数 单位为K	   
	reload*=1000000/delay_ostickspersec;	//根据delay_ostickspersec设定溢出时间
											//reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右	
	SysTick->CTRL|=1<<1;   					//开启SYSTICK中断
	SysTick->LOAD=reload; 					//每1/delay_ostickspersec秒中断一次	
	SysTick->CTRL|=1<<0;   					//开启SYSTICK   
}
void SysTick_Handler(void)  
{  
     _t_ms ++;     
}
		    								   
//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	}
}


//延时nms
//nms:要延时的ms数
void delay_ms(u16 nms)
{	
	delay_us((u32)(nms*1000));				//普通方式延时  
}



void GPIO_Config(GPIO_TypeDef* GPIOx,GPIO_Pin_e GPIO_Pin_x,INPUTorOUTPUT_e INPUTorOUTPUT)
{
	unsigned char IOx = 0;
	if((INPUTorOUTPUT&0x0F)>0x02)INPUTorOUTPUT = INPUT_FLOAT;
	
	if((GPIOx==GPIOB)&&((GPIO_Pin_x==GPIO_Pin_3)||(GPIO_Pin_x==GPIO_Pin_4)))	
	{
		//PB3/PB4                                                                                                                                                                                                           
		RCC->APB2ENR|= 0x00000001;//AFIOEN = 1;辅助功能IO时钟使能
		AFIO->MAPR 	|= 0x02000000;//配置AFIO_MAPR,的SWJ_CFG[2:0]位为010：关闭JTAG-DP，启用SW-DP
	}	
	     if(GPIOx == GPIOA)RCC->APB2ENR|=1<<2;
	else if(GPIOx == GPIOB)RCC->APB2ENR|=1<<3;	
	else if(GPIOx == GPIOC)RCC->APB2ENR|=1<<4;
	else return;


	//复位GPIO模式为00
	if(IOx<8)GPIOx->CRL&=(u32)(~(0X0000000F<<(IOx*4)));
	if(IOx>7)GPIOx->CRH&=(u32)(~(0X0000000F<<((IOx-8)*4)));

	if(INPUTorOUTPUT==INPUT_UP)//INPUT_UP
	{
		if(IOx<8)GPIOx->CRL|=0X08<<(IOx*4); 
		if(IOx>7)GPIOx->CRH|=0X08<<((IOx-8)*4);
		GPIOx->ODR|=(u32)(1<<IOx);
	}
	else if(INPUTorOUTPUT==INPUT_DOWN)//INPUT_DOWN
	{
		if(IOx<8)GPIOx->CRL|=0X08<<(IOx*4); 
		if(IOx>7)GPIOx->CRH|=0X08<<((IOx-8)*4);
		GPIOx->ODR&=~(u32)(1<<IOx);//???						
	}
	else if(INPUTorOUTPUT==INPUT_FLOAT)//INPUT_FLOAT
	{ 
		if(IOx<8)GPIOx->CRL|=0X04<<(IOx*4); 
		if(IOx>7)GPIOx->CRH|=0X04<<((IOx-8)*4);
	}

	else if(INPUTorOUTPUT==OUTPUT_UP)
	{
		if(IOx<8)GPIOx->CRL|=0X03<<(IOx*4); 
		if(IOx>7)GPIOx->CRH|=0X03<<((IOx-8)*4); 
			
		GPIOx->ODR|=(u32)(1<<IOx); 
	}
	else if(INPUTorOUTPUT==OUTPUT_DOWN)
	{
		if(IOx<8)GPIOx->CRL|=0X03<<(IOx*4); 				
		if(IOx>7)GPIOx->CRH|=0X03<<((IOx-8)*4); 
			
		GPIOx->ODR&=~(u32)(1<<IOx);//???						
	}

}


uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;
   
  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  
  GPIOx->BSRR = GPIO_Pin;
}
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{ 
  GPIOx->BRR = GPIO_Pin;
}
