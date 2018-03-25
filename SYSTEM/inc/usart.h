#ifndef __USART_H
#define __USART_H
	
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.csom
//修改日期:2011/6/14
//版本：V1.4
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
////////////////////////////////////////////////////////////////////////////////// 	

/***********Exported variations***************/
#define NAK_TIMEOUT             (0x500000)
#define SOH 					0x01
#define EOT 					0x04
#define ACK						0x06
#define NAK						0x15
#define POLY16 0x1021  // 该位为简式书写 实际为0x11021

#define USART_REC_LEN  		                	256  	
#define USART_TX_LEN 		                   	100

extern uint16_t recv_num;
extern uint32_t usart_time;
extern uint16_t USART_TX_STA;
extern uint8_t USART_TX_BUF[USART_TX_LEN],TX_MCU[USART_TX_LEN];     				   						       
extern volatile uint16_t USART_RX_STA;    
extern uint8_t USART_RX_BUF[USART_REC_LEN],RX_MCU[USART_REC_LEN];
extern uint8_t USART_assembe_flag;



/*******usart1******/
extern uint32_t usart_1_time;
extern volatile uint16_t USART_1_RX_STA;    
extern uint8_t USART_1_RX_BUF[USART_REC_LEN],RX_1_MCU[USART_REC_LEN];

/* Exported functions ------------------------------------------------------- */
void uart1_init(uint32_t bound);
void uart3_init(uint32_t bound);
void add_to_send_list(uint8_t *p,uint8_t len);

void Serial_3_PutChar(uint8_t c);
void Serial_3_PutString(uint8_t *s);
void Serial_3_PutData(uint8_t *s,uint16_t len);

void Serial_1_PutChar(uint8_t c);
void Serial_1_PutString(uint8_t *s);
void Serial_1_PutData(uint8_t *s,uint16_t len);

uint16_t YmodemCrc16(uint8_t *bytes, uint32_t len,uint32_t crc);
#endif


