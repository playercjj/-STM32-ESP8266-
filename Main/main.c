#include "led.h"
#include "key.h"
#include "lcd.h"
#include "rtc.h"
#include "usart.h"
#include "Beep.h"
#include "DHT11.h"
/*********************************************************************************
************************启明欣欣 STM32F407ZGT6核心板******************************
**********************************************************************************
* 文件名称: 例程16 RTC主函数main()                                               *
* 文件简述：RTC时钟显示实验                                                      *
* 创建日期：2018.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：RTC时钟显示                                                          *
* 淘宝店铺：https://shop125046348.taobao.com                                     *
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/

extern uint16_t Alarm_mark;
u8  *weekdate[7]={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
u8  *set_option[6]={"Year","Month","Day","Hours","Minutes","Seconds"};

u8 tempearture[2];//存放温度
u8 humidity[2];//存放湿度
u8 temp;//温度值
u8 hum;//湿度值

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
	if(shanshuo%50==0)  //提示现在要设置的选项
	  {
			LCD_DisplayString_color(20,185,24,"Please Set          ",BLUE,WHITE);
		  //LCD_DisplayString_color(152,185,24,(u8 *)set_option[option],BLUE,WHITE);
		}
		
		//设置时数值的加减1
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
		//闪烁显示：显示
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
		//闪烁显示：不显示
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
	
  u8 process=0;  //程序流程走向
  u8 option;     //调整时间选项
	u8 t=0,j;      //计时参数
	
  HAL_Init();                    	//初始化HAL库    
  Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init();     //延时函数初始化
	LED_Init();				//LED初始化
	KEY_Init();       //按键初始化
 	LCD_Init();           //初始化LCD FSMC接口和显示驱动
	RTC_InitConfig();		 	//初始化RTC
	RTC_Set_WakeUp(RTC_WAKEUPCLOCK_CK_SPRE_16BITS,0); //配置WAKE UP中断,1秒钟中断一次  
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
			case 0:   // 流程0：时钟显示
			     {
						 if(key_tem==4&&key_time>250)
							{
								process=1;   //长按KEY3 进入调整时间流程
								break;
							}
						 if((t%20)==0)	//每250ms更新显示
							{
								
								/*刷新时间*/
								RTC_GetTimes(RTC_FORMAT_BIN);
								Time_Display();
								
								/*刷新温湿度*/
								if(DHT11_Read_Data(tempearture,humidity)==0)
								{
									//读取成功
									//LCD_DisplayNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);
									//显示 数字
									
									LCD_DisplayNum(160,10,tempearture[0],2,24,1);
									LCD_DisplayNum(115,40,humidity[0],2,24,1);
									//printf("温度：%d,湿度：%d",tempearture[0],humidity[0]);
								
								}
								
							} 
						break;
			     }
			case 1:    // 流程1：调整时间
					{
					  if(keydown_data==KEY2_DATA) //按KEY2选项右移
						  {
						   option++;
							 if(option>5)   //循环
								 option=0;
						  }					
						Adjust_Time(option,t);//调整时间函数
						 if(keydown_data==KEY3_DATA)
						 {
						   Time_Display();// 防止在闪烁不显示的时候，短按KEY3确认调整后 显示空
							 RTC_SetTimes(RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date,RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds,RTC_HOURFORMAT12_PM);
							 RTC_GetTimes(RTC_FORMAT_BIN);
							 Time_Display();
							 LCD_DisplayString_color(20,185,24,"Adjust OK          ",BLUE,WHITE);
							 for(j=0;j<100;j++) delay_ms(10);  // Adjust OK 调整OK显示1秒
							 LCD_DisplayString(20,185,24,"                    ");
							 option=0;    //选项从头来
							 process=0;   //短按KEY3时间设置完成 返回到时间显示
							 break;
						 }
						break;
					}
	  }
		delay_ms(5); //系统延时，也算是系统计时
	   if(uart_byte_count!=0)//串口1
	  {
		u8 temp=uart_byte_count;
		  HAL_Delay(10);
		  if(temp==uart_byte_count)
		  {
			usart_disp();
			uart_byte_count=0;  
		  }
	  }
	   if(uart2_byte_count!=0)//串口2
	  {
		u8 temp2=uart2_byte_count;
		HAL_Delay(10);
		  if(temp2==uart2_byte_count)
		  {
			usart2_disp();
			uart2_byte_count=0;  
		  }
	  }
	  /*有闹钟需要处理*/
	  if(Alarm_mark==1)
	  {
		play_music();
		Alarm_mark=0;
	  }
	}
}


