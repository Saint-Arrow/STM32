#ifndef __GPIO_H
#define __GPIO_H	 
#include ".\sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
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

void GPIO_Conf(void);//初始化
void LED_PB2_ON(void);
void LED_PB2_OFF(void);
void LED_PB2_TOG(void);	
void beep_init(void);
void beep_10ms(void);
void KEY_init(void);
unsigned char check_key_press(void);
#endif
