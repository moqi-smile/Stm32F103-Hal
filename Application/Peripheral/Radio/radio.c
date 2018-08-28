//#include "main.h"

//#include "iwdg/iwdg.h"
//#include "gpio/gpio.h"
//#include "usart/usart.h"

//#include "Contiki.h"

//#include "radio.h"
//#include "sx1276.h"

//#include "string.h"

//#define BUFFER_SIZE     255
//// Define the payload size here

//static uint16_t BufferSize = BUFFER_SIZE;
//// RF buffer size
//static uint8_t  Buffer[BUFFER_SIZE];
//// RF buffer

//static uint16_t RadioData_Index = BUFFER_SIZE;
//// RF buffer size
//static uint8_t  RadioData_Buf[BUFFER_SIZE];

//uint32_t NetADDR = 0;

//void SX1276OnDio0Irq( void );
//void SX1276OnDio1Irq( void );
//void SX1276OnDio2Irq( void );
//void SX1276OnDio3Irq( void );
//void SX1276OnDio4Irq( void );
//void SX1276OnDio5Irq( void );

//extern uint8_t* user_DriverRadio(uint8_t *msg, uint8_t len);

//void RadioTaskFunction(void const * argument);

//uint32_t RadioDriverStatu = Radio_IDLE;

//void RadioDriverInit( void )
//{
////    Eval_GpioSetIrq(User_RADIODIO0, SX1276OnDio0Irq);
////    Eval_GpioSetIrq(User_RADIODIO1, SX1276OnDio1Irq);
////    Eval_GpioSetIrq(User_RADIODIO2, SX1276OnDio2Irq);
////    Eval_GpioSetIrq(User_RADIODIO3, SX1276OnDio3Irq);
////    Eval_GpioSetIrq(User_RADIODIO4, SX1276OnDio4Irq);
////    Eval_GpioSetIrq(User_RADIODIO5, SX1276OnDio5Irq);

//    SX1276StartRx();
//}

//void RadioTaskFunction(void  const * argument)
//{
//	
//}

//void RadioDriverTxMsg( uint8_t *msg, uint8_t len)
//{
//	uint8_t buf[32];

//	memset(buf, 0, 32);

//	buf[0] = len;
//	memcpy(&buf[1], msg, len);

//	SX1276SetTxPacket( buf, len+1 );
//    SX1276StartTx();
//}

//void RadioCADStartFunc(void *in)
//{
//    SX1276StartCAD();
//}

//void SX1276OnDio0Irq(void)
//{
//}

//void SX1276OnDio1Irq(void)
//{
//    
//}

//void SX1276OnDio2Irq(void)
//{
//    SX1276TxRxFHSSChangedChannel();
//}

//void SX1276OnDio3Irq(void)
//{
//    switch (SX1276CADRUNNING())
//    {
//        case RF_CHANNEL_ACTIVITY_DETECTED:
//            break;
//        case RF_CHANNEL_EMPTY:
//            break;
//        default:
//            break;
//    }
//}

//void SX1276OnDio4Irq(void)
//{
//    
//}

//void SX1276OnDio5Irq(void)
//{
//    
//}
