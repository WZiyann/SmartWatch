/*--------------------------------------------------------------------------
�¶ȴ�����DS18B20

V1.0
(1)ֻҪ����ӿڼ���(DS18B20_PORT DS18B20_PIN)
(2)ʹ��ǰ�����ʼ��u8 DS18B20_Init(void);
(3)��ȡ�¶�ֵshort DS18B20_Get_Temp(void);


Copyright (c) 2020-2030 Quan's technology, Inc.
All rights reserved.
--------------------------------------------------------------------------*/
#include "ds18b20.h"
#include "delay.h"	

u8 getDS18B20_DQ(void)
{
    return GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_PIN);
}
void setDS18B20_DQ(u8 _sta)
{
    if(_sta)GPIO_SetBits(DS18B20_PORT,DS18B20_PIN);
    else GPIO_ResetBits(DS18B20_PORT,DS18B20_PIN);
}

//��λDS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); 	//SET PG11 OUTPUT
    setDS18B20_DQ(0); 	//����DQ
    delay_us(750);    	//����750us
    setDS18B20_DQ(1); 	//DQ=1 
	delay_us(15);     	//15US
}
//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();	//SET PG11 INPUT	 
    while (getDS18B20_DQ()&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!getDS18B20_DQ()&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
//��DS18B20��ȡһ��λ
//����ֵ��1/0
u8 DS18B20_Read_Bit(void) 	 
{
    u8 data;
	DS18B20_IO_OUT();	//SET PG11 OUTPUT
    setDS18B20_DQ(0); 
	delay_us(2);
    setDS18B20_DQ(1); 
	DS18B20_IO_IN();	//SET PG11 INPUT
	delay_us(12);
	if(getDS18B20_DQ())data=1;
    else data=0;	 
    delay_us(50);           
    return data;
}
//��DS18B20��ȡһ���ֽ�
//����ֵ������������
u8 DS18B20_Read_Byte(void)     
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	DS18B20_IO_OUT();	//SET PG11 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            setDS18B20_DQ(0);	// Write 1
            delay_us(2);                            
            setDS18B20_DQ(1);
            delay_us(60);             
        }
        else 
        {
            setDS18B20_DQ(0);	// Write 0
            delay_us(60);             
            setDS18B20_DQ(1);
            delay_us(2);                          
        }
    }
}
//��ʼ�¶�ת��
void DS18B20_Start(void) 
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0x44);	// convert
} 
//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
u8 DS18B20_Init(void)
{
    DS18B20_IO_OUT();
	DS18B20_Rst();
	return DS18B20_Check();
}

//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
	short tem;
    DS18B20_Start ();  			// ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0xbe);	// convert	    
    TL=DS18B20_Read_Byte(); 	// LSB   
    TH=DS18B20_Read_Byte(); 	// MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;					//�¶�Ϊ��  
    }else temp=1;				//�¶�Ϊ��	  	  
    tem=TH; 					//��ø߰�λ
    tem<<=8;    
    tem+=TL;					//��õװ�λ
    tem=(float)tem*0.625;		//ת��     
	if(temp)return tem; 		//�����¶�ֵ
	else return -tem;    
}
















