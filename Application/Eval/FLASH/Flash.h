#ifndef __STMFLASH_H__
#define __STMFLASH_H__

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 128 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值
 
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
