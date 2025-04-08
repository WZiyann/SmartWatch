#ifndef __ADXL345_H__
#define __ADXL345_H__
#include "sys.h"
#include "delay.h"

//����ӿ�
#define ADXL345_SCL_PORT    GPIOA
#define ADXL345_SCL_PIN     GPIO_Pin_12
#define ADXL345_SDA_PORT    GPIOA
#define ADXL345_SDA_PIN     GPIO_Pin_15




//�����궨�壬�����޸ģ�������֪���Լ��ڸ�ʲô
#define ADXL345_SCL_Clr() GPIO_ResetBits(ADXL345_SCL_PORT,ADXL345_SCL_PIN)//SCL
#define ADXL345_SCL_Set() GPIO_SetBits(ADXL345_SCL_PORT,ADXL345_SCL_PIN)

#define ADXL345_SDA_Clr() GPIO_ResetBits(ADXL345_SDA_PORT,ADXL345_SDA_PIN)//SDA
#define ADXL345_SDA_Set() GPIO_SetBits(ADXL345_SDA_PORT,ADXL345_SDA_PIN)

#define ADXL345_SDA_IN() GPIO_Config(ADXL345_SDA_PORT,ADXL345_SDA_PIN,INPUT_FLOAT)
#define ADXL345_SDA_OUT() GPIO_Config(ADXL345_SDA_PORT,ADXL345_SDA_PIN,OUTPUT_UP)
#define ADXL345_SDA_Read() GPIO_ReadInputDataBit(ADXL345_SDA_PORT,ADXL345_SDA_PIN)


#define	SlaveAddress   0xA6	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�

#define JudgeP_M  0x8000   //�������ж�
#define Left_cmp  0x006e   //�������ݱȽ�ֵ����ֵԽС��Խ����
#define Right_cmp 0x006e
#define Go_cmp    0x006e
#define Back_cmp  0x006e


extern unsigned int ADXL345_step;

//���ú���������
void ADXL345_Init(void);
char ADXL345_Check(void);
char ADXL345_Step_Count(void);
#endif

