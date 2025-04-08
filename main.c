#include <stm32f10x.h>
#include "delay.h"
#include "led.h"
#include "key_s.h"
#include "buzzer.h"
#include "rtc.h" 
#include "timer.h"
#include "ds18b20.h"
#include "usart.h"	
#include "stmflash.h"
#include "ADXL345.h"
#include "lcd2TFT.h"
//Flash掉电保护数据部分
#define LEN  50//保存数据个数(最后位数据为CRC)
u16 eep[LEN];
#define hrMax eep[0]
#define hrMin eep[1]
#define stepMax eep[2]
#define tmpMax eep[3]
#define tmpMin eep[4]

#define DIS_NUM 3//定义3个界面
#define DIS_DEFAUL 0
#define DIS_PARA 1
#define DIS_RTC 2

#define SAMP_NUM 4

u8 disCtrl = DIS_DEFAUL;
u8 setRTC = 0;
u8 setPara = 0;
u8 tmp;//温度
u8 hr_buf[5];
u8 hr_dis;//心率值
void readFlash(void);
void writeFlash(void);
void revAPP(void);
void sendAPP(void);
void sortBubbing(u8* _buf,u8 _len)
{
    u8 i,j,temp;
    for(i = 0 ; i <_len-1; i++)
    {
        //第i趟比较
        for(j = 0 ; j <_len-i-1; j++)
        {
            //开始进行比较，如果arr[j]比arr[j+1]的值大，那就交换位置
            if(_buf[j] > _buf[j+1])
            {
                temp = _buf[j];
                _buf[j] = _buf[j+1];
                _buf[j+1] = temp;
            }
        }

    }    
}
int main(void)
{
    u8 cnt;
    u8 key;
    u8 refresh = 1; 
    u8 alarm_tmp=0;
    u8 alarm_hr=0;
    u8 alarm_step=0;
    u8 enable_step=1;
    u8 enable_hr = 1;
    u8 hr_trig_last=0;
    u8 clr_cnt=0;    
    char temp;
    LED_Init();
    delay_Init(72);
    MY_NVIC_PriorityGroupConfig(2);
    uart_init(72,9600);
    KEY_Init();
    LCD_Init(); 
    Buzzer_Init();
    DS18B20_Init();
        
    printf("USART1 send [OK]\r\n");
    ADXL345_Init();
    while(ADXL345_Check())
    {
		LCD_GotoTitle(8);
		LCD_Print("[ADXL345 [err]]");   
        LED = !LED;
        Beep(200);
		delay_ms(500);         
    }
	while(RTC_Init())
	{		
		LCD_GotoTitle(8);
		LCD_Print("RTC init [err]");   
        LED = !LED;
        Beep(200);
		delay_ms(500);        
	}
      
    readFlash();
//    OLED_Clear();
    Beep(200);
    while(1)
    {
        delay_ms(1);
        cnt++;
        revAPP();
        if(cnt>200)
        {
            cnt = 0;
            LED = !LED;
            sendAPP();
            SetBeep(0);
            clr_cnt++;
            if(clr_cnt>10)
            {
                hr_dis = 0;
                hr_trig = 0;
            }             
            tmp = DS18B20_Get_Temp()/10;
            refresh = 1;
            if(enable_step)
            {
                if(ADXL345_Step_Count())
                {
                    Beep(50);
                }                
            }
            else
            {
                ADXL345_step = 0;
            }
                              
            if((setPara|setRTC)==0)
            {
                if(tmp<tmpMin||tmp>tmpMax)
                {
                    if(alarm_tmp==0)alarm_tmp = 2;
                }
                else
                {
                    alarm_tmp = 0;
                }
                
                if(enable_step&&ADXL345_step>=stepMax)
                {
                    if(alarm_step==0)alarm_step = 2;
                }
                else
                {
                    alarm_step = 0;
                }

            }
            if(alarm_tmp==2)SetBeep(LED);
            if(alarm_step==2)SetBeep(LED);
            if(alarm_hr==2)SetBeep(LED);
        }
        if(hr_trig_last != hr_trig)
        {            
            clr_cnt = 0;

            
            if(hr_trig<SAMP_NUM)
            {
                hr_buf[hr_trig] = heartRate;
            }
            else
            {
                             
                
                
                sortBubbing(hr_buf,SAMP_NUM);
                //i = 1;
				
                hr_dis = hr_buf[SAMP_NUM/2];
				
                hr_trig = 0;
                //record_f = 1;
                
                alarm_hr =0;
                if(enable_hr)
				{
					if(hr_dis > hrMax || hr_dis < hrMin)
					{
						if(alarm_hr==0)alarm_hr = 2;
					}
                }
                else
                {
                    alarm_hr=0;
                }
                
            }
            
            hr_trig_last = hr_trig;
            refresh = 1;
        }
        key = KeyScan(1);
        if(key)
        {
            Beep(200);
            refresh = 1;
            LED = 0;
            if(alarm_tmp)alarm_tmp = 1;
            if(alarm_hr)alarm_hr = 1;
            if(alarm_step)alarm_step = 1;
            if(key==0x01)
            {
                if((setPara|setRTC)==0)//切换界面
                {
                    disCtrl++;
                    if(disCtrl>=DIS_NUM)disCtrl = 0;
					LCD_Clear();                  
                }
                else
                {
                    if(setPara)
                    {
                        setPara++;
                        if(setPara>5)
                        {
                            setPara=0;
                            writeFlash();
                        }
                    }
                    else if(setRTC)
                    {
                        setRTC++;
                        if(setRTC>6)
                        {
                            setRTC = 0;
                            RTC_Set((u16)calendar.w_year,calendar.w_month,\
                            calendar.w_date,calendar.hour,calendar.min,calendar.sec);                    
                        }                        
                    }
                }

            }
            else if(key==0xF1)
            {
                Beep(200);
                if(disCtrl==DIS_RTC)
                {
                    if(setRTC==0)setRTC++;
                    else setRTC = 0;
                }
                else if(disCtrl==DIS_PARA)
                {
                    if(setPara==0)setPara++;
                    else setPara = 0;
                }
            }
            else if(key==2)//+
            {
                if(setRTC)
                {
                    if(setRTC==1)calendar.w_year++;
                    if(setRTC==2){if(calendar.w_month<12)calendar.w_month++;else calendar.w_month=1;}
                    if(setRTC==3){if(calendar.w_date<31)calendar.w_date++; else calendar.w_date = 1;}
                    if(setRTC==4){if(calendar.hour<23)calendar.hour++; else calendar.hour=0;}
                    if(setRTC==5){if(calendar.min<59)calendar.min++;else calendar.min=0;}
                    if(setRTC==6){if(calendar.sec<59)calendar.sec++;else calendar.sec=0;}
                }
                else if(setPara)
                {
                    if(setPara==1){if(stepMax<999)stepMax++;}
                    else if(setPara==2){if(tmpMin<100)tmpMin++;}
                    else if(setPara==3){if(tmpMax<100)tmpMax++;}
                    else if(setPara==4){if(hrMin<200)hrMin++;}
                    else if(setPara==5){if(hrMax<200)hrMax++;}                    
                }
                else 
                {
                    if(enable_hr)enable_hr=0;
                    else enable_hr = 1;
                }
                
            }
            else if(key==0xF2)
            {
                if(setPara)
                {
                    if(setPara==1){if(stepMax<999)stepMax+=10;}
                    else if(setPara==2){if(tmpMin<100)tmpMin+=10;}
                    else if(setPara==3){if(tmpMax<100)tmpMax+=10;}
                    else if(setPara==4){if(hrMin<200)hrMin+=10;}
                    else if(setPara==5){if(hrMax<200)hrMax+=10;}                    
                }                
            }
            else if(key==3) //-           
            { 
                if(setRTC)
                {
                    if(setRTC==1)if(calendar.w_year>2000)calendar.w_year--;
                    if(setRTC==2){if(calendar.w_month>1)calendar.w_month--;else calendar.w_month=12;}
                    if(setRTC==3){if(calendar.w_date>1)calendar.w_date--;else calendar.w_date=31; }
                    if(setRTC==4){if(calendar.hour)calendar.hour--; else calendar.hour=23;}
                    if(setRTC==5){if(calendar.min)calendar.min--;else calendar.min=59;}
                    if(setRTC==6){if(calendar.sec)calendar.sec--;else calendar.sec=59;}
                }
                else if(setPara)
                {
                    if(setPara==1){if(stepMax>1)stepMax--;}
                    else if(setPara==2){if(tmpMin>1)tmpMin--;}
                    else if(setPara==3){if(tmpMax>1)tmpMax--;}                    
                    else if(setPara==4){if(hrMin>1)hrMin--;}
                    else if(setPara==5){if(hrMax>1)hrMax--;}
                } 
                else
                {
                    if(enable_step)enable_step=0;
                    else enable_step= 1;
                }
            }
            else if(key==0xF3)
            {
                if(setPara)
                {
                    if(setPara==1){if(stepMax>10)stepMax-=10;}
                    else if(setPara==2){if(tmpMin>10)tmpMin-=10;}
                    else if(setPara==3){if(tmpMax>10)tmpMax-=10;}                    
                    else if(setPara==4){if(hrMin>10)hrMin-=10;}
                    else if(setPara==5){if(hrMax>10)hrMax-=10;}
                }                
            }
            else if(key==4)
            {
                ADXL345_step = 0;
                hr_dis = 0;
                if(alarm_step)alarm_step = 0;
            }
            
        }
        if(refresh)
        {
            refresh = 0;
            if(disCtrl == DIS_DEFAUL)
            {
				LCD_GotoTitle(0);LCD_Print("日期:");  
				LCD_Print("20");
				LCD_WrNUM(calendar.w_year/10%10);
				LCD_WrNUM(calendar.w_year%10);
				LCD_Print("年");
				LCD_WrNUM(calendar.w_month/10%10);
				LCD_WrNUM(calendar.w_month%10);
				LCD_Print("月");
				LCD_WrNUM(calendar.w_date/10%10);
				LCD_WrNUM(calendar.w_date%10);
				LCD_Print("日");
				
				LCD_GotoXY(0,0);LCD_Print("时间:");
				LCD_WrNUM(calendar.hour/10%10);
				LCD_WrNUM(calendar.hour%10);
				LCD_Print("时");
				LCD_WrNUM(calendar.min/10%10);
				LCD_WrNUM(calendar.min%10);
				LCD_Print("分");
				LCD_WrNUM(calendar.sec/10%10);
				LCD_WrNUM(calendar.sec%10);
				LCD_Print("秒");
               
                    

                
                if(alarm_step&&LED)
				{LCD_GotoXY(0,1);LCD_Print("    ");}
	
				else
				{LCD_GotoXY(0,1);LCD_Print("步数:");}
      
                if(enable_step)
                {
					LCD_GotoXY(4,1);
					LCD_WrNUM(ADXL345_step/100%10);
					LCD_WrNUM(ADXL345_step/10%10);
					LCD_WrNUM(ADXL345_step/1%10);
					                 
                }
				else
                {
					LCD_GotoXY(4,1);
					LCD_Print("---");
                   
                }
				
                if(alarm_tmp&&LED)
				{LCD_GotoXY(8,1);LCD_Print("    ");}
		
				else
				{LCD_GotoXY(8,1);LCD_Print("体温:");}
			
				
					LCD_WrNUM(tmp/10%10);
					LCD_WrNUM(tmp%10);
					LCD_Print("度");
					
         
                
                
                if(alarm_hr&&LED)
				{LCD_GotoXY(0,2);LCD_Print("    ");}
	
				else
				{LCD_GotoXY(0,2);LCD_Print("心率:");}
				
                if(enable_hr)
                { 
					LCD_GotoXY(4,2);
					LCD_WrNUM(hr_dis/100%10);
					LCD_WrNUM(hr_dis/10%10);
					LCD_WrNUM(hr_dis%10);                  
				}
                else
                {
					LCD_GotoXY(4,2);
					LCD_Print("---");
	
                    
                }
				LCD_Print("次/分");				               
            }
            else if(disCtrl == DIS_PARA)
            {
                //OLED_Print_F12(0,0,"    [参数设置]");
				LCD_GotoTitle(8);
				LCD_Print("[参数设置]");
				LCD_GotoXY(0,0);
				LCD_Print("步数->[,");
				LCD_WrNUM(stepMax/100%10);  
				LCD_WrNUM(stepMax/10%10);  
                if(setPara==1&&LED)temp = ('_');else
                temp = stepMax%10+'0';
				LCD_WrNUM(temp); 
				LCD_Print("]");	

				LCD_GotoXY(0,1);
				LCD_Print("体温->[,");
				
				LCD_WrNUM(tmpMin/10%10); if(setPara==2&&LED)temp = ('_');else
                temp = tmpMin%10+'0'; 
				LCD_WrNUM(temp);  
				LCD_Print(",");	
				
                LCD_WrNUM(tmpMax/10%10); if(setPara==3&&LED)temp = ('_');else
                temp = tmpMax%10+'0';
				LCD_WrNUM(temp);  
				LCD_Print("]");		





				LCD_GotoXY(0,2);
				LCD_Print("心率->[,");
				
				LCD_WrNUM(hrMin/10%10); if(setPara==4&&LED)temp = ('_');else
                temp = hrMin%10+'0';
				LCD_WrNUM(temp);  
				LCD_Print(",");	
				
				LCD_WrNUM(hrMax/100%10);
                LCD_WrNUM(hrMax/10%10); if(setPara==5&&LED)temp = ('_');else
                temp = hrMax%10+'0';
				LCD_WrNUM(temp);  
				LCD_Print("]");	
			 
				


                  
            }
            else if(disCtrl == DIS_RTC)
            {
                //OLED_Print_F12(0,0,"    [时间设置]");
				LCD_GotoTitle(8);
				LCD_Print("[时间设置]");
				
				LCD_GotoXY(0,0);
				LCD_Print("20");
				
				
				LCD_WrNUM(calendar.w_year/10%10);if(setRTC==1&&LED)temp = ('_');else
                temp = calendar.w_year%10+'0';
				LCD_WrNUM(temp);
				LCD_Print("/");
				
				
				LCD_WrNUM(calendar.w_month/10%10);if(setRTC==2&&LED)temp = ('_');else
                temp = calendar.w_month%10+'0';
				LCD_WrNUM(temp);
				LCD_Print("/");
				
				
				LCD_WrNUM(calendar.w_date/10%10);if(setRTC==3&&LED)temp = ('_');else
                temp = calendar.w_date%10+'0';
				LCD_WrNUM(temp);
				
				
				
				
				
				
				LCD_Print("  ");
				
      
 
                switch (calendar.week)
                {
                    case 1:
						LCD_Print("Mon.");
                        break;
                    case 2:LCD_Print("Mon.");
                       
                        break;
                    case 3:LCD_Print("Wed.");
                       
                        break;
                    case 4:LCD_Print("Thu.");
                       
                        break;
                    case 5:LCD_Print("Fri.");
                       
                        break;
                    case 6:LCD_Print("Sat.");
                       
                        break;
                    case 0:LCD_Print("Sun.");
                       
                        break;
                    default:LCD_Print("Err.");
                      
                        break;
                } 
				
				LCD_GotoXY(0,1);
				LCD_WrNUM(calendar.hour/10%10);if(setRTC==4&&LED)temp = ('_');else
                temp = calendar.hour%10+'0';
				LCD_WrNUM(temp);
				
				LCD_Print(":");
				
				LCD_WrNUM(calendar.min/10%10);if(setRTC==5&&LED)temp = ('_');else
                temp = calendar.min%10+'0';
				LCD_WrNUM(temp);
				
				LCD_Print(":");
				
				LCD_WrNUM(calendar.sec/10%10);if(setRTC==6&&LED)temp = ('_');else
                temp = calendar.sec%10+'0';
				LCD_WrNUM(temp);			
                
				               
            }
            else
            {
            
				LCD_GotoXY(0,0);
				LCD_Print("default interface");
            }
        }
        
        
    }     
}


void writeFlash(void)
{
    Flash_Write(eep,LEN);
}

void readFlash(void)
{
    if(0==Flash_Read(eep,LEN))
    {

    }
    else
    {
        stepMax = 10;
        tmpMin = 15;
        tmpMax = 39;
        hrMax = 100;
        hrMin = 50;
    }
}

////////////////////////////////////////////////////////////////////
void sendAPP(void)
{
    static u8 step=0;
    static u8 psc=0;
    char str1[20],str2[20],str3[20];
    
	//分频执行，降低发送速度
	psc++;
	if(psc < 5)return;
	else psc = 0;
 

    if(step== 0)APPsendLable0("步数","体温","心率","","","");
    else if(step== 3)APPsendLable1("step:","Tmin","Tmax","HRmin","HRmax","","","");
    else if(step==6)APPsendLable2("","","","");
    else if(step==9)APPsendTitle("智能手环");
    else
    {
        //ADXL345_step ,tmp ,heartRate
        sprintf(str1,"%d",ADXL345_step);
        sprintf(str2,"%d",tmp);
        sprintf(str3,"%d",heartRate);
        APPsendValue(str1,str2,str3,"","","");
    }
    step++;
    if(step>10)step=0;
}



void revAPP(void)
{
    if(revPara_F)
    {
        revPara_F=0;
        Beep(200);
        if(revDate[0])
        stepMax= revDate[0]; 
        
		if(revDate[1])
        tmpMin = revDate[1]; 
		
		if(revDate[2])
		tmpMax = revDate[2]; 
		
		if(revDate[3])
        hrMin = revDate[3]; 
		
		if(revDate[4])
		hrMax = revDate[4]; 

        
    }   
}

