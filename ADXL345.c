#include "ADXL345.h"

int X,Y,Z;
unsigned int ADXL345_step;

//��ʼ��IIC
void ADXL345_IIC_Init(void)
{					     
    ADXL345_SDA_OUT();
    GPIO_Config(ADXL345_SCL_PORT,ADXL345_SCL_PIN,OUTPUT_UP);
}

//����IIC��ʼ�ź�
void ADXL345_IIC_Start(void)
{
	ADXL345_SDA_OUT();     //sda�����
    
	ADXL345_SDA_Set();	  	  
	ADXL345_SCL_Set();
	delay_us(5);
 	ADXL345_SDA_Clr();//START:when CLK is high,DATA change form high to low 
	delay_us(5);
	ADXL345_SCL_Clr();//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void ADXL345_IIC_Stop(void)
{
	ADXL345_SDA_OUT();//sda�����
    
	ADXL345_SDA_Clr();
	ADXL345_SCL_Set();
 	delay_us(5);
	ADXL345_SDA_Set();//����I2C���߽����ź�
	delay_us(5);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 ADXL345_IIC_Wait_Ack(void)
{
    u8 temp;
	ADXL345_SDA_IN();      //SDA����Ϊ����
      
	ADXL345_SCL_Set();
    delay_us(5);
    temp = ADXL345_SDA_Read();
	ADXL345_SCL_Clr();//ʱ�����0
    delay_us(5);    
	return temp;  
} 

//��IIC���߷���Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
void ADXL345_IIC_SendACK(u8 ack)
{
	ADXL345_SDA_OUT();
    if(ack)ADXL345_SDA_Set();
    else ADXL345_SDA_Clr();
    ADXL345_SCL_Set();    
    delay_us(5);
    ADXL345_SCL_Clr();
    delay_us(5);
}

//IIC����һ���ֽ�		  
void ADXL345_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	ADXL345_SDA_OUT();
    for(t=0;t<8;t++)
    {              
        if(txd&0x80)ADXL345_SDA_Set();
        else ADXL345_SDA_Clr();
        txd<<=1;
		ADXL345_SCL_Set();        
		delay_us(5); 
		ADXL345_SCL_Clr();
		delay_us(5); 
    }
    ADXL345_IIC_Wait_Ack();
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 ADXL345_IIC_Read_Byte(void)
{
	unsigned char i,receive=0;
	ADXL345_SDA_Set();    
	ADXL345_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        receive<<=1;
        ADXL345_SCL_Set(); 
        delay_us(5);
        if(ADXL345_SDA_Read())receive++;
        ADXL345_SCL_Clr();        
		delay_us(5); 
    }					 
 
    return receive;
}

void ADXL345_Single_Write(unsigned char REG_Address,unsigned char REG_data)
{
    ADXL345_IIC_Start();                  //��ʼ�ź�
    ADXL345_IIC_Send_Byte(SlaveAddress);   //�����豸��ַ+д�ź�
    ADXL345_IIC_Send_Byte(REG_Address);    //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
    ADXL345_IIC_Send_Byte(REG_data);       //�ڲ��Ĵ������ݣ���ο�����pdf22ҳ 
    ADXL345_IIC_Stop();                   //����ֹͣ�ź�
}
unsigned char ADXL345_Single_Read(unsigned char REG_Address)
{  
	unsigned char REG_data;
	ADXL345_IIC_Start();                          //��ʼ�ź�
	ADXL345_IIC_Send_Byte(SlaveAddress);           //�����豸��ַ+д�ź�
	ADXL345_IIC_Send_Byte(REG_Address);            //���ʹ洢��Ԫ��ַ����0��ʼ	
	ADXL345_IIC_Start();                          //��ʼ�ź�
	ADXL345_IIC_Send_Byte(SlaveAddress + 1);       //�����豸��ַ+���ź�
	REG_data = ADXL345_IIC_Read_Byte();            //�����Ĵ�������
    ADXL345_IIC_SendACK(1);  
	ADXL345_IIC_Stop();                           //ֹͣ�ź�
	return REG_data; 
}
void ADXL345_Multiple_Read(void)
{   
	unsigned char BUF[6];
    unsigned char i;
    ADXL345_IIC_Start();                          //��ʼ�ź�
    ADXL345_IIC_Send_Byte(SlaveAddress);          //�����豸��ַ+д�ź�
    ADXL345_IIC_Send_Byte(0x32);                  //���ʹ洢��Ԫ��ַ����0x32��ʼ	
    ADXL345_IIC_Start();                          //��ʼ�ź�
    ADXL345_IIC_Send_Byte(SlaveAddress+1);        //�����豸��ַ+���ź�
	for (i=0; i<6; i++)                   //������ȡ6����ַ���ݣ��洢��BUF
	{
		BUF[i] = ADXL345_IIC_Read_Byte();         //BUF[0]�洢0x32��ַ�е�����
		if (i == 5)
		{
            ADXL345_IIC_SendACK(1);//���ͷ�Ӧ��
		}
		else
		{     
            ADXL345_IIC_SendACK(0);//����Ӧ��
		}
	}
	ADXL345_IIC_Stop();  //ֹͣ�ź�
    
	X = (BUF[1]<<8)+BUF[0];  //�ϳ�16λ���ݣ�-32768~+32767��
	Y = (BUF[3]<<8)+BUF[2];
	Z = (BUF[5]<<8)+BUF[4];
   
}

//��ʼ��ADXL345��������Ҫ��ο�pdf�����޸�************************
void ADXL345_Init(void)
{
   ADXL345_IIC_Init();
   ADXL345_Single_Write(0x31,0x0B);   //������Χ,����16g��13λģʽ
   ADXL345_Single_Write(0x2C,0x08);   //�����趨Ϊ12.5 �ο�pdf13ҳ
   ADXL345_Single_Write(0x2D,0x08);   //ѡ���Դģʽ   �ο�pdf24ҳ
   ADXL345_Single_Write(0x2E,0x80);   //ʹ�� DATA_READY �ж�
   ADXL345_Single_Write(0x1E,0x00);   //X ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
   ADXL345_Single_Write(0x1F,0x00);   //Y ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
   ADXL345_Single_Write(0x20,0x05);   //Z ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ  
}
//0-��ȷ��1-����
char ADXL345_Check(void)
{
    unsigned char sta;
    sta = ADXL345_Single_Read(0X00);
    if(sta != 0xE5)return 1;
    return 0;
}
/*
         o
        /|\
       / | \
swing o  o  o swing
         |
      recover
*/ 

char ADXL345_Step_Count(void)
{
    static char recover = 1;
    ADXL345_Multiple_Read();
    if(X&0x8000)X = 65535 - X;
    
    if(X > 150)
    {
        if(recover)
        {
            recover = 0;
            ADXL345_step ++;
            return 1;
        }            
    }
    else
    {
        recover = 1;
    } 
    return 0;
}

