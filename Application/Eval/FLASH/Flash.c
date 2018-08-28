#include "main.h"

#include "Flash/Flash.h"

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024
//�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif

uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];
//�����2K�ֽ�

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
		//��ȡ2���ֽ�.
		ReadAddr+=2;
		//ƫ��2���ֽ�.	
	}
}

void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)   
{ 			 		 
	uint16_t i;

	for(i=0;i<NumToWrite;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, WriteAddr, pBuffer[i]);
	    WriteAddr+=2;
		//��ַ����2.
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
	// ������ַ
	uint16_t secoff;
	// ������ƫ�Ƶ�ַ(16λ�ּ���)
	uint16_t secremain;
	// ������ʣ���ַ(16λ�ּ���)	   
 	uint16_t i;
	uint32_t offaddr;   
	// ȥ��0X08000000��ĵ�ַ

	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))
		return;
	//�Ƿ���ַ
	HAL_FLASH_Unlock();
	//����
	offaddr=WriteAddr-STM32_FLASH_BASE;
	//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;
	//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;
	//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;
	//����ʣ��ռ��С
	if(NumToWrite<=secremain)secremain=NumToWrite;
	//�����ڸ�������Χ

	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);
		//������������������
		for(i=0;i<secremain;i++)
		//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)
				break;
			//��Ҫ����  	  
		}
		if(i<secremain)
			//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);
			//�����������
			for(i=0;i<secremain;i++)
			//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);
			//д����������  
		}
		else
			STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);
		//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)
			break;
		//д�������
		else//д��δ����
		{
			secpos++;
			//������ַ��1
			secoff=0;
			//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;
			//ָ��ƫ��
			WriteAddr+=secremain;
			//д��ַƫ��	   
		   	NumToWrite-=secremain;
			//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))
				secremain=STM_SECTOR_SIZE/2;
			//��һ����������д����
			else
				secremain=NumToWrite;
				//��һ����������д����
		}	 
	};	
	HAL_FLASH_Lock();
	//����
}

/**
  * @brief  Flash д�뺯��
  * @param  Flashx : ѡ��洢�� (��ʱֻ�� STM32 ���ڲ� Flash)
  *         ReadAddr : д���ַ (����Ϊż��, ���������)
  *         pBuffer : �����ַ
  *         ReadAddr : д�볤��
  *         ReadAddr : д������ʹ�С(�ֽ�, ����, ��, ˫��)
  * @retval 0: д��ɹ�
  *         1: ��������
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
  * @brief  Flash ��ȡ����
  * @param  Flashx : ѡ��洢�� (��ʱֻ�� STM32 ���ڲ� Flash)
  *         ReadAddr : ��ȡ��ַ (����Ϊż��, ���������)
  *         pBuffer : �����ַ
  *         ReadAddr : ��ȡ����
  *         ReadAddr : ��ȡ�����ʹ�С(�ֽ�, ����, ��, ˫��)
  * @retval 0: ��ȡ�ɹ�
  *         1: ��������
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
