#include "delay.h"

void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
void Delay_1ms(void)
{
	uint16_t nCount=10000;
    for(;nCount > 0; nCount--);
}
void delay_ms(uint16_t nCount)
{
	uint16_t i,j;
	for(i=0;i<nCount;i++)
		for(j=0;j<10000;j++);
}
















