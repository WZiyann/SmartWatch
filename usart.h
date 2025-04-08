#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"	 
 
void cliBufClr(void);
extern char cliBuf[];

extern u8 revLED_F;
extern u8 revPara_F;
extern u16 revDate[];

//串口1函数定义
void Send_Char_Com(char c);
void serialPrint(char *str) ;
void uart_init(u32 pclk2,u32 bound);

//APP接口函数   
void APPsendTitle(char* _name1);
void APPsendValue(char* _value1,char* _value2,char* _value3,char* _value4,char* _value5,char* _value6);
void APPsendLable0(char* _name1,char* _name2,char* _name3,char* _name4,char* _name5,char* _name6);
void APPsendLable1(char* _name1,char* _name2,char* _name3,char* _name4,char* _name5,char* _name6,char* _name7,char* _name8);
void APPsendLable2(char* _name1,char* _name2,char* _name3,char* _name4);
void APPsendTitle(char* _name1);

#endif	











