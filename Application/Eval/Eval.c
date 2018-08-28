#include "main.h"

#include "Eval.h"
#include "String.h"

uint8_t NodeAddr[5];
uint8_t GroupAddr[5];

void Read_ChipID(void)
{
	uint32_t ChipUniqueID[3];

	ChipUniqueID[0] = *(__IO uint32_t *)(0X1FFFF7F0); // ¸ß×Ö½Ú
	ChipUniqueID[1] = *(__IO uint32_t *)(0X1FFFF7EC); // 
	ChipUniqueID[2] = *(__IO uint32_t *)(0X1FFFF7E8); // µÍ×Ö½Ú

	NodeAddr[0] = (uint8_t)((ChipUniqueID[0] >> 24) & 0x000000FF);
	NodeAddr[1] = (uint8_t)((ChipUniqueID[1] >> 24) & 0x000000FF);
	NodeAddr[2] = (uint8_t)((ChipUniqueID[2] >> 24) & 0x000000FF);
	NodeAddr[3] = (uint8_t)((ChipUniqueID[2] >> 16) & 0x000000FF);
	NodeAddr[4] = 0x1C;

	memset(GroupAddr, 0, 5);

	Flash_Read(Stm32Flash, 0x801F800, GroupAddr, 5, BYTE);
}

void SetNodeID(uint8_t *Buffer)
{
	switch (Buffer[0])
	{
		case 0x01:
			DEBUG_Log ("SetGroupID\r\n");
			Flash_Write(Stm32Flash, 0x801F800, &Buffer[1], 5, BYTE);
			break;
		case 0x02:
			DEBUG_Log ("SetNodeID\r\n");
			break;
	}
}

void Eval_Init(void)
{
	IWDG_Init();

	Read_ChipID();

	Spi_Init();
	Uart_Init();
	Gpio_Init();
//	Tim_Init();
	Rtc_Init();

}
