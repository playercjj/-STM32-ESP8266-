#include "led.h"
#include "key.h"
#include "lcd.h"
#include "rtc.h"
#include "usart.h"
#include "Beep.h"
#include "DHT11.h"
/*********************************************************************************
************************�������� STM32F407ZGT6���İ�******************************
**********************************************************************************
* �ļ�����: ����16 RTC������main()                                               *
* �ļ�������RTCʱ����ʾʵ��                                                      *
* �������ڣ�2018.08.30                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����RTCʱ����ʾ                                                          *
* �Ա����̣�https://shop125046348.taobao.com                                     *
* ��    ���������̴��������ѧϰ�ο�                                             *
**********************************************************************************
*********************************************************************************/

extern uint16_t Alarm_mark;
u8  *weekdate[7]={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
u8  *set_option[6]={"Year","Month","Day","Hours","Minutes","Seconds"};

u8 tempearture[2];//����¶�
u8 humidity[2];//���ʪ��
u8 temp;//�¶�ֵ
u8 hum;//ʪ��ֵ

void Time_Display()
{
 LCD_DisplayString(30,220,16,"Date:20  -  -  ");
 LCD_DisplayNum(86,220,RTC_DateStruct.Year,2,16,1);
 LCD_DisplayNum(110,220,RTC_DateStruct.Month,2,16,1);
 LCD_DisplayNum(134,220,RTC_DateStruct.Date,2,16,1);
 LCD_DisplayString(30,240,16,"Time:  :  :     ");
 LCD_DisplayNum(70,240,RTC_TimeStruct.Hours,2,16,1);	
 LCD_DisplayNum(94,240,RTC_TimeStruct.Minutes,2,16,1);
 LCD_DisplayNum(118,240,RTC_TimeStruct.Seconds,2,16,1);
 LCD_DisplayString(30,260,16,"Week:        ");
 LCD_DisplayString(70,260,16,(u8 *)weekdate[RTC_DateStruct.WeekDay-1]);
}
void Adjust_Time(u8 option,u8 shanshuo)
{	
	if(shanshuo%50==0)  //��ʾ����Ҫ���õ�ѡ��
	  {
			LCD_DisplayString_color(20,185,24,"Please Set          ",BLUE,WHITE);
		  //LCD_DisplayString_color(152,185,24,(u8 *)set_option[option],BLUE,WHITE);
		}
		
		//����ʱ��ֵ�ļӼ�1
		if( (keydown_data==KEY0_DATA)||(keydown_data==KEY1_DATA))
		{
		  if(keydown_data==KEY0_DATA)
			{
			  switch(option)
				{
				  case 0: RTC_DateStruct.Year+=1;   break;
					case 1: RTC_DateStruct.Month+=1;  break;
					case 2: RTC_DateStruct.Date+=1;   break;
					case 3: RTC_TimeStruct.Hours+=1;  break;
					case 4: RTC_TimeStruct.Minutes+=1;break;
					case 5: RTC_TimeStruct.Seconds+=1;break;
				}	
			}
			if(keydown_data==KEY1_DATA)
			{
			  switch(option)
				{
				  case 0: RTC_DateStruct.Year-=1;   break;
					case 1: RTC_DateStruct.Month-=1;  break;
					case 2: RTC_DateStruct.Date-=1;   break;
					case 3: RTC_TimeStruct.Hours-=1;  break;
					case 4: RTC_TimeStruct.Minutes-=1;break;
					case 5: RTC_TimeStruct.Seconds-=1;break;
				}	
			}
		}
		//��˸��ʾ����ʾ
	if(shanshuo==150)
		{
			Time_Display();
//			LCD_DisplayNum(86,220,RTC_DateStruct.RTC_Year,2,16,1);
//			LCD_DisplayNum(110,220,RTC_DateStruct.RTC_Month,2,16,1);
//			LCD_DisplayNum(134,220,RTC_DateStruct.RTC_Date,2,16,1);
//			LCD_DisplayNum(70,240,RTC_TimeStruct.RTC_Hours,2,16,1);
//			LCD_DisplayNum(94,240,RTC_TimeStruct.RTC_Minutes,2,16,1);
//			LCD_DisplayNum(118,240,RTC_TimeStruct.RTC_Seconds,2,16,1);
		}	
		//��˸��ʾ������ʾ
	switch(option)
	  {  
			case 0: {  if(shanshuo==49)  LCD_DisplayString(86,220,16,"  ");	 break;  }
			case 1: {  if(shanshuo==49)  LCD_DisplayString(110,220,16,"  "); break;  }
			case 2: {  if(shanshuo==49)  LCD_DisplayString(134,220,16,"  "); break;  }
			case 3: {  if(shanshuo==49)  LCD_DisplayString(70,240,16,"   ");  break;  }
			case 4: {	 if(shanshuo==49)  LCD_DisplayString(94,240,16,"  ");	 break;	 }
			case 5: {	 if(shanshuo==49)  LCD_DisplayString(118,240,16,"     "); break;  }		
	  }
}

int main(void)
{ 
	
  u8 process=0;  //������������
  u8 option;     //����ʱ��ѡ��
	u8 t=0,j;      //��ʱ����
	
  HAL_Init();                    	//��ʼ��HAL��    
  Stm32_Clock_Init(336,8,2,7);  	//����ʱ��,168Mhz
	delay_init();     //��ʱ������ʼ��
	LED_Init();				//LED��ʼ��
	KEY_Init();       //������ʼ��
 	LCD_Init();           //��ʼ��LCD FSMC�ӿں���ʾ����
	RTC_InitConfig();		 	//��ʼ��RTC
	RTC_Set_WakeUp(RTC_WAKEUPCLOCK_CK_SPRE_16BITS,0); //����WAKE UP�ж�,1�����ж�һ��  
	uart1_init(9600);
	uart2_init(9600);
	BEEP_Init();
	HAL_Delay(1000);
	DHT11_Init();
	BRUSH_COLOR=RED;
	LCD_DisplayString(10,10,24,"tempearture:   C");	
	LCD_DisplayString(10,40,24,"humidity:  %");
	//LCD_DisplayString(30,70,24,"14.RTC TEST");
	LCD_DisplayString(30,100,16,"KEY3 Long :Adjust time");
	LCD_DisplayString(30,120,16,"     Short:Adjust OK");
	LCD_DisplayString(30,140,16,"KEY0: V++  KEY1: V--");
	LCD_DisplayString(30,160,16,"KEY2: Right Move");
	RTC_GetTimes(RTC_FORMAT_BIN);
	Time_Display();
  	while(1) 
	{		
		key_scan(0);	
		t++;

		switch(process)
		{
			case 0:   // ����0��ʱ����ʾ
			     {
						 if(key_tem==4&&key_time>250)
							{
								process=1;   //����KEY3 �������ʱ������
								break;
							}
						 if((t%20)==0)	//ÿ250ms������ʾ
							{
								
								/*ˢ��ʱ��*/
								RTC_GetTimes(RTC_FORMAT_BIN);
								Time_Display();
								
								/*ˢ����ʪ��*/
								if(DHT11_Read_Data(tempearture,humidity)==0)
								{
									//��ȡ�ɹ�
									//LCD_DisplayNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);
									//��ʾ ����
									
									LCD_DisplayNum(160,10,tempearture[0],2,24,1);
									LCD_DisplayNum(115,40,humidity[0],2,24,1);
									//printf("�¶ȣ�%d,ʪ�ȣ�%d",tempearture[0],humidity[0]);
								
								}
								
							} 
						break;
			     }
			case 1:    // ����1������ʱ��
					{
					  if(keydown_data==KEY2_DATA) //��KEY2ѡ������
						  {
						   option++;
							 if(option>5)   //ѭ��
								 option=0;
						  }					
						Adjust_Time(option,t);//����ʱ�亯��
						 if(keydown_data==KEY3_DATA)
						 {
						   Time_Display();// ��ֹ����˸����ʾ��ʱ�򣬶̰�KEY3ȷ�ϵ����� ��ʾ��
							 RTC_SetTimes(RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds,RTC_HOURFORMAT12_PM);
							 RTC_GetTimes(RTC_FORMAT_BIN);
							 Time_Display();
							 LCD_DisplayString_color(20,185,24,"Adjust OK          ",BLUE,WHITE);
							 for(j=0;j<100;j++) delay_ms(10);  // Adjust OK ����OK��ʾ1��
							 LCD_DisplayString(20,185,24,"                    ");
							 option=0;    //ѡ���ͷ��
							 process=0;   //�̰�KEY3ʱ��������� ���ص�ʱ����ʾ
							 break;
						 }
						break;
					}
	  }
		delay_ms(5); //ϵͳ��ʱ��Ҳ����ϵͳ��ʱ
	   if(uart_byte_count!=0)//����1
	  {
		u8 temp=uart_byte_count;
		  HAL_Delay(10);
		  if(temp==uart_byte_count)
		  {
			usart_disp();
			uart_byte_count=0;  
		  }
	  }
	   if(uart2_byte_count!=0)//����2
	  {
		u8 temp2=uart2_byte_count;
		HAL_Delay(10);
		  if(temp2==uart2_byte_count)
		  {
			usart2_disp();
			uart2_byte_count=0;  
		  }
	  }
	  /*��������Ҫ����*/
	  if(Alarm_mark==1)
	  {
		play_music();
		Alarm_mark=0;
	  }
	}
}


