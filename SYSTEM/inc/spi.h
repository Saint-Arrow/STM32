#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SPI���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


#define SPI_Master_Slave 0	   //1 Master ;0 Slave

#if	 SPI_Master_Slave
	#define SPI_MCU_MODE SPI_Mode_Master
#else
	#define SPI_MCU_MODE SPI_Mode_Slave 
#endif
//#define SPI_INTERRUPT 
 				  	    													  
void SPI1_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(uint8_t SpeedSet); //����SPI�ٶ�  

#ifndef  SPI_INTERRUPT
uint8_t SPI1_ReadWriteByte(uint8_t TxData);//SPI���߶�дһ���ֽ�
uint8_t SPI1_ReadByte(void);
void SPI1_WriteByte(uint8_t TxData);	
#endif
	 
#endif

