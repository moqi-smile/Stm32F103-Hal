#include "main.h"

#include "Eval.h"

#include "Contiki.h"
#include "String.h"

#define RECV_LENTH   255

uint8_t Debug_Data = 0;
uint8_t Debug_Index = 0;
uint8_t Debug_Buffer[RECV_LENTH];

static struct ctimer DebugUart_Ctime;

void DebugUartIrq(void)
{
    ctimer_restart(&DebugUart_Ctime);

	Debug_Buffer[Debug_Index++] = Debug_Data;
	UART_ReceiveIT(User_DEBUGUart, &Debug_Data, 1);
}


void DebugUart_CtimeTask(void * arg)
{
	if (Debug_Index != 0)
	{
		DEBUG_Log("Debug_Index = %d\r\n", Debug_Index);
		UART_Transmit(User_DEBUGUart, Debug_Buffer, Debug_Index, 10);
	}

	Debug_Data = 0;
	Debug_Index = 0;
	memset(Debug_Buffer, 0, RECV_LENTH);
	UART_ReceiveIT(User_DEBUGUart, &Debug_Data, 1);
}

void UartApplicationInit(void)
{
	UART_SetIrqCallback(User_DEBUGUart, DebugUartIrq);

	ctimer_set(&DebugUart_Ctime, 30, DebugUart_CtimeTask, NULL);
}




