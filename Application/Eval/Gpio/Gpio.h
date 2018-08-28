#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
 extern "C" {
#endif

#define LED1  PC13
#define LED2  PA1

#define UART1_Tx  PA9
#define UART1_Rx  PA10

#define RADIO_Res  PB11
#define RADIO_Nss  PA4
#define RADIO_DIO0  PB0
#define RADIO_DIO1  PB1
#define RADIO_DIO2  PB10
#define RADIO_DIO3  PA8
#define RADIO_DIO4  PA11
#define RADIO_DIO5  PB3

typedef void (*GpioIrqCallback)(void);

typedef enum 
{
	PA0 = 0x00, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
	PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
	PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
	PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
	PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
	PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,
	PG0, PG1, PG2, PG3, PG4, PG5, PG6, PG7, PG8, PG9, PG10, PG11, PG12, PG13, PG14, PG15,
	PH0, PH1, PH2, PH3, PH4, PH5, PH6, PH7, PH8, PH9, PH10, PH11, PH12, PH13, PH14, PH15,
} Gpio_t;

typedef enum 
{
	Gpio_ModeInput = 0x00,
	Gpio_ModeOutputPP ,
	Gpio_ModeOutputOD ,
	Gpio_ModeAFPP ,
	Gpio_ModeAFOD,
	Gpio_ModeAFINPUT,

	Gpio_ModeANALOG,

	Gpio_ModeITRISING,
	Gpio_ModeITFALLING,
	Gpio_ModeITRISING_FALLING,

	Gpio_ModeEVTRISING,
	Gpio_ModeEVTFALLING,
	Gpio_ModeEVTRISING_FALLING,
} GpioMode_t;

typedef enum 
{
	Gpio_SpeedLow = 0x00,
	Gpio_SpeedMedium ,
	Gpio_SpeedHigh ,
} GpioSpeed_t;

typedef enum 
{
	Gpio_NOPULL = 0x00,
	Gpio_PULLUP,
	Gpio_PULLDOWN,
} GpioPull_t;

typedef enum 
{
  GpioInputRes = 0,
  GpioInputSet
} GpioInputStatus;


void Gpio_Init(void);

void GpioPinSet(Gpio_t Gpiox);
void GpioPinRes(Gpio_t Gpiox);
void GpioPinToggle(Gpio_t Gpiox);

GpioInputStatus Gpio_ReadInput(Gpio_t Gpiox);

void Gpio_SetIrq(Gpio_t Gpiox, GpioIrqCallback Irq, uint32_t Priority);
void Gpio_Config(Gpio_t Gpiox, GpioMode_t Mode, GpioSpeed_t Speed, GpioPull_t Pull);

#ifdef __cplusplus
}
#endif

#endif
/* __GPIO_H__ */
