#include "stm32f1xx_hal.h"
#include "main.h"

#include "usart/usart.h"
#include "Gpio/Gpio.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
UART_HandleTypeDef husart[Uartnum];
UartIrqCallback UartIrq[Uartnum];

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart);
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart);
static void MX_NVIC_USARTx_Init(void);
/* 函数体 --------------------------------------------------------------------*/

int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&husart[Uart1], (uint8_t*)&ch, 1,1000);
	return ch;
}

void UART_Transmit(Uart_t Uartx, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	HAL_UART_Transmit(&husart[Uartx], pData, Size, Timeout);
}

void UART_SetIrqCallback(Uart_t Uartx, UartIrqCallback Irq)
{
	UartIrq[Uartx] = Irq;
}

void UART_ReceiveIT(Uart_t Uartx, uint8_t *pData, uint16_t Size)
{
	HAL_UART_Receive_IT(&husart[Uartx], pData, Size);
}

void Uart_Init(void)
{
	/* 串口外设时钟使能 */
	__HAL_RCC_USART1_CLK_ENABLE();

	husart[Uart1].Instance = USART1;
	husart[Uart1].Init.Mode = UART_MODE_TX_RX;
	husart[Uart1].Init.Parity = UART_PARITY_NONE;
	husart[Uart1].Init.StopBits = UART_STOPBITS_1;
	husart[Uart1].Init.BaudRate = 115200;
	husart[Uart1].Init.WordLength = UART_WORDLENGTH_8B;
	husart[Uart1].Init.HwFlowCtl = UART_HWCONTROL_NONE;
	husart[Uart1].Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&husart[Uart1]);

	/* 配置串口中断并使能，需要放在HAL_UART_Init函数后执行修改才有效 */
	MX_NVIC_USARTx_Init();
}

/**
  * 函数功能: 串口硬件初始化配置
  * 输入参数: huart：串口句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	if(huart->Instance==USART1)
	{
		Gpio_Config(UART1_Tx, Gpio_ModeAFPP, Gpio_SpeedHigh, Gpio_NOPULL);
		Gpio_Config(UART1_Rx, Gpio_ModeInput, Gpio_SpeedHigh, Gpio_NOPULL);

		HAL_NVIC_SetPriority(USART1_IRQn, 9, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}

/**
  * 函数功能: 串口硬件反初始化配置
  * 输入参数: huart：串口句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
	if (huart->Instance==USART1)
	{
		/* 串口外设时钟禁用 */
		__HAL_RCC_USART1_CLK_DISABLE();

		/* 串口外设功能GPIO配置 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);

		/* 串口中断禁用 */
		HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
}

/**
  * 函数功能: NVIC配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void MX_NVIC_USARTx_Init(void)
{
	/* USART1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance==USART1)
	{
		if (UartIrq[Uart1] != NULL)
		{
			UartIrq[Uart1]();
		}
	}
}

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&husart[Uart1]);
}
