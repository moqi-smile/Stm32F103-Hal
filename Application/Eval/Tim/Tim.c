#include "main.h"

#include "Tim/Tim.h"
#include "Gpio/Gpio.h"

TIM_HandleTypeDef htimx;

void Tim_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htimx.Instance = TIM3;
	htimx.Init.Prescaler = 71;
	htimx.Init.Period = 4999;
	htimx.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimx.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htimx.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htimx);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	// ����ʱ��Դ
	HAL_TIM_ConfigClockSource(&htimx, &sClockSourceConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	// �ر�����ģʽ
	HAL_TIMEx_MasterConfigSynchronization(&htimx, &sMasterConfig);
	HAL_TIM_Base_Start_IT(&htimx);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();

		HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
	}
}

/**
  * ��������: ������ʱ��Ӳ������ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==TIM3)
	{
		GENERAL_TIM_RCC_CLK_DISABLE();
		HAL_NVIC_DisableIRQ(TIM3_IRQn);
	}
} 
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimx);
}
