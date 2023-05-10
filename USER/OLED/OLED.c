/*OLED�ײ�����
*@��������ʽ���ֱ���Ϊ128*64
*��Ÿ�ʽ����.
*[0]0 1 2 3 ... 127	
*[1]0 1 2 3 ... 127	
*[2]0 1 2 3 ... 127	
*[3]0 1 2 3 ... 127	
*[4]0 1 2 3 ... 127	
*[5]0 1 2 3 ... 127	
*[6]0 1 2 3 ... 127	
*[7]0 1 2 3 ... 127 
*/
#include "OLED.h"



#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xCF 


#define X_WIDTH 128
#define Y_WIDTH 64
//======================================
const u8 F6x8[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,   // sp
    0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 ,   // !
    0x00, 0x00, 0x07, 0x00, 0x07, 0x00 ,   // "
    0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 ,   // #
    0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 ,   // $
    0x00, 0x62, 0x64, 0x08, 0x13, 0x23 ,   // %
    0x00, 0x36, 0x49, 0x55, 0x22, 0x50 ,   // &
    0x00, 0x00, 0x05, 0x03, 0x00, 0x00 ,   // '
    0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 ,   // (
    0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 ,   // )
    0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 ,   // *
    0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 ,   // +
    0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 ,   // ,
    0x00, 0x08, 0x08, 0x08, 0x08, 0x08 ,   // -
    0x00, 0x00, 0x60, 0x60, 0x00, 0x00 ,   // .
    0x00, 0x20, 0x10, 0x08, 0x04, 0x02 ,   // /
    0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E ,   // 0
    0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 ,   // 1
    0x00, 0x42, 0x61, 0x51, 0x49, 0x46 ,   // 2
    0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 ,   // 3
    0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 ,   // 4
    0x00, 0x27, 0x45, 0x45, 0x45, 0x39 ,   // 5
    0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 ,   // 6
    0x00, 0x01, 0x71, 0x09, 0x05, 0x03 ,   // 7
    0x00, 0x36, 0x49, 0x49, 0x49, 0x36 ,   // 8
    0x00, 0x06, 0x49, 0x49, 0x29, 0x1E ,   // 9
    0x00, 0x00, 0x36, 0x36, 0x00, 0x00 ,   // :
    0x00, 0x00, 0x56, 0x36, 0x00, 0x00 ,   // ;
    0x00, 0x08, 0x14, 0x22, 0x41, 0x00 ,   // <
    0x00, 0x14, 0x14, 0x14, 0x14, 0x14 ,   // =
    0x00, 0x00, 0x41, 0x22, 0x14, 0x08 ,   // >
    0x00, 0x02, 0x01, 0x51, 0x09, 0x06 ,   // ?
    0x00, 0x32, 0x49, 0x59, 0x51, 0x3E ,   // @
    0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C ,   // A
    0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 ,   // B
    0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 ,   // C
    0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C ,   // D
    0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 ,   // E
    0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 ,   // F
    0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A ,   // G
    0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F ,   // H
    0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 ,   // I
    0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 ,   // J
    0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 ,   // K
    0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 ,   // L
    0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F ,   // M
    0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F ,   // N
    0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E ,   // O
    0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 ,   // P
    0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E ,   // Q
    0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 ,   // R
    0x00, 0x46, 0x49, 0x49, 0x49, 0x31 ,   // S
    0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 ,   // T
    0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F ,   // U
    0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F ,   // V
    0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F ,   // W
    0x00, 0x63, 0x14, 0x08, 0x14, 0x63 ,   // X
    0x00, 0x07, 0x08, 0x70, 0x08, 0x07 ,   // Y
    0x00, 0x61, 0x51, 0x49, 0x45, 0x43 ,   // Z
    0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 ,   // [
    0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 ,   // 55
    0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 ,   // ]
    0x00, 0x04, 0x02, 0x01, 0x02, 0x04 ,   // ^
    0x00, 0x40, 0x40, 0x40, 0x40, 0x40 ,   // _
    0x00, 0x00, 0x01, 0x02, 0x04, 0x00 ,   // '
    0x00, 0x20, 0x54, 0x54, 0x54, 0x78 ,   // a
    0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 ,   // b
    0x00, 0x38, 0x44, 0x44, 0x44, 0x20 ,   // c
    0x00, 0x38, 0x44, 0x44, 0x48, 0x7F ,   // d
    0x00, 0x38, 0x54, 0x54, 0x54, 0x18 ,   // e
    0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 ,   // f
    0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C ,   // g
    0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 ,   // h
    0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 ,   // i
    0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 ,   // j
    0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 ,   // k
    0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 ,   // l
    0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 ,   // m
    0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 ,   // n
    0x00, 0x38, 0x44, 0x44, 0x44, 0x38 ,   // o
    0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 ,   // p
    0x00, 0x18, 0x24, 0x24, 0x18, 0xFC ,   // q
    0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 ,   // r
    0x00, 0x48, 0x54, 0x54, 0x54, 0x20 ,   // s
    0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 ,   // t
    0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C ,   // u
    0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C ,   // v
    0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C ,   // w
    0x00, 0x44, 0x28, 0x10, 0x28, 0x44 ,   // x
    0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C ,   // y
    0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 ,   // z
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14     // horiz lines
};



void LCD_WrDat(u8 data)
{
	u8 i=8;
	//LCD_CS=0;;
	LCD_DC_SET();
  LCD_SCL_CLR();
  __nop();     
  while(i--)
  {
    if(data&0x80){LCD_SDA_SET();}
    else{LCD_SDA_CLR();}
    LCD_SCL_SET(); 
    __nop(); 
		__nop(); 		
    LCD_SCL_CLR();    
    data<<=1;    
  }
	//LCD_CS=1;
}
void LCD_WrCmd(u8 cmd)
{
	u8 i=8;
	
	//LCD_CS=0;;
	LCD_DC_CLR();
  LCD_SCL_CLR();
  __nop();    
  while(i--)
  {
    if(cmd&0x80){LCD_SDA_SET();}
    else{LCD_SDA_CLR();}
    LCD_SCL_SET();
    __nop();   
		__nop();     
    LCD_SCL_CLR();    
    cmd<<=1;;   
  } 	
	//LCD_CS=1;
}

/*������ʼ��ַ
*Y���ǰ�8��ݽ��ģ�y��0~63��ֻ�ܰ�8��ı�����ʾ��
*��Ϊ����ʽֻ�ܰ�8���ֽڽ���
*/
void LCD_Set_Pos(u8 x, u8 y)
{ 
  LCD_WrCmd(0xb0+(y>>3));
  LCD_WrCmd(((x&0xf0)>>4)|0x10);
  //LCD_WrCmd((x&0x0f)|0x01); 
	LCD_WrCmd(x&0x0f);
} 
void LCD_Fill(u8 bmp_data)
{
	u8 y,x;
	
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x00);
		LCD_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(bmp_data);
	}
}
void LCD_CLS(void)
{
	u8 y,x;	
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x00);
		LCD_WrCmd(0x10); 
		for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(0);
	}
}
void LCD_DLY_ms(u16 ms)
{                         
  u16 a;
  while(ms)
  {
    a=1500*10;
    while(a--);
    ms--;
  }
  return;
}
void LCD_CLS_Y(u8 y)
{
	u8 x;
	LCD_Set_Pos(0,8*y);
	for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(0);
}
void LCD_ch(u8 x,u8 y,u8 ch) //F6x8 show
{
	u8 i;
	u16 j;
	if(x>21) return;
	if(y>8) return;
	if(ch<32) return;
	if(ch>127) return;
	LCD_Set_Pos(x*6,8*y);
	
	j=(ch-32)*6;	
	for(i=0;i<6;i++)
	{
		LCD_WrDat(F6x8[j++]);
	}
}
void LCD_Str(u8 x,u8 y,char *ch,u8 len)
{
	u8 i,k;
	u16 j;
	if((x+len)>21) return;
	if(y>8) return;

	LCD_Set_Pos(x*6,8*y);
	for(i=0;i<len;i++)
	{
		
		if(*ch<32 || *ch >127) 
    	j=0;
		else j=(*ch-32)*6;	
		
		for(k=0;k<6;k++)
		{
			LCD_WrDat(F6x8[j++]);
		}
		ch++;
	}
}
void LCD_Init(void)        
{
	GPIO_InitTypeDef  GPIO_InitStructure;	 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;				 		     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		    
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 		     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		    
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
  //DDRA=0XFF;
  
	LCD_SCL_SET();
	//LCD_CS=1;	//Ԥ��SLK��SSΪ�ߵ�ƽ  
	 	
	
	LCD_RST_CLR();
	LCD_DLY_ms(100);
	LCD_RST_SET();
	
  LCD_WrCmd(0xae);//--turn off oled panel
	LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  LCD_WrCmd(0x3f);//--1/64 duty
	LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  LCD_WrCmd(0x00);//-not offset
	LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
  LCD_WrCmd(0x14);//--set(0x10) disable
	LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  LCD_WrCmd(0x02);//
	LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
  LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����	
	LCD_WrCmd(0xda);//--set com pins hardware configuration
  LCD_WrCmd(0x12);
	LCD_WrCmd(0x81);//--set contrast control register
  LCD_WrCmd(0x7f); // Set SEG Output Current Brightness
	LCD_WrCmd(0xd9);//--set pre-charge period
  LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	LCD_WrCmd(0xdb);//--set vcomh
  LCD_WrCmd(0x30);//Set VCOM Deselect Level
	
	

 

  LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
  LCD_WrCmd(0xaf);//--turn on oled panel
	
	
	LCD_CLS();
	//LCD_Str(0,0,"GW...",2);
} 



//==============================================================
//�������� void Draw_BMP(u8 x,u8 y)
//������������ʾBMPͼƬ128��64
//��������ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��63
//���أ���
//==============================================================
void Draw_BMP(u8 x,u8 y,const u8 *bmp)
{ 	
	u8 wm=0;
	u8 adder=0;
  
  	LCD_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)               
	{
		LCD_WrDat(*(bmp+adder));	
		adder += 1;
	}      
	LCD_Set_Pos(x,y + 8); 
	for(wm = 0;wm < 16;wm++)          
	{
		LCD_WrDat(*(bmp+adder));
		adder += 1;
	} 
 /* u32 ii=0;
  u8 x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<=y1;y++)
	{
		LCD_Set_Pos(x0,y);				
    for(x=x0;x<x1;x++)
	    {      
	    	LCD_WrDat(bmp[ii++]);	    	
	    }
	}	*/
}

