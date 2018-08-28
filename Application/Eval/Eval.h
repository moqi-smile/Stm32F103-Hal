#ifndef __EVAL_H__
#define __EVAL_H__

#include "iwdg/iwdg.h"
#include "Spi/Spi.h"
#include "gpio/gpio.h"
#include "usart/usart.h"
#include "Tim/Tim.h"
#include "Rtc/Rtc.h"
#include "Flash/Flash.h"

extern uint8_t NodeAddr[5];
extern uint8_t GroupAddr[5];

void Eval_Init(void);
void SetNodeID(uint8_t *Buffer);

#endif

/* __EVAL_H__ */
