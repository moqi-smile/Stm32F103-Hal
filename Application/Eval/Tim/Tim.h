#ifndef __GENERAL_TIM_H__
#define __GENERAL_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/

#define GENERAL_TIMx                     TIM3
#define GENERAL_TIM_RCC_CLK_ENABLE()     __HAL_RCC_TIM3_CLK_ENABLE()
#define GENERAL_TIM_RCC_CLK_DISABLE()    __HAL_RCC_TIM3_CLK_DISABLE()
#define GENERAL_TIM_IRQ                  TIM3_IRQn
#define GENERAL_TIM_INT_FUN              TIM3_IRQHandler

#define GENERAL_TIM_PRESCALER           83  // 实际时钟频率为：1MHz
#define GENERAL_TIM_PERIOD              (1000-1)  // 定时器产生中断频率为：1MHz/1000=1KHz，即1ms定时周期

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx;

/* 函数声明 ------------------------------------------------------------------*/

void Tim_Init(void);

#endif	/* __GENERAL_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
