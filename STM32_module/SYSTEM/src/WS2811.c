
#include "WS2811.h"
int8_t WS2814_DIM_CONF(void)
{
	//使用RESET的PIN
	//与RESET 485冲突
	GPIO_InitTypeDef  GPIO_InitStructure;	 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	 			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);	
	return 0;
}
int8_t send_bit_0(void)
{
	GPIOB->BSRR = GPIO_Pin_13;
	__nop();__nop();__nop();
	Delay(1);
	GPIOB->BRR = GPIO_Pin_13;
	Delay(10);	
	return 0;
}
int8_t send_bit_1(void)
{
	GPIOB->BSRR = GPIO_Pin_13;
	Delay(13);	
	GPIOB->BRR = GPIO_Pin_13;
	__nop();__nop();__nop();
	return 0;
}
int8_t RESET_WS2811(void)
{
	GPIOB->BRR = GPIO_Pin_13;
	Delay(400);
	GPIOB->BSRR = GPIO_Pin_13;
	Delay(13);	
	GPIOB->BRR = GPIO_Pin_13;
	return 0;
}
int8_t WS2814_char(uint8_t value)
{
	uint8_t i=0;
	uint8_t check=0X80;
	while(i<8)
	{
		if(value & check ) 
			send_bit_1();
		else 
			send_bit_0();
		check >>= 1;
		i++;
	}
	return 0;
}
int8_t WS2814_rgb(unsigned long value)
{
	unsigned char i;
        
        for(i=0; i<24; i++)
        {
                if(0x800000 == (value & 0x800000) )        
					send_bit_1();
                else                                                               
					send_bit_0();
                value<<=1;                                                   
        }
	return 0;
}
