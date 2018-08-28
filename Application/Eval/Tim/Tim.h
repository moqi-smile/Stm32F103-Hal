#ifndef __GENERAL_TIM_H__
#define __GENERAL_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/

#define GENERAL_TIMx                     TIM3
#define GENERAL_TIM_RCC_CLK_ENABLE()     __HAL_RCC_TIM3_CLK_ENABLE()
#define GENERAL_TIM_RCC_CLK_DISABLE()    __HAL_RCC_TIM3_CLK_DISABLE()
#define GENERAL_TIM_IRQ                  TIM3_IRQn
#define GENERAL_TIM_INT_FUN              TIM3_IRQHandler

#define GENERAL_TIM_PRESCALER           83  // ʵ��ʱ��Ƶ��Ϊ��1MHz
#define GENERAL_TIM_PERIOD              (1000-1)  // ��ʱ�������ж�Ƶ��Ϊ��1MHz/1000=1KHz����1ms��ʱ����

/* ��չ���� ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx;

/* �������� ------------------------------------------------------------------*/

void Tim_Init(void);

#endif	/* __GENERAL_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
