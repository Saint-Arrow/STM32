#ifndef __USART_H
#define __USART_H
	
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2011/6/14
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////////////////////////////////////////////////////////////////////////////////// 	

/***********Exported variations***************/
#define NAK_TIMEOUT             (0x500000)
#define SOH 					0x01
#define EOT 					0x04
#define ACK						0x06
#define NAK						0x15
#define POLY16 0x1021  // ��λΪ��ʽ��д ʵ��Ϊ0x11021

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


