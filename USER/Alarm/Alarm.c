#include "Alarm.h"
#include "rtc.h"
#include "usart1.h"


uint16_t Alarm_mark=0;//闹钟标志位

//Set Alarm 12-00-00
void Alarm_set(u8 *str)
{
	u8 hour=(str[10]-'0')*10+str[11]-'0';
	u8 min=(str[13]-'0')*10+str[14]-'0';
	u8 sec=(str[16]-'0')*10+str[17]-'0';
	u8 week=RTC_DateStruct.WeekDay;
	RTC_SetAlarmA(week,hour,min,sec);
	printf("闹钟设置成功");
}
