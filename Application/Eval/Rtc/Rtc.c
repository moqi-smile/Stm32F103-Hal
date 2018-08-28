#include "main.h"

#include "Rtc/Rtc.h"

#define TimeZone				(8)
#define TimeDifference			(TimeZone * 3600)

uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};
// ���������ݱ�	  
const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

RTC_HandleTypeDef hrtc;
RtcIrqCallback AlarmIrqCallback[RTCnum];
RtcIrqCallback SecIrqCallback[RTCnum];

void Rtc_Init(void)
{
	hrtc.Instance = RTC;
	hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}

	if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2)
	{
		HAL_RTC_SetTimeStamp(&hrtc, 1426295450+TimeDifference);
		HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,0x32F2);
	}
	//HAL_RTCEx_SetSecond_IT(&hrtc);
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	if(hrtc->Instance==RTC)
	{
		HAL_PWR_EnableBkUpAccess();
		__HAL_RCC_BKP_CLK_ENABLE();
		__HAL_RCC_RTC_ENABLE();

		HAL_NVIC_SetPriority(RTC_IRQn, 10, 0);
		HAL_NVIC_EnableIRQ(RTC_IRQn);
		HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 10, 0);
	}
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
	if(hrtc->Instance==RTC)
	{
		__HAL_RCC_RTC_DISABLE();

		HAL_NVIC_DisableIRQ(RTC_IRQn);
		HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
	}
}

//�ж��Ƿ������꺯��
uint8_t Is_Leap_Year(uint16_t year)
{			  
	if(year%4==0)
		//�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)
				return 1;
			//�����00��β,��Ҫ�ܱ�400���� 	   
			else
				return 0;   
		}
		else
			return 1;   
	}
	else
		return 0;	
}

//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//������������������� 
//����ֵ�����ں�																						 
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day)
{	
	uint16_t temp2;
	uint8_t yearH,yearL;

	yearH=year/100;
	yearL=year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)
		yearL+=100;

	// ����������ֻ��1900��֮���  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];

	if (yearL%4==0&&month<3)
		temp2--;

	return(temp2%7);
}

uint32_t CalendarToTimeStamp(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec)
{
	uint16_t t;
	uint32_t seccount=0;
 
	for(t=1970;t<syear;t++)
	//��������ݵ��������
	{
		if(Is_Leap_Year(t))
			seccount+=31622400;
		//�����������
		else
			seccount+=31536000;
		//ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)
	//��ǰ���·ݵ����������
	{
		seccount+=(uint32_t)mon_table[t]*86400;
		// �·����������
		if(Is_Leap_Year(syear)&&t==1)
			seccount+=86400;
		//����2�·�����һ���������	   
	}
	seccount+=(uint32_t)(sday-1)*86400;
	//��ǰ�����ڵ���������� 
	seccount+=(uint32_t)hour*3600;
	//Сʱ������
    seccount+=(uint32_t)min*60;
	//����������
	seccount+=sec;
	//�������Ӽ���ȥ

	return seccount;
}

//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
static Calendar_t Stm32_RTCGetCalendar(void)
{
	static uint16_t daycnt=0;
	static Calendar_t calendar;
	uint32_t timecount=0; 
	uint32_t temp=0;
	uint16_t temp1=0;
	//ʱ�ӽṹ��

	HAL_RTC_GetTimeStamp(&hrtc, &timecount);

 	temp=timecount/86400;
	//�õ�����(��������Ӧ��)

	if(daycnt!=temp)//����һ����
	{
		daycnt=temp;
		temp1=1970;
		//��1970�꿪ʼ

		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))
				//������
			{
				if(temp>=366)
					temp-=366;
				//�����������
				else
				{
					temp1++;
					break;
				}  
			}
			else
				temp-=365;
			//ƽ�� 
			temp1++;  
		}   
		calendar.year=temp1;
		//�õ����
		temp1=0;
		while(temp>=28)
			//������һ����
		{
			if(Is_Leap_Year(calendar.year)&&temp1==1)
				//�����ǲ�������/2�·�
			{
				if(temp>=29)
					temp-=29;
				//�����������
				else
					break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])
					temp-=mon_table[temp1];
				//ƽ��
				else break;
			}
			temp1++;  
		}
		calendar.month=temp1+1;
		// �õ��·�
		calendar.date=temp+1;
		// �õ����� 
	}
	temp=timecount%86400;
	// �õ�������

	calendar.hour=temp/3600;
	// Сʱ
	calendar.min=(temp%3600)/60;
	// ����	
	calendar.sec=(temp%3600)%60;
	// ����

	calendar.week=RTC_Get_Week(calendar.year,calendar.month,calendar.date);
	// ��ȡ����   

	return calendar;
}

static uint8_t Stm32_RTCSetAlarm(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec)
{
	return 0;
}

uint8_t RTC_SetAlarm(Rtc_t Rtcx, Calendar_t Calendar)
{
	return Stm32_RTCSetAlarm(Calendar.year, Calendar.month, Calendar.date, Calendar.hour, Calendar.min, Calendar.sec);
}

uint8_t RTC_SetCalendar(Rtc_t Rtcx, Calendar_t Calendar)
{
	HAL_RTC_SetTimeStamp(&hrtc,
		CalendarToTimeStamp(Calendar.year, Calendar.month, Calendar.date, Calendar.hour, Calendar.min, Calendar.sec));
	return 0;
}

uint8_t RTC_GetCalendar(Rtc_t Rtcx, Calendar_t *Calendar)
{
	*Calendar = Stm32_RTCGetCalendar();

	return 0;
}

void RTC_AlarmConfig(Rtc_t Rtcx, Calendar_t Calendar)
{
	HAL_RTC_SetTimeStampAlarm(&hrtc,
		CalendarToTimeStamp(Calendar.year, Calendar.month, Calendar.date, Calendar.hour, Calendar.min, Calendar.sec));
}

void RTC_SecSetIrq(Rtc_t Rtcx, RtcIrqCallback Irq)
{
	SecIrqCallback[Rtcx] = Irq;
	HAL_RTCEx_SetSecond_IT(&hrtc);
}


void RTC_AlarmSetIrq(Rtc_t Rtcx, RtcIrqCallback Irq)
{
	AlarmIrqCallback[Rtcx] = Irq;
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	if (AlarmIrqCallback[Stm32RTC] != NULL)
		AlarmIrqCallback[Stm32RTC]();
}

void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
{
	if (SecIrqCallback[Stm32RTC] != NULL)
		SecIrqCallback[Stm32RTC]();
}

void RTC_IRQHandler(void)
{
	HAL_RTCEx_RTCIRQHandler(&hrtc);
}

void RTC_Alarm_IRQHandler(void)
{
	HAL_RTC_AlarmIRQHandler(&hrtc);
}
