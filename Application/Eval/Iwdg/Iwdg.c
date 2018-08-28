#include "stm32f1xx_hal.h"
#include "main.h"

#include "Iwdg/Iwdg.h"

IWDG_HandleTypeDef hiwdg;

void IWDG_Init(void)
{
	hiwdg.Instance = IWDG;
	hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
	hiwdg.Init.Reload = 1000;

	if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}
}

void IWDG_Refresh(void)
{
	HAL_IWDG_Refresh(&hiwdg);
}
