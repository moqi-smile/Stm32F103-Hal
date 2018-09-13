#include "main.h"

#include "Eval.h"

#include "Contiki.h"

unsigned int idle_count = 0;

void RTCApplicationInit(void);
void UartApplicationInit(void);
void RadioApplicationInit(void);
void MotorApplicationInit(void);


PROCESS(led_process, "led");

/**
  * @brief  主函数
  * @param  void
  * @retval None
  */
void Application_Main(void)
{
	Eval_Init();

	process_init ();
	// 进程初始化

	ctimer_init();
	process_start(&etimer_process, NULL);
	// 启动系统进程

	process_start(&led_process, NULL);

	RTCApplicationInit();
	UartApplicationInit();
	RadioApplicationInit();
	MotorApplicationInit();

	DEBUG_Log  ("ChipID %.2X%.2X%.2X%.2X%.2X\r\n",
		NodeAddr[0], NodeAddr[1], NodeAddr[2], NodeAddr[3], NodeAddr[4]);
	DEBUG_Log  ("ChipID %.2X%.2X%.2X%.2X%.2X\r\n",
		GroupAddr[0], GroupAddr[1], GroupAddr[2], GroupAddr[3], GroupAddr[4]);

	while (1)
	{
		do 
		{
			IWDG_Refresh();
		}
		while(process_run() > 0);
		idle_count++;
	}
}


PROCESS_THREAD(led_process, ev, data)
{
	static struct etimer et;

	PROCESS_BEGIN();

	while(1)
	{
		etimer_set(&et, CLOCK_SECOND/2);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

//		if (Gpio_ReadInput(PA1) == GpioInputRes)
//		{
//			DEBUG_Log  ("GpioInputRes\r\n\r\n");
//		}else
//		{
//			DEBUG_Log  ("GpioInputSet\r\n\r\n");
//		}
	}

	PROCESS_END();
	// 宏用于辅助保存断点信息 (即行数 )
}



