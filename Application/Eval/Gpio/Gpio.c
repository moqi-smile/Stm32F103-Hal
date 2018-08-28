#include "stm32f1xx_hal.h"
#include "main.h"

#include "Gpio/Gpio.h"

GpioIrqCallback Gpio_IrqCallback[16] = 
{
	NULL, NULL, NULL, NULL, 
	NULL, NULL, NULL, NULL, 
	NULL, NULL, NULL, NULL, 
	NULL, NULL, NULL, NULL, 
};

const IRQn_Type Gpio_Exti[16] = 
{
	EXTI0_IRQn
};

const uint32_t GPIO_Mode[13] =
{
	GPIO_MODE_INPUT, GPIO_MODE_OUTPUT_PP, GPIO_MODE_OUTPUT_OD, GPIO_MODE_AF_PP, GPIO_MODE_AF_OD, GPIO_MODE_AF_INPUT,
	GPIO_MODE_ANALOG,
	GPIO_MODE_IT_RISING, GPIO_MODE_IT_FALLING, GPIO_MODE_IT_RISING_FALLING,
	GPIO_MODE_EVT_RISING, GPIO_MODE_EVT_FALLING, GPIO_MODE_EVT_RISING_FALLING
};

const uint32_t GPIO_Speed[3] =
{
	GPIO_SPEED_FREQ_LOW, GPIO_SPEED_FREQ_MEDIUM, GPIO_SPEED_FREQ_HIGH
};

const uint32_t GPIO_Pull[3] =
{
	GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
};

void Gpio_Config(Gpio_t Gpiox, GpioMode_t Mode, GpioSpeed_t Speed, GpioPull_t Pull)
{
	uint16_t Pinx = 1;
	GPIO_TypeDef *PORT;
	GPIO_InitTypeDef GPIO_InitStruct;

	PORT = ((GPIO_TypeDef *) (GPIOA_BASE + ((Gpiox&0xF0)>>4)*0x0400U ));
	Pinx <<=  Gpiox&0x0F;

	GPIO_InitStruct.Pin = Pinx;
	GPIO_InitStruct.Mode = GPIO_Mode[Mode];
	GPIO_InitStruct.Speed = GPIO_Speed[Speed];
	GPIO_InitStruct.Pull = GPIO_Pull[Pull];
	HAL_GPIO_Init(PORT, &GPIO_InitStruct);
}

void GpioPinSet(Gpio_t Gpiox)
{
	uint16_t Pinx = 1;
	GPIO_TypeDef *PORT;

	PORT = ((GPIO_TypeDef *) (GPIOA_BASE + ((Gpiox&0xF0)>>4)*0x0400U ));
	Pinx <<=  Gpiox&0x0F;

	HAL_GPIO_WritePin(PORT, Pinx, GPIO_PIN_SET);
}

void GpioPinRes(Gpio_t Gpiox)
{
	uint16_t Pinx = 1;
	GPIO_TypeDef *PORT;

	PORT = ((GPIO_TypeDef *) (GPIOA_BASE + ((Gpiox&0xF0)>>4)*0x0400U ));
	Pinx <<=  Gpiox&0x0F;

	HAL_GPIO_WritePin(PORT, Pinx, GPIO_PIN_RESET);
}

void GpioPinToggle(Gpio_t Gpiox)
{
	uint16_t Pinx = 1;
	GPIO_TypeDef *PORT;

	PORT = ((GPIO_TypeDef *) (GPIOA_BASE + ((Gpiox&0xF0)>>4)*0x0400U ));
	Pinx <<=  Gpiox&0x0F;

	HAL_GPIO_TogglePin(PORT, Pinx);
}

GpioInputStatus Gpio_ReadInput(Gpio_t Gpiox)
{
	uint16_t Pinx = 1;
	GPIO_TypeDef *PORT;

	PORT = ((GPIO_TypeDef *) (GPIOA_BASE + ((Gpiox&0xF0)>>4)*0x0400U ));
	Pinx <<=  Gpiox&0x0F;

	if (HAL_GPIO_ReadPin(PORT, Pinx) == GPIO_PIN_RESET)
	{
		return GpioInputRes;
	}
	else
	{
		return GpioInputSet;
	}
}

void Gpio_SetIrq(Gpio_t Gpiox, GpioIrqCallback Irq, uint32_t Priority)
{
	Gpio_IrqCallback[Gpiox&0x0F] = Irq;
	HAL_NVIC_SetPriority(Gpio_Exti[Gpiox&0x0F], 8, 0);
	HAL_NVIC_EnableIRQ(Gpio_Exti[Gpiox&0x0F]);
}

void Gpio_Init(void)
{
	Gpio_Config(LED1, Gpio_ModeOutputPP, Gpio_SpeedHigh, Gpio_NOPULL);
	Gpio_Config(PA1, Gpio_ModeInput, Gpio_SpeedLow, Gpio_PULLUP);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint16_t Pin;
	uint8_t i;

	for (i=0,Pin=0x0001; GPIO_Pin == Pin<<i; i++)
	{
		if (Gpio_IrqCallback[i] != NULL)
			Gpio_IrqCallback[i]();
	}
}

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI4_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}
