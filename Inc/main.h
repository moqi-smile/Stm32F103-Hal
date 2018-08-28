/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#ifndef __GNUC__
#define inline
#endif

#define __DEBUG__       1

#if (__DEBUG__ == 0)
#define DEBUG_Log(format, ...)
#elif  (__DEBUG__ == 1)
#define DEBUG_Log(format, ...) printf (""format"", ##__VA_ARGS__)
#endif

void Error_Handler(char * file, int line);

#endif /* __MAIN_H */
