#include "main.h"

#include "Eval.h"

#include "Contiki.h"

#define MotorForward        0x01
#define MotorReverse        0x02
#define MotorStop           0x03

typedef enum 
{
	Stop = 0,
	Reverse ,
	Forward,
} MotorStatus_t;

static uint8_t MotorStatus = 0;
static process_event_t MotorControlEvent = 0;

PROCESS(Motor_process, "Motor");

typedef struct 
{
	uint8_t CMD;
	uint8_t Buffer[32];
}Motor_ControlMsg;

void RTCApplicationInit(void);
void UartApplicationInit(void);
void RadioApplicationInit(void);

void MotorApplicationInit(void)
{
    MotorControlEvent = process_alloc_event();

	process_start(&Motor_process, NULL);
}

void Motor_Control(uint8_t *CmdBuffer)
{
	Motor_ControlMsg *Msg;

	Msg = (Motor_ControlMsg *)CmdBuffer;
	switch (Msg->CMD)
	{
		case MotorForward:
			MotorStatus = Forward;
			process_post(&Motor_process, MotorControlEvent, NULL);
			break;
		case MotorReverse:
			MotorStatus = Reverse;
			process_post(&Motor_process, MotorControlEvent, NULL);
			break;
		case MotorStop:
			MotorStatus = Stop;
			process_post(&Motor_process, MotorControlEvent, NULL);
			break;
	}
}

PROCESS_THREAD(Motor_process, ev, data)
{
	static struct etimer et;

	PROCESS_BEGIN();

	while(1)
	{
        PROCESS_WAIT_EVENT_UNTIL(ev == MotorControlEvent);

		DEBUG_Log ("Motor_process\r\n");
		etimer_set(&et, CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		switch (MotorStatus)
		{
			case Stop:
				DEBUG_Log ("MotorStop\r\n");
				break;
			case Reverse:
				DEBUG_Log ("MotorReverse\r\n");
				break;
			case Forward:
				DEBUG_Log ("MotorForward\r\n");
				break;
		}
	}

	PROCESS_END();
	// 宏用于辅助保存断点信息 (即行数 )
}



