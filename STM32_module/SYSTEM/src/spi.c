#include "..\inc\spi.h"
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
 
//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25Q64/NRF24L01						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
//PA4-NSS
//PA5-SCK
//PA6-MISO
//PA7-MOSI


void SPI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

	#if SPI_Master_Slave

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
	SPI_NSSInternalSoftwareConfig(SPI1,SPI_NSSInternalSoft_Set);

	#else
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI_NSSInternalSoftwareConfig(SPI1,SPI_NSSInternalSoft_Reset);

	#endif

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_MCU_MODE;		//����SPI����ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����

	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	
	#ifdef SPI_INTERRUPT

	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   	
	
	SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE); 
	#endif

	SPI1_ReadWriteByte(0X00);

	 
 

}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
  
void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); 

} 



#ifndef SPI_INTERRUPT

	//SPIx ��дһ���ֽ�
	//TxData:Ҫд����ֽ�
	//����ֵ:��ȡ�����ֽ�
	uint8_t SPI1_ReadWriteByte(uint8_t TxData)
	{		
		uint8_t retry=0;				 	
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
			{
			retry++;
			if(retry>200)return 0;
			}			  
		SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
		retry=0;
	
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
			{
			retry++;
			if(retry>200)return 0;
			}	  						    
		return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
		    
				    
	}
	
	
	uint8_t SPI1_ReadByte(void)
	{	uint8_t retry=0;		
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		 	{
			retry++;
			if(retry>200)return 0;
			} 						    
		return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����
							    
	}

	void SPI1_WriteByte(uint8_t TxData)
	{					 	
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
					  
		SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������					    
	}
#else
//	void SPI1_IRQnHandler(void)
//	{
//		if(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
//		{
//			if(SPI_I2S_ReceiveData(SPI1)==0XA5)
//			{
//			
//			}	
//		}
//
//	}
#endif































