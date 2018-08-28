#include "main.h"

#include "Flash/Flash.h"

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024
//字节
#else 
#define STM_SECTOR_SIZE	2048
#endif

uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];
//最多是2K字节

uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
	return *(__IO uint16_t*)faddr; 
}

void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);
		//读取2个字节.
		ReadAddr+=2;
		//偏移2个字节.	
	}
}

void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)   
{ 			 		 
	uint16_t i;

	for(i=0;i<NumToWrite;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, WriteAddr, pBuffer[i]);
	    WriteAddr+=2;
		//地址增加2.
	}  
}

void FLASH_ErasePage(uint32_t Page_Address)
{
	uint32_t PageError = 0;
	FLASH_EraseInitTypeDef f;
	f.TypeErase = FLASH_TYPEERASE_PAGES;
	f.PageAddress = Page_Address;
	f.NbPages = 1;
	//??PageError
	//??????
	HAL_FLASHEx_Erase(&f, &PageError);
}

void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t secpos;
	// 扇区地址
	uint16_t secoff;
	// 扇区内偏移地址(16位字计算)
	uint16_t secremain;
	// 扇区内剩余地址(16位字计算)	   
 	uint16_t i;
	uint32_t offaddr;   
	// 去掉0X08000000后的地址

	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))
		return;
	//非法地址
	HAL_FLASH_Unlock();
	//解锁
	offaddr=WriteAddr-STM32_FLASH_BASE;
	//实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;
	//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;
	//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;
	//扇区剩余空间大小
	if(NumToWrite<=secremain)secremain=NumToWrite;
	//不大于该扇区范围

	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);
		//读出整个扇区的内容
		for(i=0;i<secremain;i++)
		//校验数据
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)
				break;
			//需要擦除  	  
		}
		if(i<secremain)
			//需要擦除
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);
			//擦除这个扇区
			for(i=0;i<secremain;i++)
			//复制
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);
			//写入整个扇区  
		}
		else
			STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);
		//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumToWrite==secremain)
			break;
		//写入结束了
		else//写入未结束
		{
			secpos++;
			//扇区地址增1
			secoff=0;
			//偏移位置为0 	 
		   	pBuffer+=secremain;
			//指针偏移
			WriteAddr+=secremain;
			//写地址偏移	   
		   	NumToWrite-=secremain;
			//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))
				secremain=STM_SECTOR_SIZE/2;
			//下一个扇区还是写不完
			else
				secremain=NumToWrite;
				//下一个扇区可以写完了
		}	 
	};	
	HAL_FLASH_Lock();
	//上锁
}

/**
  * @brief  Flash 写入函数
  * @param  Flashx : 选择存储器 (暂时只有 STM32 的内部 Flash)
  *         ReadAddr : 写入地址 (必须为偶数, 奇数会出错)
  *         pBuffer : 缓存地址
  *         ReadAddr : 写入长度
  *         ReadAddr : 写入的类型大小(字节, 半字, 字, 双字)
  * @retval 0: 写入成功
  *         1: 参数错误
  */
uint8_t Flash_Write(Flash_t Flashx, uint32_t WriteAddr, void *pBuffer, uint16_t Size, FlashType_t Type)
{
	uint16_t WriteLen = 0;

	if ((WriteAddr %2 != 0)||(pBuffer == NULL))
		return 1;

	switch (Type)
	{
		case BYTE:
			WriteLen = Size*1;
			break;
		case HALFWORD:
			WriteLen = Size*2;
			break;
		case WORD:
			WriteLen = Size*4;
			break;
		case DOUBLEWORD:
			WriteLen = Size*8;
			break;
	}

	STMFLASH_Write(0x801F800, pBuffer, WriteLen);
	return 0;
}

/**
  * @brief  Flash 读取函数
  * @param  Flashx : 选择存储器 (暂时只有 STM32 的内部 Flash)
  *         ReadAddr : 读取地址 (必须为偶数, 奇数会出错)
  *         pBuffer : 缓存地址
  *         ReadAddr : 读取长度
  *         ReadAddr : 读取的类型大小(字节, 半字, 字, 双字)
  * @retval 0: 读取成功
  *         1: 参数错误
  */
uint8_t Flash_Read(Flash_t Flashx, uint32_t ReadAddr, void *pBuffer, uint16_t Size, FlashType_t Type)
{
	uint16_t ReadLen = 0;

	if ((ReadAddr %2 != 0)||(pBuffer == NULL))
		return 1;
	switch (Type)
	{
		case BYTE:
			if (Size %2 != 0)
				Size+=1;
			ReadLen = Size/2;
			break;
		case HALFWORD:
			ReadLen = Size;
			break;
		case WORD:
			ReadLen = Size*2;
			break;
		case DOUBLEWORD:
			ReadLen = Size*4;
			break;
	}

	STMFLASH_Read(0x801F800, pBuffer, ReadLen);
	return 0;
}
