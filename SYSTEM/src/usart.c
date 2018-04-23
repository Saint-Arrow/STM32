#include "usart.h"	  
#include "stmflash.h"
//注意,读取USARTx->SR能避免莫名其妙的错误  

/*******usart3******/
uint16_t recv_num;
uint32_t usart_time;
uint16_t USART_TX_STA;
uint8_t USART_TX_BUF[USART_TX_LEN],TX_MCU[USART_TX_LEN];     				   						       
volatile uint16_t USART_RX_STA;    
uint8_t USART_RX_BUF[USART_REC_LEN],RX_MCU[USART_REC_LEN];
uint8_t USART_assembe_flag;

/*******usart1******/
uint32_t usart_1_time;
volatile uint16_t USART_1_RX_STA;    
uint8_t USART_1_RX_BUF[USART_REC_LEN],RX_1_MCU[USART_REC_LEN];

void uart1_init(uint32_t bound)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);		
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
USART_DeInit(USART1); 															

//USART1_TX   PA.9
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 									     
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									
GPIO_Init(GPIOA, &GPIO_InitStructure);									         
//USART1_RX	  PA.10
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							 
GPIO_Init(GPIOA, &GPIO_InitStructure);  										


USART_InitStructure.USART_BaudRate = bound;										 
USART_InitStructure.USART_WordLength = USART_WordLength_8b;						 
USART_InitStructure.USART_StopBits = USART_StopBits_1;							 
USART_InitStructure.USART_Parity = USART_Parity_No;								
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					
USART_Init(USART1, &USART_InitStructure);

NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;						
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 14;								
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									
NVIC_Init(&NVIC_InitStructure);													

USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
		
USART_Cmd(USART1, ENABLE);   		
}
void uart3_init(uint32_t bound)
{	
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);		 
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
USART_DeInit(USART3); 															 

//USART3_TX   PB.10
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 									     
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									 
GPIO_Init(GPIOB, &GPIO_InitStructure);									        
//USART3_RX	  PB.11
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							
GPIO_Init(GPIOB, &GPIO_InitStructure);  										 



USART_InitStructure.USART_BaudRate = bound;										
USART_InitStructure.USART_WordLength = USART_WordLength_8b;						
USART_InitStructure.USART_StopBits = USART_StopBits_1;							
USART_InitStructure.USART_Parity = USART_Parity_No;								 
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					
USART_Init(USART3, &USART_InitStructure);
				
	


NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;						
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 15;								
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									
NVIC_Init(&NVIC_InitStructure);													

USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);									


USART_Cmd(USART3, ENABLE);  
}

void add_to_send_list(uint8_t *p,uint8_t len)
{
	if((USART_TX_STA+len) > USART_TX_LEN)
	{
		return;
	}
	
	memcpy(&TX_MCU[USART_TX_STA],p,len);
	USART_TX_STA+=len;
	return;
}

uint8_t numtostr(uint32_t num,char *str)
{
	uint8_t i;
	uint8_t j;
	uint8_t str_temp[10];
	for(i=0;i<10;i++)
	{
		str_temp[i]='0'+num%10;
		num=num/10;
		if(!num) break;
	}
	i++;
	for(j=0;j<i;j++)
	{
		str[j]=str_temp[i-1-j];
	}
	str[j]=0;
	return i;
}

/**
  * @brief  Print a character on the HyperTerminal
  * @param  c: The character to be printed
  * @retval None
  */
void Serial_3_PutChar(uint8_t c)
{
	uint16_t count = 0;
	USART_SendData(USART3, c);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
	{
		count ++;
		if(count >= 10000) return;
	}
}

/**
  * @brief  Print a string on the HyperTerminal
  * @param  s: The string to be printed
  * @retval None
  */
void Serial_3_PutString(char *s)
{	
	while (*s != '\0')
	{
		Serial_3_PutChar(*s);
		s ++;
	}
}
void Serial_3_PutData(uint8_t *s,uint16_t len)
{	
	uint16_t i=0;
	for(i=len;i>0;i--)
	{
		Serial_3_PutChar(*s);
		s ++;
	}

}
void Serial_1_PutChar(uint8_t c)
{
uint16_t count = 0;
USART_SendData(USART1, c);
while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
{
count ++;
if(count >= 10000) return;
}
}
void Serial_1_PutString(char *s)
{	
	while (*s != '\0')
	{
		Serial_1_PutChar(*s);
		s ++;
	}
}
void Serial_1_PutData(uint8_t *s,uint16_t len)
{	
	uint16_t i=0;
	for(i=len;i>0;i--)
	{
		Serial_1_PutChar(*s);
		s ++;
	}

}

/**
  * @brief  Test to see if a key has been pressed on the HyperTerminal
  * @param  key: The key pressed
  * @retval 1: Correct
  *         0: Error
  */
uint8_t SerialKeyPressed(uint8_t *key)
{
	#ifdef U3
		if ( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET)
		{
			*key = (uint8_t)USART3->DR;
			return 1;
		}
		else
		{
			return 0;
		}
	#endif
	
	
	#ifdef U1
		if ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
		{
			*key = (uint8_t)USART1->DR;
			return 1;
		}
		else
		{
			return 0;
		}
	#endif
  return 0;
  
}

/**
  * @brief  Receive byte from sender
  * @param  c: Character
  * @param  timeout: Timeout
  * @retval 0: Byte received
  *         -1: Timeout
  */
int32_t Receive_Byte (uint8_t *c, uint32_t timeout)
{
  while (timeout-- > 0)
  {
    if (SerialKeyPressed(c) == 1)
    {
      return 0;
    }
  }
  return -1;
}
/**
  * @brief  correct CRC
  * @param  *bytes *crc_master
  * @param  len
  * @retval 0: normally return
  *        -1: crc error
  */
 uint16_t crc16(uint16_t data_old,uint16_t data_new)
{
	uint32_t crc = data_old;   
	uint8_t i, j;  
	uint8_t *bytes =(uint8_t *)&data_new;
	for(i = 0; i < 2; i++)    
	{ 
		crc = crc ^ (unsigned int)(* bytes) << 8;   
		 
		for(j = 0; j < 8; j++)      
		{ 
			if(crc & 0x8000)        
				crc = crc << 1 ^ POLY16;       
			else 
	    		crc = crc << 1;     
		}  
		bytes++;
	} 
	return crc&0XFFFF;
} 
int8_t YmodemCrc(uint8_t *bytes, uint8_t len,uint8_t *crc_master) 
{ //缺点长度小于256
	uint32_t crc = 0x0000;   
	uint8_t i, j;  
	for(i = 0; i < len; i++)    
	{ 
		crc = crc ^ (unsigned int)(* bytes) << 8;   
		 
		for(j = 0; j < 8; j++)      
		{ 
			if(crc & 0x8000)        
				crc = crc << 1 ^ POLY16;       
			else 
	    		crc = crc << 1;     
		}  
		bytes++;
	} 
	 crc &= 0xFFFF; 
	if( (crc>>8) != *crc_master ) return -1;
	if( (u8)crc != *(crc_master+1) ) return -1;
	return 0;
} 
uint16_t YmodemCrc16(uint8_t *bytes, uint32_t len,uint32_t crc) 
{ 
	//uint32_t crc = 0x0000;   
	uint32_t i=0, j=0;  
	for(i = 0; i < len; i++)    
	{ 
		crc = crc ^ (unsigned int)(* bytes) << 8;   
		 
		for(j = 0; j < 8; j++)      
		{ 
			if(crc & 0x8000)        
				crc = crc << 1 ^ POLY16;       
			else 
	    		crc = crc << 1;     
		}  
		bytes++;
	} 
	 crc &= 0xFFFF; 

	return crc;
}







/**RS485初始化***/
void RS485_init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);		//??USART1,GPIOA??
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 	USART_DeInit(USART1); 															 //????3

	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 									     //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									 //??????
    GPIO_Init(GPIOA, &GPIO_InitStructure);									         //???PA9   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							 //????
    GPIO_Init(GPIOA, &GPIO_InitStructure);  										 //???PA10
	//485enable
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;							 
    GPIO_Init(GPIOB, &GPIO_InitStructure);  										 //???PB13
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);

    //USART
	USART_InitStructure.USART_BaudRate = bound;										 //?????9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						 //???8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							 //?????
	USART_InitStructure.USART_Parity = USART_Parity_No;								 //??????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					 //????
	USART_Init(USART1, &USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;						//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;								//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQ????
	NVIC_Init(&NVIC_InitStructure);													//??????????VIC???

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									//????
	USART_Cmd(USART1, ENABLE);     
										 	
}
void Config_Status_Send(void)
{
	uint16_t j = 1000;
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	while(j-- > 1);
}
void Config_Status_Recieve(void)
{
	uint16_t j = 1000;
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	while(j-- > 1);
}
void SerialPutChar_485(uint8_t c)
{
	uint16_t j=5000;
	uint16_t count=0;
	Config_Status_Send();
	USART_SendData(USART1, c);

	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
		count ++;
		if(count >= 10000) 
		{
		Config_Status_Recieve();
		return;
		}
		
	}
	while(j-->1);
	Config_Status_Recieve();
}

void Serial_PutStr00_485(uint8_t *s,uint8_t len)
{	
	uint8_t i=0;
	uint16_t j=10000;
	Config_Status_Send();
	for(i=len;i>0;i--)
	{
		SerialPutChar_485(*s);
		s ++;
	}
	while(j-->1);
	Config_Status_Recieve();
}

/**DMX512初始化***/
//250Kbps
void DMX512_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 	USART_DeInit(USART1); 															 

	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 									     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									
    GPIO_Init(GPIOA, &GPIO_InitStructure);									         
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							
    GPIO_Init(GPIOA, &GPIO_InitStructure);  										
	//485enable
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;							 
    GPIO_Init(GPIOB, &GPIO_InitStructure);  										 
	GPIO_SetBits(GPIOB,GPIO_Pin_13);

    //USART
	USART_InitStructure.USART_BaudRate = 250000;		
	//9 Bit
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;						 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							
	USART_InitStructure.USART_Parity = USART_Parity_No;								 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					
	USART_Init(USART1, &USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;						
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;								
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									
	NVIC_Init(&NVIC_InitStructure);													

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									
	USART_Cmd(USART1, ENABLE);     
										 	
}
static void SetTxConfig(unsigned char i)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 									     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	if(0 == i)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		
	else	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
    GPIO_Init(GPIOA, &GPIO_InitStructure);	
}
//char updata512(unsigned char *buf,unsigned int len,struct pwm_mcu_data *tar)
//{
//	uint16_t i = 0;
//	unsigned char *pt = buf;
//	//start code
//	*buf = 0;
//	//DIM code
//	for(i=1;i<=len;i++)
//	{
//		pt[i++] = tar->Red_PWM;
//		pt[i++]= tar->Green_PWM;
//		pt[i] = tar->Blue_PWM;
//	}
//	return 0;
//}
void DMX512_Send(unsigned char *buf,unsigned int len)
{
	unsigned int count_buf = 0;	
	uint32_t count=0;
	while(count_buf <= (len+1))
	{
		/*SPACE FOR BREAK*/
		if(0 >= count_buf)
		{
			SetTxConfig(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_9);
			while(count++ <= 1000);
			count = 0;
			GPIO_SetBits(GPIOA,GPIO_Pin_9);
			while(count++ <= 100);
			count = 0;
			SetTxConfig(0);
			
		}
		else
		{
			USART1->DR = 0X0100 | buf[count_buf-1];
		}
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		{
			count ++;
			if(count >= 10000) 
			{
			return;
			}		
		}
		count = 0;
		count_buf++;
	}
	
}



