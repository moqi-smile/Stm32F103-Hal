#include "stm32f1xx_hal.h"
#include "main.h"

#include "Spi/Spi.h"

#include "Gpio/Gpio.h"

SPI_HandleTypeDef hspi[Spinum];

/* SPI1 init function */
void Spi_Init(void)
{
	hspi[Spi1].Instance = SPI1;
	hspi[Spi1].Init.Mode = SPI_MODE_MASTER;
	hspi[Spi1].Init.Direction = SPI_DIRECTION_2LINES;
	hspi[Spi1].Init.DataSize = SPI_DATASIZE_8BIT;
	hspi[Spi1].Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi[Spi1].Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi[Spi1].Init.NSS = SPI_NSS_SOFT;
	hspi[Spi1].Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi[Spi1].Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi[Spi1].Init.TIMode = SPI_TIMODE_DISABLE;
	hspi[Spi1].Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi[Spi1].Init.CRCPolynomial = 10;

	if (HAL_SPI_Init(&hspi[Spi1]) != HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}
}

void Spi_Transmit(Spi_t Spix, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	HAL_SPI_Transmit( &hspi[Spix], pData, Size, Timeout );
}

void Spi_Receive(Spi_t Spix, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	HAL_SPI_Receive( &hspi[Spix], pData, Size, Timeout );
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	if(spiHandle->Instance==SPI1)
	{
		__HAL_RCC_SPI1_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{
	if(spiHandle->Instance==SPI1)
	{
		__HAL_RCC_SPI1_CLK_DISABLE();

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	}
} 
