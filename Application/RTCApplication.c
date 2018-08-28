#include "main.h"

#include "Eval.h"

#include "Contiki.h"

PROCESS(RTC_process, "RTC");

void RTCApplicationInit(void)
{
	process_start(&RTC_process, NULL);
}

void RTC11111(void)
{
	DEBUG_Log("RTC11111\r\n");
}

PROCESS_THREAD(RTC_process, ev, data)
{
	static struct etimer et;
	Calendar_t calendar;

	PROCESS_BEGIN();

	RTC_AlarmSetIrq(Stm32RTC, RTC11111);
	RTC_GetCalendar(Stm32RTC, &calendar);
	calendar.min = 11;
	calendar.sec = 0;
	RTC_AlarmConfig(Stm32RTC, calendar);

	while(1)
	{
		etimer_set(&et, CLOCK_SECOND/2);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

	}

	PROCESS_END();
}



