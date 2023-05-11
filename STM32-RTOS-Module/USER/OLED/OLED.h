#ifndef _LQOLED_H
#define _LQOLED_H
#include "sys.h"
#include "stdlib.h"

//汉字大小，英文数字大小
#define 	TYPE8X16		1
#define 	TYPE16X16		2
#define 	TYPE6X8			3

//-----------------OLED端口定义----------------  					   

#define LCD_SCL_CLR()	GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define LCD_SCL_SET()	GPIO_SetBits(GPIOB,GPIO_Pin_13)

#define LCD_SDA_CLR()	GPIO_ResetBits(GPIOB,GPIO_Pin_15)
#define LCD_SDA_SET()	GPIO_SetBits(GPIOB,GPIO_Pin_15)

#define LCD_RST_CLR()	GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define LCD_RST_SET()	GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define LCD_DC_CLR()	GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define LCD_DC_SET()	GPIO_SetBits(GPIOB,GPIO_Pin_12)

void LCD_DLY_ms(u16 ms);
extern void LCD_Init(void);
extern void LCD_CLS(void);
void LCD_CLS_Y(u8 y);
extern void LCD_Str(u8 x,u8 y,char *ch,u8 len);

extern void Draw_BMP(u8 x,u8 y,const u8 *bmp); 
extern void LCD_Fill(u8 dat);
#endif

