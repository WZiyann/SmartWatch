#include "usart.h"	  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "led.h" 
#include "delay.h"
#include "buzzer.h"

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
int _sys_exit(int x) 
{ 
	x = x;
    return 0;
} 
//�ض���fputc����
//printf�������ָ��fputc����fputc���������
//����ʹ�ô���1(USART1)���printf��Ϣ
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//�ȴ���һ�δ������ݷ������  
	USART1->DR = (u8) ch;      	//дDR,����1����������
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////


//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬���� 
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(2,0,USART1_IRQn,2);//��2��������ȼ� 

}

    
u8 revLED_F  = 0;
u8 revPara_F = 0;
u16 revDate[8];
static int32_t revTemp=0;;
static uint8_t revLen=0;
void USART1_IRQHandler(void)
{
	u8 c;
    static u8 index=0;
	if(USART1->SR&(1<<5))//���յ�����
	{	 
		c = USART1->DR;
        switch (index)
        {
            case 0:
                if(c=='k')index++;                    
                else if(c=='s')index=0xf0;                                   
                break;
            case 1:
                if(c=='e')index++;                
                else index = 0;                                  
                break;
            case 2://key< >
                if(c=='y')index++;
                else index = 0;             
                break;
            case 3:
                if(c=='<')index++;
                else index = 0;
                break;
            //set<...><><>    
            case 0xf0:
                if(c=='e')index++;                    
                else index = 0;                    
                break; 
            case 0xf1://set<>
                if(c=='t'){index++;revLen=0;}
                else index = 0;                                  
                break;                 
            case 0xf2:
                if(c=='<'){index++;revTemp=0;revLen++;revLen=0x80;}
                else index = 0;                                  
                break;
            case 0xf3:
				if(c=='>')//��ֵ������
				{
					revDate[(revLen&0x7F)] = revTemp;//��������
                    
                    if(revLen>=8)//�������
                    {
                        index = 0;
                        revPara_F = 1;
                    }
                    else//������һ������
                    {
                        index = 0xf2;
                    }
				}        
                break; 
			

            default:
                break;
        }        
          		 									     
	}  									 
}

void Send_Char_Com(char c){
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) c;
}
void serialPrint(char *str) {
    while (*str)
	{
        while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
        USART1->DR = (u8) *str;
        str++;
    }
}
//�����ӿڳ���
void APPsendValue(char* _value1,char* _value2,char* _value3,char* _value4,char* _value5,char* _value6)
{
    char strBuf[100];
    sprintf(strBuf, "value<%s><%s><%s><%s><%s><%s>", _value1,_value2,_value3,_value4,_value5,_value6);
    serialPrint(strBuf);
}
void APPsendLable0(char* _name1,char* _name2,char* _name3,char* _name4,char* _name5,char* _name6)
{
    char strBuf[100];
    sprintf(strBuf, "label0<%s><%s><%s><%s><%s><%s>", _name1,_name2,_name3,_name4,_name5,_name6);
    serialPrint(strBuf);   
}
void APPsendLable1(char* _name1,char* _name2,char* _name3,char* _name4,char* _name5,char* _name6,char* _name7,char* _name8)
{
    char strBuf[100];
    sprintf(strBuf, "label1<%s><%s><%s><%s><%s><%s><%s><%s>", _name1,_name2,_name3,_name4,_name5,_name6,_name7,_name8);
    serialPrint(strBuf);   
}
void APPsendLable2(char* _name1,char* _name2,char* _name3,char* _name4)
{
    char strBuf[100];
    sprintf(strBuf, "label2<%s><%s><%s><%s>", _name1, _name2, _name3, _name4);
    serialPrint(strBuf);   
}
void APPsendTitle(char* _name1)
{
    char strBuf[100];
    sprintf(strBuf, "title<%s>", _name1);
    serialPrint(strBuf);     
}



