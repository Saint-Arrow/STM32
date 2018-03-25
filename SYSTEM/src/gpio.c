#include "gpio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
void GPIO_Conf(void)
{
	
	#if Hardware_Version == 3
	//�ƶ�
	GPIO_InitTypeDef  GPIO_InitStructure;	 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	   //ʹ��PB�˿�ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	   //AFIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 		     //LED0-->PB.2 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		    //��ת�ٶȲ���ʡ��
	GPIO_Init(GPIOB, &GPIO_InitStructure);			
		#ifdef motor_define
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;		    //PM1,PM2 �˿�����
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			GPIO_ResetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);	
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				        //GATE  -->PB3�����
			GPIO_Init(GPIOB, &GPIO_InitStructure);		
			GPIO_ResetBits(GPIOB,GPIO_Pin_3);			
		#endif	
	#elif Hardware_Version == 4
	GPIO_InitTypeDef  GPIO_InitStructure;	 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	   //ʹ��PB�˿�ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	   //AFIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 		     //LED0-->PB.2 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		    //��ת�ٶȲ���ʡ��
	GPIO_Init(GPIOB, &GPIO_InitStructure);			
		#ifdef motor_define
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;		    //PM1,PM2 �˿�����
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				        //GATE  -->PB3�����
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		//��V3��ͬ
		GPIO_SetBits(GPIOB,GPIO_Pin_3);			
		#endif
	#elif Hardware_Version == 0
	GPIO_InitTypeDef  GPIO_InitStructure;	 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	#elif (Hardware_Version == 1)||(Hardware_Version == 2)   
    GPIO_InitTypeDef  GPIO_InitStructure;	 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;				 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_15);	
#endif
	
					
	
}
 
void LED_PB2_ON(void)
{
	GPIOB->BSRR = GPIO_Pin_2;
}
void LED_PB2_OFF(void)
{
	GPIOB->BRR = GPIO_Pin_2;
}
