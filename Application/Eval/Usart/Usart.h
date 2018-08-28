#ifndef __UART_H__
#define __UART_H__

typedef enum 
{
	Uart1 = 0,
	Uart2,
	Uart3,
	Uart4,
	Uart5,
	Uartnum,

	User_DEBUGUart = Uart1,
	User_ClientUart = Uart3,
} Uart_t;

typedef void (*UartIrqCallback)(void);

void Uart_Init(void);
void UART_Transmit(Uart_t Uartx, uint8_t *pData, uint16_t Size, uint32_t Timeout);

void UART_SetIrqCallback(Uart_t Uartx, UartIrqCallback Irq);
void UART_ReceiveIT(Uart_t Uartx, uint8_t *pData, uint16_t Size);

#define UARTn Uartnum

#endif
