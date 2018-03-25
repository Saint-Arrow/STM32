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
	
	#if Hardware_Version == 3
	//云动
	GPIO_InitTypeDef  GPIO_InitStructure;	 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	   //使能PB端口时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	   //AFIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 		     //LED0-->PB.2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		    //翻转速度不能省略
	GPIO_Init(GPIOB, &GPIO_InitStructure);			
		#ifdef motor_define
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;		    //PM1,PM2 端口配置
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			GPIO_ResetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);	
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				        //GATE  -->PB3输出高
			GPIO_Init(GPIOB, &GPIO_InitStructure);		
			GPIO_ResetBits(GPIOB,GPIO_Pin_3);			
		#endif	
	#elif Hardware_Version == 4
	GPIO_InitTypeDef  GPIO_InitStructure;	 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	   //使能PB端口时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	   //AFIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 		     //LED0-->PB.2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		    //翻转速度不能省略
	GPIO_Init(GPIOB, &GPIO_InitStructure);			
		#ifdef motor_define
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;		    //PM1,PM2 端口配置
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				        //GATE  -->PB3输出高
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		//与V3不同
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
