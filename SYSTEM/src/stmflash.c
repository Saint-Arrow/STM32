/**
  ******************************************************************************
  * @file    EEPROM_Emulation/src/eeprom.c 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    07/27/2009
  * @brief   This file provides all the EEPROM emulation firmware functions.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 
/** @addtogroup EEPROM_Emulation
  * @{
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stmflash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




uint8_t Flash_ByteRead(uint32_t fadder)
{
	return *(uint8_t * )fadder;
}
uint16_t Flash_HalfWordRead(uint32_t fadder)
{
	return *(uint16_t * )fadder;
}


int16_t eeprom_SerialRead(uint8_t page,uint16_t fadder,uint8_t *pBuffer,uint16_t numToRead)
{
    int i =0;
    uint16_t DataVar;
    uint32_t tempaddr = FLASH_MAX_ADDRESS-PAGE_SIZE-page*PAGE_SIZE+fadder;
    uint8_t *p = pBuffer;
    uint16_t Status = 0;
	assert_param(tempaddr<FLASH_MAX_ADDRESS);
    FLASH_Unlock();
	if(numToRead%2==1)
	{
		while(i<((numToRead-1)/2))
	    {
			DataVar = (*(__IO uint16_t*)(tempaddr));
		    i++;	
			*(unsigned short*)p = DataVar;
			p+=2;
			tempaddr+=2;
	    }
		DataVar = (*(__IO uint16_t*)(tempaddr))>>8;
		*p = DataVar;
	}
	else
	{
		while(i<(numToRead/2))
	    {
			DataVar = (*(__IO uint16_t*)(tempaddr));
		    i++;	
			*(unsigned short*)p = DataVar;
			p+=2;
			tempaddr+=2;
	    }
	}
    
    FLASH_Lock();
    return Status;
}
/**
  * @brief  write the data @ the address.
  * @param  0X800 0000-
  * @retval no
  */
int16_t eeprom_SerialWrite(uint8_t page,uint16_t fadder,uint8_t *pBuffer,uint16_t numToWrite)
{
	uint16_t DataVar;
	int i=0;
	uint32_t tempaddr = FLASH_MAX_ADDRESS-PAGE_SIZE-page*PAGE_SIZE+fadder;
    uint16_t Status = 0;
	uint8_t *p =pBuffer;
	assert_param(tempaddr<FLASH_MAX_ADDRESS);
	FLASH_Unlock();
	if(numToWrite%2==1) 
	{
		while(i<(numToWrite-1))
		{
			DataVar = *(unsigned short*)p;
			/* Set variable virtual address */
			Status = FLASH_ProgramHalfWord(tempaddr , DataVar);
	        if (Status != FLASH_COMPLETE)
	        {
	            FLASH_Lock();
	 	        return Status;//³ö´í
	        }
			tempaddr += 2;
			p+= 2;
			i+=2;
		}
		DataVar = (*p)<<8;
		Status = FLASH_ProgramHalfWord(tempaddr , DataVar);
		if (Status != FLASH_COMPLETE)
        {
            FLASH_Lock();
 	        return Status;//³ö´í
        }

	}
	else
	{
		while(i<numToWrite)
		{
			DataVar = *(unsigned short*)p;
			/* Set variable virtual address */
			Status = FLASH_ProgramHalfWord(tempaddr , DataVar);
	        if (Status != FLASH_COMPLETE)
	        {
	            FLASH_Lock();
	 	        return Status;//³ö´í
	        }
			tempaddr += 2;
			p+= 2;
			i+=2;
		}
	}

	FLASH_Lock();
 	return Status;
}
/**
  * @brief  erase the page.
  * @param  0X800 0000-
  * @retval no
  */
void eeprom_Erase(unsigned char page)
{
	uint32_t address = FLASH_MAX_ADDRESS-PAGE_SIZE-page*PAGE_SIZE;
	assert_param(address<FLASH_MAX_ADDRESS);
	if(address >= FLASH_MAX_ADDRESS) return;
	FLASH_Unlock();
	FLASH_ErasePage(address);
	FLASH_Lock();
	
}
/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
