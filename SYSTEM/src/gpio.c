#include "gpio.h"
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
void GPIO_Conf(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	   //使能PB端口时钟	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 		     //LED0-->PB.2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		    //翻转速度不能省略
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
void beep_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	 
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	   //使能PB端口时钟	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				 		     //LED0-->PB.2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		    //翻转速度不能省略
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIOA->BSRR = GPIO_Pin_14;	
}
void beep_10ms(void)
{
	GPIOA->BRR = GPIO_Pin_14;	
	delay_ms(10);
	GPIOA->BSRR = GPIO_Pin_14;	
}
void KEY_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;				 		     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		    
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}
unsigned char check_key_press(void)
{
	if(KEY1) return S1_PRESS;
	if(KEY2) return S2_PRESS;
	if(KEY3) return S3_PRESS;
	
	return NO_PRESS;
}
