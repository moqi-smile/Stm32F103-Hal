#ifndef __RTC_H__
#define __RTC_H__

typedef void (*RtcIrqCallback)(void);

typedef struct 
{
	__IO uint8_t hour;
	__IO uint8_t min;
	__IO uint8_t sec;			
	//公历日月年周
	__IO uint16_t year;
	__IO uint8_t  month;
	__IO uint8_t  date;
	__IO uint8_t  week;		 
}Calendar_t;


typedef enum 
{
	Stm32RTC = 0,
	DS1301,
	RTCnum,

} Rtc_t;

void Rtc_Init(void);
uint8_t RTC_SetCalendar(Rtc_t Rtcx, Calendar_t Calendar);
uint8_t RTC_GetCalendar(Rtc_t Rtcx, Calendar_t *Calendar);

void RTC_SecSetIrq(Rtc_t Rtcx, RtcIrqCallback Irq);

void RTC_AlarmSetIrq(Rtc_t Rtcx, RtcIrqCallback Irq);
void RTC_AlarmConfig(Rtc_t Rtcx, Calendar_t Calendar);

#endif
