#include "rtc.h"
#include "led.h"
#include "usart1.h"
/*********************************************************************************
************************启明欣欣 STM32F407核心开发板******************************
**********************************************************************************
* 文件名称: rtc.c                                                                *
* 文件简述：RTC初始化与使用                                                      *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：RTC初始化与使用                                                      * 
**********************************************************************************
*********************************************************************************/


extern uint16_t Alarm_mark;//闹钟标志位


RTC_HandleTypeDef RTC_Handler;  //RTC句柄

//RTC时钟数据结构体，时间跟日期分开
RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

//NVIC_InitTypeDef   NVIC_InitStructure;

									 
u8 const month_amendBuf[12]={0,3,3,6,1,4,6,2,5,0,3,5}; 
/****************************************************************************
* 名    称: u8 RTC_GetWeek(u16 wyear,u8 wmonth,u8 wday)
* 功    能：获得某天是星期几
* 入口参数：wyear：年(最大99)  wmonth：月  wday：日
* 返回参数：星期几
* 说    明：从2000~2099有效
            返回值1-7依次对应星期一到星期天  
            该函数用于设置时间时，无需输入星期几，取时间星期几时无需使用该函数，
            因直接读取日期寄存器的星期值就行
            注意形参“年”是8位，即要输入2位的年的参数  15：合法    2015：不合法
****************************************************************************/																						 
u8 RTC_GetWeek(u8 wyear,u8 wmonth,u8 wday)
{	
	u16 middata;
	u8 yearL;
	
	yearL=wyear+100; 	 //从2000年开始，加100

	middata=yearL+yearL/4;
	middata=middata%7; 
	middata=middata+wday+month_amendBuf[wmonth-1];
	if (yearL%4==0&&wmonth<3)middata--;
	
	if((middata%7==0)) return 7;       //与STM32F103的rtc例程不同，该函数星期天返回值为7
	return(middata%7);
}	
/****************************************************************************
* 名    称: ErrorStatus RTC_SetTimes(u8 year,u8 month,u8 date,u8 hour,u8 min,u8 sec)
* 功    能：设置RTC时间
* 入口参数：年月日时分秒
* 返回参数：成功与否  1：成功   0：失败
* 说    明：注意形参“年”是8位，即要输入2位的年的参数  15：合法    2015：不合法    
****************************************************************************/	
ErrorStatus RTC_SetTimes(u8 year,u8 month,u8 date,u8 hour,u8 min,u8 sec,u8 ampm)
{
  RTC_DateTypeDef RTC_DateTypeInitStructure;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_DateTypeInitStructure.Date=date;
	RTC_DateTypeInitStructure.Month=month;
	RTC_DateTypeInitStructure.WeekDay=RTC_GetWeek(year,month,date);
//	RTC_DateTypeInitStructure.WeekDay=week;
	RTC_DateTypeInitStructure.Year=year;
  
	HAL_RTC_SetDate(&RTC_Handler,&RTC_DateTypeInitStructure,RTC_FORMAT_BIN);
	
	RTC_TimeTypeInitStructure.Hours=hour;
	RTC_TimeTypeInitStructure.Minutes=min;
	RTC_TimeTypeInitStructure.Seconds=sec;
	
	RTC_TimeTypeInitStructure.TimeFormat=ampm;
//	RTC_TimeTypeInitStructure.DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
//  RTC_TimeTypeInitStructure.StoreOperation=RTC_STOREOPERATION_RESET;
	
	HAL_RTC_SetTime(&RTC_Handler,&RTC_TimeTypeInitStructure,RTC_FORMAT_BIN);
	
	return 0;
}
/****************************************************************************
* 名    称: void RTC_GetTimes(uint32_t RTC_Format)
* 功    能：读取RTC时间
* 入口参数：RTC_Format：读取时间数据的数据类型  RTC_Format_BIN：字节型  RTC_Format_BCD：BCD码数据类型
* 返回参数：无
* 说    明：     
****************************************************************************/
void RTC_GetTimes(uint32_t RTC_Format)
{
	HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_Format);
	HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_Format);
}

//RTC初始化函数
u8 RTC_InitConfig(void)
{
	 RTC_Handler.Instance=RTC;
    RTC_Handler.Init.HourFormat=RTC_HOURFORMAT_24;//RTC设置为24小时格式 
    RTC_Handler.Init.AsynchPrediv=0X7F;           //RTC异步分频系数(1~0X7F)
    RTC_Handler.Init.SynchPrediv=0XFF;            //RTC同步分频系数(0~7FFF)   
    RTC_Handler.Init.OutPut=RTC_OUTPUT_DISABLE;     
    RTC_Handler.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
    RTC_Handler.Init.OutPutType=RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&RTC_Handler)!=HAL_OK) return 2;
      
   
      RTC_SetTimes(20,9,6,12,00,00,RTC_HOURFORMAT12_PM);   //设置日期时间 ,根据实际时间修改

    
    return 0;
}

/****************************************************************************
* 名    称: void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
* 功    能：RTC底层驱动，时钟配置
* 入口参数：hrtc:RTC句柄
* 返回参数：无
* 说    明：此函数会被HAL_RTC_Init()调用    
****************************************************************************/
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();//使能电源时钟PWR
	  HAL_PWR_EnableBkUpAccess();//取消备份区域写保护
    
    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;//LSE配置
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState=RCC_LSE_ON;                  //RTC使用LSE
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;//外设为RTC
    PeriphClkInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;//RTC时钟源为LSE
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
        
    __HAL_RCC_RTC_ENABLE();//RTC时钟使能
}

/****************************************************************************
* 名    称: void RTC_SetWakeUp(u32 wktime,u16 autodata)
* 功    能：设置唤醒定时器的时间
* 入口参数：wktime：唤醒时间   autodata：自动重装载值
* 返回参数：无
* 说    明：设置该函数的唤醒时间为1S中断一次，相当于STM32F103例程中的秒中断     
****************************************************************************/
void RTC_Set_WakeUp(u32 wktime,u16 autodata)
{ 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_WUTF);//清除RTC WAKE UP的标志
	
	HAL_RTCEx_SetWakeUpTimer_IT(&RTC_Handler,autodata,wktime);            //设置重装载值和时钟 
	
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn,0x02,0x02); //抢占优先级1,子优先级2
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}
/****************************************************************************
* 名    称: void RTC_WKUP_IRQHandler(void)
* 功    能：RTC WAKE UP中断服务函数
* 入口参数：无
* 返回参数：无
* 说    明：相当于秒中断
****************************************************************************/
void RTC_WKUP_IRQHandler(void)
{    
   HAL_RTCEx_WakeUpTimerIRQHandler(&RTC_Handler); 							
}

//RTC WAKE UP中断处理
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    LED1=!LED1;
}

/****************************************************************************
* 名    称: void RTC_SetAlarmA(u8 week,u8 hour,u8 min,u8 sec)
* 功    能：设置闹钟时间(按星期闹铃,24小时制)
* 入口参数：week:星期几(1~7) 时分秒
* 返回参数：无
* 说    明： 
****************************************************************************/
void RTC_SetAlarmA(u8 week,u8 hour,u8 min,u8 sec)
{ 
    RTC_AlarmTypeDef RTC_AlarmSturuct;
    
    RTC_AlarmSturuct.AlarmTime.Hours=hour;  //小时
    RTC_AlarmSturuct.AlarmTime.Minutes=min; //分钟
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; //秒
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;
    
    RTC_AlarmSturuct.AlarmMask=RTC_ALARMMASK_NONE;//精确匹配星期，时分秒
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;//按星期
    RTC_AlarmSturuct.AlarmDateWeekDay=week; //星期
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     //闹钟A
    HAL_RTC_SetAlarm_IT(&RTC_Handler,&RTC_AlarmSturuct,RTC_FORMAT_BIN);
    
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,0x01,0x02); //抢占优先级1,子优先级2
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

//RTC闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{    
  HAL_RTC_AlarmIRQHandler(&RTC_Handler);										 
}
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	printf("闹钟到了，快起来上班");
	Alarm_mark=1;
}



