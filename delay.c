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
#include "delay.h"

static u32 _t_ms = 0;//10ms

//////////////////////////////////////////////////////////////////////////////////	 
static u8  fac_us=0;//
//static u16 fac_ms=0;//


u32 GetTick(void)
{
	return _t_ms;//us��
}


void delay_Init(u8 SYSCLK)
{
	u32 reload;
 	SysTick->CTRL&=~(1<<2);					//SYSTICKʹ���ⲿʱ��Դ	 
	fac_us=SYSCLK/8;						//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	reload=SYSCLK/8;						//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/delay_ostickspersec;	//����delay_ostickspersec�趨���ʱ��
											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
	SysTick->CTRL|=1<<1;   					//����SYSTICK�ж�
	SysTick->LOAD=reload; 					//ÿ1/delay_ostickspersec���ж�һ��	
	SysTick->CTRL|=1<<0;   					//����SYSTICK   
}
void SysTick_Handler(void)  
{  
     _t_ms ++;     
}
		    								   
//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}
}


//��ʱnms
//nms:Ҫ��ʱ��ms��
void delay_ms(u16 nms)
{	
	delay_us((u32)(nms*1000));				//��ͨ��ʽ��ʱ  
}



void GPIO_Config(GPIO_TypeDef* GPIOx,GPIO_Pin_e GPIO_Pin_x,INPUTorOUTPUT_e INPUTorOUTPUT)
{
	unsigned char IOx = 0;
	if((INPUTorOUTPUT&0x0F)>0x02)INPUTorOUTPUT = INPUT_FLOAT;
	
	if((GPIOx==GPIOB)&&((GPIO_Pin_x==GPIO_Pin_3)||(GPIO_Pin_x==GPIO_Pin_4)))	
	{
		//PB3/PB4                                                                                                                                                                                                           
		RCC->APB2ENR|= 0x00000001;//AFIOEN = 1;��������IOʱ��ʹ��
		AFIO->MAPR 	|= 0x02000000;//����AFIO_MAPR,��SWJ_CFG[2:0]λΪ010���ر�JTAG-DP������SW-DP
	}	
	     if(GPIOx == GPIOA)RCC->APB2ENR|=1<<2;
	else if(GPIOx == GPIOB)RCC->APB2ENR|=1<<3;	
	else if(GPIOx == GPIOC)RCC->APB2ENR|=1<<4;
	else return;


	//��λGPIOģʽΪ00
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
