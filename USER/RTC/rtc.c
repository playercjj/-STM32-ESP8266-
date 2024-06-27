#include "rtc.h"
#include "led.h"
#include "usart1.h"
/*********************************************************************************
************************�������� STM32F407���Ŀ�����******************************
**********************************************************************************
* �ļ�����: rtc.c                                                                *
* �ļ�������RTC��ʼ����ʹ��                                                      *
* �������ڣ�2017.08.30                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����RTC��ʼ����ʹ��                                                      * 
**********************************************************************************
*********************************************************************************/


extern uint16_t Alarm_mark;//���ӱ�־λ


RTC_HandleTypeDef RTC_Handler;  //RTC���

//RTCʱ�����ݽṹ�壬ʱ������ڷֿ�
RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

//NVIC_InitTypeDef   NVIC_InitStructure;

									 
u8 const month_amendBuf[12]={0,3,3,6,1,4,6,2,5,0,3,5}; 
/****************************************************************************
* ��    ��: u8 RTC_GetWeek(u16 wyear,u8 wmonth,u8 wday)
* ��    �ܣ����ĳ�������ڼ�
* ��ڲ�����wyear����(���99)  wmonth����  wday����
* ���ز��������ڼ�
* ˵    ������2000~2099��Ч
            ����ֵ1-7���ζ�Ӧ����һ��������  
            �ú�����������ʱ��ʱ�������������ڼ���ȡʱ�����ڼ�ʱ����ʹ�øú�����
            ��ֱ�Ӷ�ȡ���ڼĴ���������ֵ����
            ע���βΡ��ꡱ��8λ����Ҫ����2λ����Ĳ���  15���Ϸ�    2015�����Ϸ�
****************************************************************************/																						 
u8 RTC_GetWeek(u8 wyear,u8 wmonth,u8 wday)
{	
	u16 middata;
	u8 yearL;
	
	yearL=wyear+100; 	 //��2000�꿪ʼ����100

	middata=yearL+yearL/4;
	middata=middata%7; 
	middata=middata+wday+month_amendBuf[wmonth-1];
	if (yearL%4==0&&wmonth<3)middata--;
	
	if((middata%7==0)) return 7;       //��STM32F103��rtc���̲�ͬ���ú��������췵��ֵΪ7
	return(middata%7);
}	
/****************************************************************************
* ��    ��: ErrorStatus RTC_SetTimes(u8 year,u8 month,u8 date,u8 hour,u8 min,u8 sec)
* ��    �ܣ�����RTCʱ��
* ��ڲ�����������ʱ����
* ���ز������ɹ����  1���ɹ�   0��ʧ��
* ˵    ����ע���βΡ��ꡱ��8λ����Ҫ����2λ����Ĳ���  15���Ϸ�    2015�����Ϸ�    
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
* ��    ��: void RTC_GetTimes(uint32_t RTC_Format)
* ��    �ܣ���ȡRTCʱ��
* ��ڲ�����RTC_Format����ȡʱ�����ݵ���������  RTC_Format_BIN���ֽ���  RTC_Format_BCD��BCD����������
* ���ز�������
* ˵    ����     
****************************************************************************/
void RTC_GetTimes(uint32_t RTC_Format)
{
	HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_Format);
	HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_Format);
}

//RTC��ʼ������
u8 RTC_InitConfig(void)
{
	 RTC_Handler.Instance=RTC;
    RTC_Handler.Init.HourFormat=RTC_HOURFORMAT_24;//RTC����Ϊ24Сʱ��ʽ 
    RTC_Handler.Init.AsynchPrediv=0X7F;           //RTC�첽��Ƶϵ��(1~0X7F)
    RTC_Handler.Init.SynchPrediv=0XFF;            //RTCͬ����Ƶϵ��(0~7FFF)   
    RTC_Handler.Init.OutPut=RTC_OUTPUT_DISABLE;     
    RTC_Handler.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
    RTC_Handler.Init.OutPutType=RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&RTC_Handler)!=HAL_OK) return 2;
      
   
      RTC_SetTimes(20,9,6,12,00,00,RTC_HOURFORMAT12_PM);   //��������ʱ�� ,����ʵ��ʱ���޸�

    
    return 0;
}

/****************************************************************************
* ��    ��: void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
* ��    �ܣ�RTC�ײ�������ʱ������
* ��ڲ�����hrtc:RTC���
* ���ز�������
* ˵    �����˺����ᱻHAL_RTC_Init()����    
****************************************************************************/
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();//ʹ�ܵ�Դʱ��PWR
	  HAL_PWR_EnableBkUpAccess();//ȡ����������д����
    
    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;//LSE����
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState=RCC_LSE_ON;                  //RTCʹ��LSE
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;//����ΪRTC
    PeriphClkInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;//RTCʱ��ԴΪLSE
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
        
    __HAL_RCC_RTC_ENABLE();//RTCʱ��ʹ��
}

/****************************************************************************
* ��    ��: void RTC_SetWakeUp(u32 wktime,u16 autodata)
* ��    �ܣ����û��Ѷ�ʱ����ʱ��
* ��ڲ�����wktime������ʱ��   autodata���Զ���װ��ֵ
* ���ز�������
* ˵    �������øú����Ļ���ʱ��Ϊ1S�ж�һ�Σ��൱��STM32F103�����е����ж�     
****************************************************************************/
void RTC_Set_WakeUp(u32 wktime,u16 autodata)
{ 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_WUTF);//���RTC WAKE UP�ı�־
	
	HAL_RTCEx_SetWakeUpTimer_IT(&RTC_Handler,autodata,wktime);            //������װ��ֵ��ʱ�� 
	
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn,0x02,0x02); //��ռ���ȼ�1,�����ȼ�2
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}
/****************************************************************************
* ��    ��: void RTC_WKUP_IRQHandler(void)
* ��    �ܣ�RTC WAKE UP�жϷ�����
* ��ڲ�������
* ���ز�������
* ˵    �����൱�����ж�
****************************************************************************/
void RTC_WKUP_IRQHandler(void)
{    
   HAL_RTCEx_WakeUpTimerIRQHandler(&RTC_Handler); 							
}

//RTC WAKE UP�жϴ���
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    LED1=!LED1;
}

/****************************************************************************
* ��    ��: void RTC_SetAlarmA(u8 week,u8 hour,u8 min,u8 sec)
* ��    �ܣ���������ʱ��(����������,24Сʱ��)
* ��ڲ�����week:���ڼ�(1~7) ʱ����
* ���ز�������
* ˵    ���� 
****************************************************************************/
void RTC_SetAlarmA(u8 week,u8 hour,u8 min,u8 sec)
{ 
    RTC_AlarmTypeDef RTC_AlarmSturuct;
    
    RTC_AlarmSturuct.AlarmTime.Hours=hour;  //Сʱ
    RTC_AlarmSturuct.AlarmTime.Minutes=min; //����
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; //��
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;
    
    RTC_AlarmSturuct.AlarmMask=RTC_ALARMMASK_NONE;//��ȷƥ�����ڣ�ʱ����
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;//������
    RTC_AlarmSturuct.AlarmDateWeekDay=week; //����
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     //����A
    HAL_RTC_SetAlarm_IT(&RTC_Handler,&RTC_AlarmSturuct,RTC_FORMAT_BIN);
    
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,0x01,0x02); //��ռ���ȼ�1,�����ȼ�2
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

//RTC�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{    
  HAL_RTC_AlarmIRQHandler(&RTC_Handler);										 
}
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	printf("���ӵ��ˣ��������ϰ�");
	Alarm_mark=1;
}



