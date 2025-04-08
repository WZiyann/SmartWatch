/*--------------------------------------------------------------------------
TIM��������
Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#include "timer.h"
#include "buzzer.h"
#include "led.h"

void TIM1_Cap_PA11_Init(void)
{
    //����TIM1_CH4,�������룬ӳ�䵽PA11
    RCC->APB2ENR |= 1U<<11;     //TIM1 ʱ��ʹ��
	GPIOA->CRH &= 0XFFFF0FFF;   //PA11 ���֮ǰ����  
	GPIOA->CRH |= 0X00008000;	//PA11 ��������
	GPIOA->ODR |= 1<<11;		//PA11 ����    

 	//����TIM1ʱ����1MHzƵ�ʼ���
    TIM1->ARR=0XFFFF;  		//�趨�������Զ���װֵ   


    //����ʱ��Դ    
    TIM1->SMCR &= (0x7<<0);//SMS[2:0]=000;//�ڲ�ʱ��CK_INT
    
    //���ò���ͨ��
    TIM1->CCMR2 &= 0x00FF;      //��λͨ��4�ϵ�����
	TIM1->CCMR2 |= 1U<<(0+8);   //CC4S=01 	ѡ������� IC1ӳ�䵽TI1��
 	TIM1->CCMR2 |= 0U<<(4+8);   //IC4F=0000 ���������˲��� ���˲�
    TIM2->CCMR2 |= (0xC<<(4+8));   //IC4F=1100,f_SAMPLING=f_DTS/16, N=8
 	TIM1->CCMR2 |= 0U<<(2+8);   //IC4PSC=00 	���������Ƶ,����Ƶ 

    TIM1->CCER &= ~(0U<<13); 		//CC1P=0	�����ز���   
	TIM1->CCER |= 1U<<12; 		//CC4E=1 	�������������ֵ������Ĵ�����

    TIM1->DIER = 0X0000;        //��λ���ر�����DMA������ж�����			
	TIM1->DIER |= 1U<<0;        //UIE = 1;//�������жϷ�������
	TIM1->DIER |= 1U<<4;        //CC4IE = 1;//��ͨ��4�Ĳ���/�Ƚ��жϷ�������	

	MY_NVIC_Init(2,0,TIM1_UP_IRQn,2);//��ռ2�������ȼ�0����2
    TIM1->CR1 |= (0x1<<0);    	//CEN = 1; ������ʱ��
}

//____      _____________      ________________
//    |    |             |    |     
//    |____|             |____| 
//
//����2���½��ص�����
static u8  TIM1CH4_CAPTURE_STA=0;	//���벶��״̬		    				
static u16	TIM1CH4_CAPTURE_VAL;	//���벶��ֵ
static u32 TIM1CH4_CAPTURE_T;
u8 heartRate=0;
u8 hr_trig=0;
//��ʱ��5�жϷ������
void TIM1_UP_IRQHandler(void)
{
    if(TIM1->SR & 0X0001)//�����ж�if(UIF=1)
    {
        TIM1->SR &= ~(1U<<0);//��������жϱ�־λUIF=0;
        TIM1CH4_CAPTURE_STA ++;//��¼����Ĵ���
    } 
    //TIM1->SR=0;//����жϱ�־λ
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
    if(TIM1->SR&0X0010)//�Ƚ�/����ͨ��4�ж�if(CC4IF=1)
    {
        TIM1->SR &= ~(1U<<4);//��������жϱ�־λCC4IF=0;
        TIM1->SR &= ~(1U<<12);//����ظ�������CC4OF=0
        
        //��¼һ�β���ʱ��
        TIM1CH4_CAPTURE_VAL = TIM1->CCR4;
        TIM1CH4_CAPTURE_T = TIM1CH4_CAPTURE_STA*0XEEEE + TIM1CH4_CAPTURE_VAL;
        
        TIM1CH4_CAPTURE_STA = 0;
        TIM1->CNT = 0;
         
        //���������½���ʱ��
        TIM1CH4_CAPTURE_T +=5;
        TIM1CH4_CAPTURE_T /=10;//��λms
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
    //����TIM2_CH4,�������룬ӳ�䵽PA11
    //TIM1_REMAP[1:0]=00(û����ӳ��) TIM1_CH1~CH4 --> PA8~PA11
    RCC->APB1ENR |= 1U<<1;     //TIM
	GPIOA->CRL &= 0XFFFFFF0F;   //PA3���֮ǰ����  
	GPIOA->CRL |= 0X00008000;	//PA3 ��������
	GPIOA->ODR |= 1<<3;		//PA3 ����    

 	//����TIM1ʱ����1MHzƵ�ʼ���
    TIM2->ARR=0XFFFF;  		//�趨�������Զ���װֵ   

    //����ʱ��Դ    
    TIM2->SMCR &= ~(0x7<<0);//SMS[2:0]=000;//�ڲ�ʱ��CK_INT
    
    //���ò���ͨ��
    TIM2->CCMR2 &= 0x00FF;      //��λͨ��4�ϵ�����
	TIM2->CCMR2 |= 1U<<(0+8);   //CC4S=01 	ѡ������� IC1ӳ�䵽TI1��
 	TIM2->CCMR2 |= 0U<<(4+8);   //IC4F=0000 ���������˲��� ���˲�
    TIM2->CCMR2 |= (0xC<<(4+8));   //IC4F=1100,f_SAMPLING=f_DTS/16, N=8
 	TIM2->CCMR2 |= 0U<<(2+8);   //IC4PSC=00 	���������Ƶ,����Ƶ 
    TIM1->CCER &= ~(0U<<13); 		//CC1P=0	�����ز���   
	TIM2->CCER |= 1U<<12; 		//CC4E=1 	�������������ֵ������Ĵ�����		
	TIM2->DIER |= 1U<<0;        //UIE = 1;//�������жϷ�������
	TIM2->DIER |= 1U<<4;        //CC4IE = 1;//��ͨ��4�Ĳ���/�Ƚ��жϷ�������	
	
	MY_NVIC_Init(2,0,TIM2_IRQn,2);//��ռ2�������ȼ�0����2
    TIM2->CR1 |= (0x1<<0);    	//CEN = 1; ������ʱ��
}
static u8  TIM2CH4_CAPTURE_STA=0;	//���벶��״̬		    				
static u32	TIM2CH4_CAPTURE_VAL;	//���벶��ֵ
u32 TIM2CH4_T=0;
u8 TIM2CH4_trig=0;
void TIM2_IRQHandler(void)
{
    if(TIM2->SR & 0X0001)//�����ж�if(UIF=1)
    {
        TIM2->SR &= ~(1U<<0);//��������жϱ�־λUIF=0;
        TIM2CH4_CAPTURE_STA++;
    }
    if(TIM2->SR&0X0010)//�Ƚ�/����ͨ��4�ж�if(CC4IF=1)
    {
        TIM2->SR &= ~(1U<<4);//��������жϱ�־λCC4IF=0;
     
        TIM2CH4_CAPTURE_VAL = (TIM2->CNT) + (TIM2CH4_CAPTURE_STA*0xFFFF);
        
      
    
        //���������½���ʱ��
        TIM2CH4_CAPTURE_VAL +=5;
        TIM2CH4_CAPTURE_VAL /=10;//��λms
        
        if(TIM2CH4_CAPTURE_VAL)          
        TIM2CH4_T = 6000/TIM2CH4_CAPTURE_VAL;
        
        TIM2CH4_trig ++;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TIM3_PWM_PB1_Init(void)
{
    //����TIM3_CH4,���������ӳ�䵽PB1
    //TIM1_REMAP[1:0]=00(û����ӳ��) TIM1_CH1~CH4 --> PA8~PA11
    RCC->APB1ENR |= 1U<<1;     //TIM3 ʱ��ʹ��
    RCC->APB2ENR |= 1U<<3;  	//PORTBʱ��ʹ��
	GPIOB->CRL &= 0XFFFFFF0F;   //PB1 ���֮ǰ����  
	GPIOB->CRL |= 0X000000B0;	//PB1 �����������

    //����ʱ��Դ    
    TIM3->SMCR &= ~(0x7<<0);//SMS[2:0]=000;//�ڲ�ʱ��CK_INT    
    
    //10KHz
	TIM3->ARR = 100-1;	    //�趨�������Զ���װֵ 
    //����Compare Outputͨ��4
    TIM3->CCER &= (uint16_t)(~((uint16_t)TIM_CCER_CC4E));//CC4E = 0;
    TIM3->CCMR2 &= (uint16_t)(~((uint16_t)TIM_CCMR2_OC4M));
    TIM3->CCMR2 &= (uint16_t)(~((uint16_t)TIM_CCMR2_CC4S));//CC4S[1:0] = 00 ����ΪCC4Ϊ���ģʽ
    TIM3->CCMR2 |= (uint16_t)(((uint16_t)0x0070) << 8);//OC4M[2:0]=111 ����ΪPWMģʽ��
    TIM3->CCER &= (uint16_t)(~((uint16_t)TIM_CCER_CC4P));//CC4P = 0;
    TIM3->CCER |= (uint16_t)(((uint16_t)TIM_CCER_CC4P));//CC4P = 1;
    TIM3->CCER |= (uint16_t)(((uint16_t)0x0001) << 12);//CC4E = 1;
    TIM3->CR2 &= (uint16_t)(~((uint16_t)TIM_CR2_OIS4));
    TIM3->CR2 |= (uint16_t)((uint16_t)0x0 << 6);//OIS4 = 0
    TIM3->BDTR |= TIM_BDTR_MOE;
    TIM3->CCMR2 |= (uint16_t)((uint16_t)0x8<<8); //OC4CE = 1;
    TIM3->CR1 |= TIM_CR1_ARPE;//ARPE = 1;ʹ��ARRԤװ��
    TIM3->DIER &= 0x0000;//��λ���ر�����DMA������ж�����
    TIM1->DIER |= 1U<<0;        //UIE = 1;//�������жϷ�������
	MY_NVIC_Init(2,1,TIM3_IRQn,2);//��ռ2�������ȼ�0����2

   
}

void setPWM(u8 _pwm)
{
    if(_pwm > 100)
        _pwm = 100;
    
    TIM3->CCR4 = _pwm;
}








