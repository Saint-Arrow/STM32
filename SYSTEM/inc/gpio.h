#ifndef __GPIO_H
#define __GPIO_H	 
#include ".\sys.h"
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
//#define LED PBout(6)
//#define WIFI_RESET PBout(12)	
//#define UART_BUF PAin(5)
//

void GPIO_Conf(void);//��ʼ��
void LED_PB2_ON(void);
void LED_PB2_OFF(void);
		 				    
#endif
