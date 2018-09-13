#include "main.h"

#include "Eval.h"

#include "Contiki.h"
#include "String.h"

#include "sx1276.h"
#include "sx1276-Hal.h"

PROCESS(RadioRx_process, "RadioRx");
PROCESS(RadioTx_process, "RadioTx");

#include "radio.h"

#define Radio_Tx

#define BUFFER_SIZE                                 9 // Define the payload size here

tRadioDriver *Radio = NULL;
static uint16_t BufferSize = BUFFER_SIZE;			// RF buffer size
static uint8_t Buffer[BUFFER_SIZE];					// RF buffer

static uint16_t RecvPackNum = 0;
static uint16_t TransmitPackNum = 0;

void DIO0_Irq(void);

double SX1276GetPacketRssi( void );

void RadioApplicationInit(void)
{
    process_start(&RadioRx_process, NULL);
    process_start(&RadioTx_process, NULL);

    Radio = RadioDriverInit( );

	Gpio_SetIrq(RADIO_DIO0, DIO0_Irq, 8);

	Radio->Init( );
	Radio->StartRx( );
}

void DIO0_Irq(void)
{
	switch (Radio->TxRxRunnin())
	{
		case RF_TX_DONE:
			Radio->StartRx( );
			break;
		case RF_RX_DONE:
			Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );
			if ( BufferSize > 0 )
			{
#ifdef  Radio_Tx
				if( strncmp( ( const char* )Buffer, ( const char* )"pong", 4 ) == 0 )
				{
					RecvPackNum = (((uint16_t)Buffer[4])<<8) | Buffer[5];
					process_poll(&RadioTx_process);
				}
#else
				if( strncmp( ( const char* )Buffer, ( const char* )"ping", 4 ) == 0 )
				{
					RecvPackNum = (((uint16_t)Buffer[4])<<8) | Buffer[5];
					TransmitPackNum++;
					process_poll(&RadioTx_process);
				}
#endif
			}
			break;
		default:
			break;
	}
}

PROCESS_THREAD(RadioRx_process, ev, data)
{
#ifdef  Radio_Tx
	static struct etimer et;

	memset (Buffer, 0, BUFFER_SIZE);
	memcpy (Buffer, "ping", 4);
#endif

	PROCESS_BEGIN();

	while(1)
	{
#ifdef  Radio_Tx
		etimer_set(&et, CLOCK_SECOND*3);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		TransmitPackNum ++;
		Buffer[4] = (uint8_t)(TransmitPackNum>>8);
		Buffer[5] = (uint8_t)TransmitPackNum;

		Radio->SetTxPacket( Buffer, 6 );
		DEBUG_Log  ("TransmitPackNum\r\n");
#else
        PROCESS_YIELD();
#endif

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

#ifdef  Radio_Tx
		DEBUG_Log  ("TransmitPackNum %.4X\r\n", TransmitPackNum);
		DEBUG_Log  ("RecvPackNum %.4X\r\n", RecvPackNum);
		DEBUG_Log  ("Rssi %.4f\r\n\r\n\r\n\r\n", SX1276GetPacketRssi());
#else
		DEBUG_Log  ("TransmitPackNum %.4X\r\n\r\n", TransmitPackNum);
		DEBUG_Log  ("RecvPackNum %.4X\r\n\r\n", RecvPackNum);
		DEBUG_Log  ("Rssi %.4f\r\n\r\n\r\n\r\n", SX1276GetPacketRssi());
		memset (Buffer, 0, BUFFER_SIZE);
		memcpy (Buffer, "pong", 4);
		Buffer[4] = (uint8_t)(TransmitPackNum>>8);
		Buffer[5] = (uint8_t)TransmitPackNum;
		Radio->SetTxPacket( Buffer, 6 );

		GpioPinRes(LED1);
		etimer_set(&et, CLOCK_SECOND/4);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		GpioPinSet(LED1);
#endif
    }

    PROCESS_END();
}

