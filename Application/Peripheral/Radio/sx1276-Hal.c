/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       sx1276-Hal.c
 * \brief      SX1276 Hardware Abstraction Layer
 *
 * \version    2.0.B2 
 * \date       Nov 21 2012
 * \author     Miguel Luis
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */

#include "platform.h"

#include "sx1276-Hal.h"

extern SPI_HandleTypeDef hspi1;

void Set_RF_Switch_RX(void)
{
//	RXE_HIGH();
//	TXE_LOW();
}

void Set_RF_Switch_TX(void)
{
//	RXE_LOW();
//	TXE_HIGH();
}


void SX1276InitIo( void )
{
	Gpio_Config(RADIO_Nss, Gpio_ModeOutputPP, Gpio_SpeedLow, Gpio_NOPULL);
	Gpio_Config(RADIO_Res, Gpio_ModeOutputPP, Gpio_SpeedLow, Gpio_NOPULL);

	Gpio_Config(RADIO_DIO0, Gpio_ModeITRISING, Gpio_SpeedLow, Gpio_NOPULL);
	Gpio_Config(RADIO_DIO1, Gpio_ModeITRISING, Gpio_SpeedLow, Gpio_NOPULL);
	Gpio_Config(RADIO_DIO2, Gpio_ModeITRISING, Gpio_SpeedLow, Gpio_NOPULL);
	Gpio_Config(RADIO_DIO3, Gpio_ModeITRISING, Gpio_SpeedLow, Gpio_NOPULL);
	Gpio_Config(RADIO_DIO4, Gpio_ModeITRISING, Gpio_SpeedLow, Gpio_NOPULL);
	Gpio_Config(RADIO_DIO5, Gpio_ModeITRISING, Gpio_SpeedLow, Gpio_NOPULL);
}

void SX1276SetReset( uint8_t state )
{
	if( state == RADIO_RESET_ON )
	{
		GpioPinRes(RADIO_Res);
	}
	else
	{
		GpioPinSet(RADIO_Res);
	}
}

void SX1276Write( uint8_t addr, uint8_t data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}

void SX1276Read( uint8_t addr, uint8_t *data )
{
   SX1276ReadBuffer( addr, data, 1 );
}

void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
	addr = addr|0x80;

	GpioPinRes(RADIO_Nss);
	Spi_Transmit( Spi1, &addr, 1, 100 );
	Spi_Transmit( Spi1, buffer, size, 100 );
	GpioPinSet(RADIO_Nss);
}

void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
	addr = addr&0x7F;

	GpioPinRes(RADIO_Nss);
	Spi_Transmit( Spi1, &addr, 1, 100 );
	Spi_Receive ( Spi1, buffer, size, 100);
	GpioPinSet(RADIO_Nss);
}

void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
	SX1276WriteBuffer( 0, buffer, size );
}

void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
	SX1276ReadBuffer( 0, buffer, size );
}

//射频芯片收发切换
inline void SX1276WriteRxTx( uint8_t txEnable )
{
    if( txEnable != 0 )
    {
//		Set_RF_Switch_TX(); //单片机将射频开关芯片切换成发射状态
//        IoePinOn( FEM_CTX_PIN );
//        IoePinOff( FEM_CPS_PIN );
    }
    else
    {
//		Set_RF_Switch_RX();  //单片机将射频开关芯片切换成接收状态
//        IoePinOff( FEM_CTX_PIN );
//        IoePinOn( FEM_CPS_PIN );
    }
}
