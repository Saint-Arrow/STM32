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

#define KEY1 PAin(5)
#define KEY2 PAin(6)
#define KEY3 PAin(7)
#define S1_PRESS 1
#define S2_PRESS 2
#define S3_PRESS 4

#define AN_XD_DL 1 //time to responce for the first press
#define AN_LA_DL 20 //time to responce for continue press
#define AN_CJ_DL 50 //time to responce for long press
#define NO_PRESS 0

#define KEY_Click             1
#define KEY_Click_Release     2
#define KEY_LongPress_enable
#ifdef  KEY_LongPress_enable
#define KEY_LongPress         3
#endif
#define KEY_LongPress_Ralease 4


struct key_data
{
	unsigned char key_status;
	unsigned char key_num;
	unsigned int  key_time;
};

void GPIO_Conf(void);//��ʼ��
void LED_PB2_ON(void);
void LED_PB2_OFF(void);
void LED_PB2_TOG(void);	
void beep_init(void);
void beep_10ms(void);
void KEY_init(void);
unsigned char check_key_press(void);
#endif
