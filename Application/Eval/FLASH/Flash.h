#ifndef __STMFLASH_H__
#define __STMFLASH_H__

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 128 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
//FLASH������ֵ
 
typedef enum 
{
	BYTE = 0,
	HALFWORD,
	WORD,
	DOUBLEWORD
} FlashType_t;

typedef enum 
{
	Stm32Flash = 0,
} Flash_t;

void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);
uint8_t Flash_Write(Flash_t Flashx, uint32_t WriteAddr, void *pBuffer, uint16_t Size, FlashType_t Type);
uint8_t Flash_Read(Flash_t Flashx, uint32_t ReadAddr, void *pBuffer, uint16_t Size, FlashType_t Type);						   

#endif
