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
	GPIO_InitTypeDef  GPIO_InitStructure;	 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	   //ʹ��PB�˿�ʱ��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 		     //LED0-->PB.2 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		    //��ת�ٶȲ���ʡ��
	GPIO_Init(GPIOB, &GPIO_InitStructure);			
					
}
 
void LED_PB2_ON(void)
{
	GPIOB->BSRR = GPIO_Pin_2;
}
void LED_PB2_OFF(void)
{
	GPIOB->BRR = GPIO_Pin_2;
}
void LED_PB2_TOG(void)
{
	GPIOB->ODR ^= GPIO_Pin_2;
}
