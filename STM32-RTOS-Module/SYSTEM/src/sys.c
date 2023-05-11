#include "sys.h"
uint32_t IC_Uid;
uint32_t TimeIncrease;
void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	

}
void SoftReset(void)
{
   __set_FAULTMASK(1);      // 关闭所有中端
   NVIC_SystemReset();// 复位
}

void STM32_SysTickInit(uint32_t HzPreSecond)
{
    // HzPreSecond = 1000 to Setup SysTick Timer for 1 msec interrupts.
   
    if (SysTick_Config(SystemCoreClock / HzPreSecond))
    { 
        /* Capture error */ 
        while (1);
    }
}
uint32_t cal_timeout(uint32_t t_sys,uint32_t t_usart)
{
	uint32_t timeout;
	 
	 if(t_sys >= t_usart) 
	 	timeout = t_sys - t_usart;
	 else 	
	 	timeout = UINT32_MAX - t_usart + t_sys;
	 return timeout;
}

//http://blog.csdn.net/cp1300/article/details/48035667
uint32_t crc32_byte(uint8_t *ptr, uint32_t len)  
{  
    uint32_t xbit;  
    uint32_t data;  
    uint32_t CRC32 = 0xFFFFFFFF;      
    uint32_t bits;  
    uint32_t i;  
    uint8_t buff[4];  
    uint32_t *p32 = (uint32_t *)buff;  
    const u32 dwPolynomial = 0x04c11db7;  
      
    buff[0] = buff[1] = buff[2] = buff[3] = 0XFF;  
    for(i = 0;i < len;i ++)  
    {  
        xbit = (unsigned int)1 << 31;  
          
        buff[3] = ptr[i];  
        data = *p32;  
        for(bits = 0; bits < 32; bits++)   
        {  
            if (CRC32 & 0x80000000) {  
                CRC32 <<= 1;  
                CRC32 ^= dwPolynomial;  
            }  
            else  
                CRC32 <<= 1;  
            if (data & xbit)  
                CRC32 ^= dwPolynomial;  
  
            xbit >>= 1;  
        }  
    }  
    return CRC32;  
}  
void Get_SerialNum(void) 
{ 
  	uint32_t Device_Serial0, Device_Serial1, Device_Serial2;
	  uint8_t temp_id[12];	
    Device_Serial0 = *(vu32*)(0x1FFFF7E8); 
    Device_Serial1 = *(vu32*)(0x1FFFF7EC); 
    Device_Serial2 = *(vu32*)(0x1FFFF7F0);   
	  temp_id[0] = (u8)(Device_Serial0 & 0x000000FF);
    temp_id[1] = (u8)((Device_Serial0 & 0x0000FF00)>>8);
    temp_id[2] = (u8)((Device_Serial0 & 0x00FF0000)>>16);
    temp_id[3] = (u8)((Device_Serial0 & 0xFF000000)>>24);
    temp_id[4] = (u8)(Device_Serial1 & 0x000000FF);
    temp_id[5] = (u8)((Device_Serial1 & 0x0000FF00)>>8);
    temp_id[6] = (u8)((Device_Serial1 & 0x00FF0000)>>16);
    temp_id[7] = (u8)((Device_Serial1 & 0xFF000000)>>24);
    temp_id[8] = (u8)(Device_Serial2 & 0x000000FF);
    temp_id[9] = (u8)((Device_Serial2 & 0x0000FF00)>>8);
    temp_id[10] = (u8)((Device_Serial2 & 0x00FF0000)>>16);
    temp_id[11] = (u8)((Device_Serial2 & 0xFF000000)>>24);	
	  IC_Uid = crc32_byte(temp_id,sizeof(temp_id));
}


