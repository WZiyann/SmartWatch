#include "usart.h"	  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "led.h" 
#include "delay.h"
#include "buzzer.h"

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
int _sys_exit(int x) 
{ 
	x = x;
    return 0;
} 
//重定向fputc函数
//printf的输出，指向fputc，由fputc输出到串口
//这里使用串口1(USART1)输出printf信息
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART1->DR = (u8) ch;      	//写DR,串口1将发送数据
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////


//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置 
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断 
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(2,0,USART1_IRQn,2);//组2，最低优先级 

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
	if(USART1->SR&(1<<5))//接收到数据
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
				if(c=='>')//数值结束符
				{
					revDate[(revLen&0x7F)] = revTemp;//保存数据
                    
                    if(revLen>=8)//接收完成
                    {
                        index = 0;
                        revPara_F = 1;
                    }
                    else//接收下一个数字
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
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) c;
}
void serialPrint(char *str) {
    while (*str)
	{
        while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
        USART1->DR = (u8) *str;
        str++;
    }
}
//蓝牙接口程序
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



