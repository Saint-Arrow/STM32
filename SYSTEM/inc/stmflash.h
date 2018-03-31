/**
  ******************************************************************************
  * @file    EEPROM_Emulation/inc/eeprom.h 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    07/27/2009
  * @brief   This file contains all the functions prototypes for the EEPROM 
  *          emulation firmware library.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPROM_H
#define __EEPROM_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported constants --------------------------------------------------------*/
/* Define the STM32F10Xxx Flash page size depending on the used STM32 device */
#if defined (STM32F10X_LD) || defined (STM32F10X_MD)
  #define PAGE_SIZE  (uint16_t)0x400  /* Page size = 1KByte */
#elif defined (STM32F10X_HD) || defined (STM32F10X_CL)
  #define PAGE_SIZE  (uint16_t)0x800  /* Page size = 2KByte */
#endif

/* EEPROM start address in Flash */
#define FLASH_MAX_ADDRESS		((uint32_t)0x08010000) 
#define srtApplicationAddress                   0x8008C00
#define UPDATEFLAG                              0xA5A5
#define Version_Address                         0x10


/* Pages --最后一页存储软件升级消息 */
#define PAGE(n)				               (n)
/* No valid page define */
#define NO_VALID_PAGE           ((uint16_t)0x00AB)

/* Page status definitions */
#define ERASED                  ((uint16_t)0xFFFF)     /* PAGE is empty */
#define RECEIVE_DATA            ((uint16_t)0xEEEE)     /* PAGE is marked to receive data */
#define VALID_PAGE              ((uint16_t)0x0000)     /* PAGE containing valid data */

/* Valid pages in read and write defines */
#define READ_FROM_VALID_PAGE    ((uint8_t)0x00)
#define WRITE_IN_VALID_PAGE     ((uint8_t)0x01)

/* Page full define */
#define PAGE_FULL               ((uint8_t)0x80)
#define PAGE_UnErased           ((uint8_t)0xC0)

/* Variables' number */
#define NumbOfVar               ((uint8_t)0x03)

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


uint8_t Flash_ByteRead(uint32_t fadder);
uint16_t Flash_HalfWordRead(uint32_t fadder);

int16_t eeprom_SerialRead(unsigned char page,uint16_t fadder,uint8_t *pBuffer,uint16_t numToRead);
int16_t eeprom_SerialWrite(uint8_t page,uint16_t fadder,uint8_t *pBuffer,uint16_t numToWrite);
void eeprom_Erase(uint8_t page);
#endif /* __EEPROM_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
