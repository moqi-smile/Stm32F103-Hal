#include "main.h"

#include "Eval.h"

#include "Contiki.h"
#include "String.h"

#include "sx1276.h"
#include "sx1276-Hal.h"

#define BUFFER_SIZE			255
// Define the payload size here
#define ControlNode         0x01
#define SetGroupID          0x05


static uint16_t BufferSize = BUFFER_SIZE;
// RF buffer size
static uint8_t  Buffer[BUFFER_SIZE];
// RF buffer

static uint16_t RadioData_Index = BUFFER_SIZE;
// RF buffer size
static uint8_t  RadioData_Buf[BUFFER_SIZE];

uint32_t NetADDR = 0;

PROCESS(RadioRx_process, "RadioRx");
PROCESS(RadioTx_process, "RadioTx");

static process_event_t Radio_RecvMsg = 0;

typedef struct 
{
	uint8_t len;
	uint8_t ID[5];
	uint8_t CMD;
	uint8_t Buffer[32];
}Radio_CmdStruct;

void DIO0_Irq(void);

void Motor_Control(uint8_t *CmdBuffer);

void RadioApplicationInit(void)
{
	SX1276Init();

	SX1276StartRx();

	Gpio_SetIrq(RADIO_DIO0, DIO0_Irq, 8);

    Radio_RecvMsg = process_alloc_event();

    process_start(&RadioRx_process, NULL);
    process_start(&RadioTx_process, NULL);
}

void Node_Cmd(Radio_CmdStruct Msg)
{
	switch (Msg.CMD)
	{
		case ControlNode:
			Motor_Control(Msg.Buffer);
			break;

		case SetGroupID:
			SetNodeID(Msg.Buffer);
			break;
	}
}

PROCESS_THREAD(RadioRx_process, ev, data)
{
	Radio_CmdStruct *msg = NULL;

    PROCESS_BEGIN();

    while(1)
    {
        PROCESS_WAIT_EVENT_UNTIL(ev == Radio_RecvMsg);

		msg = (Radio_CmdStruct *)Buffer;

		if (RadioData_Index == msg->len+1)
		{
			if (memcmp (msg->ID, NodeAddr, 5) == 0)
			{
				DEBUG_Log ("Buffer = %.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X\r\n",
					Buffer[0], Buffer[1], Buffer[2], Buffer[3], Buffer[4], Buffer[5], Buffer[6], Buffer[7]);
				Node_Cmd(*msg);
			}
		}
    }

    PROCESS_END();
    // 宏用于辅助保存断点信息 (即行数 )
}

PROCESS_THREAD(RadioTx_process, ev, data)
{
	static struct etimer et;
    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND/2);

    while(1)
    {
        PROCESS_YIELD();

    }

    PROCESS_END();
}

void DIO0_Irq(void)
{
    switch (SX1276TxRxRUNNING())
    {
        case RF_TX_DONE:
            break;
        case RF_RX_DONE:
            if (SX1276RXDONE() == RF_RX_DONE)
            {
                SX1276GetRxPacket( Buffer, ( uint16_t* )&BufferSize );

				memset(RadioData_Buf, 0, 255);
				memcpy((char*)RadioData_Buf, (const char*)Buffer, BufferSize);
                RadioData_Index = BufferSize;

                process_post(&RadioRx_process, Radio_RecvMsg, NULL);
			}
            break;
        default:
            break;
    }
    SX1276StartRx();
}

