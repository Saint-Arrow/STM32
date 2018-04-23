/* Version :V1.0-----------------------------------------------------------------------*/
/* Includes ---------------------------------------------------------------------------*/
#include "main.h"

#define START_TASK_PRIO     1
#define START_STK_SIZE      128
TaskHandle_t STARTTask_Handler;
void START_task(void *pvParameters);

#define OLED_TASK_PRIO     2
#define OLED_STK_SIZE      128
TaskHandle_t OLEDTask_Handler;
void OLED_task(void *pvParameters);
uint8_t net_id_temp;
uint8_t led_flag=0;

#define Check_TASK_PRIO     3
#define Check_STK_SIZE      128
TaskHandle_t CheckTask_Handler;
void Check_task(void *pvParameters);
uint8_t init_ok_flag  =0;
uint8_t one_mode_flag =1;
uint8_t auto_push_flag=1;


#define USART_TX_Call_TASK_PRIO     4
#define USART_TX_Call_STK_SIZE      500
TaskHandle_t USART_TX_CallTask_Handler;
void USART_TX_Call_task(void *pvParameters);


#define KEY_TASK_PRIO     5
#define KEY_STK_SIZE      128
TaskHandle_t KEYTask_Handler;
void KEY_task(void *pvParameters);
#define KEYMSG_Q_NUM 6
QueueHandle_t Key_Queue;


#define USART_1_RX_Call_TASK_PRIO     6
#define USART_1_RX_Call_STK_SIZE      500
TaskHandle_t USART_1_RX_CallTask_Handler;
void USART_1_RX_Call_task(void *pvParameters);


#define USART_RX_Call_TASK_PRIO     7
#define USART_RX_Call_STK_SIZE      500
TaskHandle_t USART_RX_CallTask_Handler;
void USART_RX_Call_task(void *pvParameters);


TimerHandle_t 	AutoReloadTimer_Handle;			
TimerHandle_t	OneShotTimer_Handle;			

void AutoReloadCallback(TimerHandle_t xTimer); 
void OneShotCallback(TimerHandle_t xTimer);		

uint8_t dev_buf[5+10*ZLL_MAX_Dev];

void Hal_init(void);
void Data_Recovery(void);
int zll_parket_check(unsigned char *data,uint16_t len);
void qt_cmd(uint8_t *p,uint8_t len);
void zll_send_login(uint8_t *p,uint8_t len);
int main(void)
{
	Hal_init();
	xTaskCreate((TaskFunction_t   )START_task,
	            (const char*      )"START_task",
	            (uint16_t         )START_STK_SIZE,
	            (void*            )NULL,
	            (UBaseType_t      )START_TASK_PRIO,
	            (TaskHandle_t*    )&STARTTask_Handler
	           );
	vTaskStartScheduler();
}


void Hal_init(void)
{
	char net_str[4]="";
	uint8_t len;
	GPIO_Conf();
	LCD_Init();
	KEY_init();
	beep_init();
	uart1_init(115200);
	uart3_init(115200);
	Data_Recovery();
	if(gw_confige.net_id>=16) gw_confige.net_id=0;
	len=numtostr(gw_confige.net_id,net_str);
	LCD_Str(0,0,"GW",2);
	LCD_Str(6,0,"Net:",4);
	LCD_Str(10,0,net_str,len);
	LCD_CLS_Y(2);
	LCD_CLS_Y(3);
	//Init...
	LCD_Str(0,1,"ZB check...",strlen("ZB check..."));
	memset(dev_list,0XFF,sizeof(dev_list)); //初始化数据
	
	
	IWDG_Init(6,1250);
	IWDG_Feed();
}

void Data_Recovery(void)
{
	uint8_t flag=0;
	eeprom_SerialRead(0,0,(uint8_t *)&gw_confige,sizeof(gw_confige));
	if(VERSION != gw_confige.version) 
		flag=1;
	else
	{
		flag=memcmp(gw_confige.check_str,"mod",3);
	}
	if(flag)
	{
		gw_confige.check_str[0]='m';
		gw_confige.check_str[1]='o';
		gw_confige.check_str[2]='d';
		gw_confige.version=VERSION;
		gw_confige.net_id=0;
		eeprom_Erase(0);
		eeprom_SerialWrite(0,0,(uint8_t *)&gw_confige,sizeof(gw_confige));
	}
	net_id_temp=gw_confige.net_id;
	
	USART_TX_STA=0;
	USART_RX_STA=0;
}
int zll_parket_check(unsigned char *data,uint16_t len)
{
    unsigned char *p=data;
    unsigned char msg_len=0;
	
    while(*p == ZLL_MSG_HEAD)
    {
		if(len<3) return -3;
		
        msg_len = *(p+1);
        if(msg_len > len)  		{return len;}
		
        else
        {
            if(*(p+msg_len-1)==ZLL_MSG_END)
            {
                zll_parket_cmd(p,msg_len);                
								p=p+msg_len;
								len=len-msg_len;										                              
            }
            else return -2;
        }
    }
    if(*p != ZLL_MSG_HEAD)    return -1;
    
    return 0;
}
void qt_cmd(uint8_t *p,uint8_t len)  
{
	if(*(p+2) == 1)//READ DEV
	{
		auto_push_flag=0;
		zll_send_login(p,len);//向上位机发送登录设备的信息
	}
}
void zll_send_login(uint8_t *p,uint8_t len)
{
		len=list_read_num((uint8_t *)dev_list);	
		if(len < ZLL_MAX_Dev)
		{		
			uint8_t i=0;
			uint8_t *p=(uint8_t *)dev_list;		
			memset(dev_buf,0,sizeof(dev_buf));
			
			dev_buf[0] =0X7B;
			dev_buf[1] =5+10*len;//Length
			dev_buf[2] =0X02;
			dev_buf[3] =len; //设备数
			for(i=0;i<len;i++)
			{
				dev_buf[4+i*10+0]=i+1;//NUM
				dev_buf[4+i*10+1]=0;//RSSI
				dev_buf[4+i*10+2]=0;//LOGIN				
				dev_buf[4+i*10+3]=*(p+6);//TYPE			
				memcpy(&dev_buf[4+i*10+4],p,6);
				
				p+=7;
			}
			dev_buf[4+len*10]=0X7D;
			Serial_1_PutData(dev_buf,5+len*10);		
		}		
}
void AutoReloadCallback(TimerHandle_t xTimer)
{
	if(one_mode_flag&&auto_push_flag)
	{
		uint8_t dev_temp[6+1]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
		if(memcmp(dev_active,dev_temp,7)!=0)
		{
			uint8_t *p=(uint8_t *)dev_active;		
			dev_buf[0] =0X7B;
			dev_buf[1] =5+10;
			dev_buf[2] =0X02;
			dev_buf[3] =0X01; 
			dev_buf[4+0]=1;//NUM
			dev_buf[4+1]=0;//RSSI
			dev_buf[4+2]=0;//LOGIN				
			dev_buf[4+3]=*(p+6);//TYPE			
			memcpy(&dev_buf[4+4],p,6);				
			dev_buf[14]=0X7D;
			Serial_1_PutData(dev_buf,15);	
		}
	}
}
void OneShotCallback(TimerHandle_t xTimer)
{
	if(one_mode_flag)
	{//check version of device
		struct Get_Lamp_Status temp;
		temp.msg_head=0X7E;
		temp.msg_len=sizeof(struct Get_Lamp_Status);
		temp.msg_id=MSG_GET_STATUS;
		memcpy(temp.lamp_id,&dev_active[2],4);
		temp.msg_end=0X5E;
		add_to_send_list((uint8_t*)&temp,sizeof(struct Get_Lamp_Status));
	}
}
void START_task(void *pvParameters)
{
	taskENTER_CRITICAL();

	xTaskCreate((TaskFunction_t   )OLED_task,
	            (const char*      )"OLED_task",
	            (uint16_t         )OLED_STK_SIZE,
	            (void*            )NULL,
	            (UBaseType_t      )OLED_TASK_PRIO,
	            (TaskHandle_t*    )&OLEDTask_Handler
	           );
	xTaskCreate((TaskFunction_t   )Check_task,
	            (const char*      )"Check_task",
	            (uint16_t         )Check_STK_SIZE,
	            (void*            )NULL,
	            (UBaseType_t      )Check_TASK_PRIO,
	            (TaskHandle_t*    )&CheckTask_Handler
	           );

	xTaskCreate((TaskFunction_t   )USART_TX_Call_task,
	            (const char*      )"USART_TX_Call_task",
	            (uint16_t         )USART_TX_Call_STK_SIZE,
	            (void*            )NULL,
	            (UBaseType_t      )USART_TX_Call_TASK_PRIO,
	            (TaskHandle_t*    )&USART_TX_CallTask_Handler
	           );						 
	xTaskCreate((TaskFunction_t   )KEY_task,
	            (const char*      )"KEY_task",
	            (uint16_t         )KEY_STK_SIZE,
	            (void*            )NULL,
	            (UBaseType_t      )KEY_TASK_PRIO,
	            (TaskHandle_t*    )&KEYTask_Handler
	           );					 
	xTaskCreate((TaskFunction_t   )USART_RX_Call_task,
	            (const char*      )"USART_RX_Call_task",
	            (uint16_t         )USART_RX_Call_STK_SIZE,
	            (void*            )NULL,
	            (UBaseType_t      )USART_RX_Call_TASK_PRIO,
	            (TaskHandle_t*    )&USART_RX_CallTask_Handler
	           );		
		
	xTaskCreate((TaskFunction_t   )USART_1_RX_Call_task,
	            (const char*      )"USART_1_RX_Call_task",
	            (uint16_t         )USART_1_RX_Call_STK_SIZE,
	            (void*            )NULL,
	            (UBaseType_t      )USART_1_RX_Call_TASK_PRIO,
	            (TaskHandle_t*    )&USART_1_RX_CallTask_Handler
	           );		
  AutoReloadTimer_Handle=xTimerCreate((const char*		)"AutoReloadTimer",
									                    (TickType_t			)1000/portTICK_PERIOD_MS,
																			(UBaseType_t		)pdTRUE,
																			(void*				)1,
																			(TimerCallbackFunction_t)AutoReloadCallback); 
   
	OneShotTimer_Handle=xTimerCreate((const char*			)"OneShotTimer",
																	 (TickType_t			)1000/portTICK_PERIOD_MS,
																	 (UBaseType_t			)pdFALSE,
																	 (void*					)2,
																	 (TimerCallbackFunction_t)OneShotCallback); 
	xTimerStart(AutoReloadTimer_Handle,0);																 
	Key_Queue=xQueueCreate(1,sizeof(struct key_data));
	vTaskDelete(STARTTask_Handler);
	taskEXIT_CRITICAL(); 	
}
void USART_RX_Call_task(void *pvParameters)
{
	while(1)
	{
		uint32_t TimeIncrease_temp=xTaskGetTickCount();
		if(USART_RX_STA >0)
		{
				uint32_t timeout=0;
				timeout=cal_timeout(TimeIncrease_temp,usart_time);
				if(timeout >30)
				{							
					memcpy((char *)&RX_MCU[recv_num],(char*)USART_RX_BUF,USART_RX_STA);
					recv_num     += USART_RX_STA;
					if(RX_MCU[0] != 0X7E) Serial_1_PutData(RX_MCU,recv_num);
					usart_time   = TimeIncrease_temp;
					USART_RX_STA =0;								
					{
						int erro;
						erro=zll_parket_check((unsigned char *)RX_MCU,recv_num);
						
						if(erro > 0) //msg incomplete...
						{			
							#ifdef add_pack							
							memcpy((char *)&RX_MCU[0],(char*)(char *)&RX_MCU[recv_num-erro],erro);
							recv_num=erro;
							USART_assembe_flag=1;
							#else
							recv_num=0;
							#endif
							
						}						
						else	
						{					
							USART_assembe_flag=0;						
							recv_num=0;	
						}
					}
					LED_PB2_ON();												
				}
		}
		vTaskDelay(10);
		#ifdef add_pack
		if(USART_assembe_flag && !USART_RX_STA)
		{
			uint32_t timeout=0;
			timeout=cal_timeout(TimeIncrease_temp,usart_time);
			if(timeout >200)
			{	
					usart_time   = TimeIncrease_temp;			
					recv_num=0;
					USART_assembe_flag=0;	
					LED_PB2_ON();					
			}
		}
		#endif
	}
	
	
}
void USART_TX_Call_task(void *pvParameters)
{
	while(1)
	{
		uint16_t len;
		if(USART_TX_STA)
		{
			if(USART_TX_STA<=90)
			{
				len=USART_TX_STA;
				USART_TX_STA=0;
				memcpy((uint8_t *)USART_TX_BUF,(uint8_t*)&TX_MCU[0],len);			
				Serial_3_PutData(USART_TX_BUF,len);	
				vTaskDelay(200);				
			}
			else 
			{
				USART_TX_STA=0;
			}		
		}
		vTaskDelay(10);
	}
	
}



void usart1_pack(uint8_t *p,uint8_t len)
{
	if(*p == 0X7E) 
	{
		if((*(p+1) == len)&&(*(p+len-1) == 0X5E))
		  add_to_send_list(p,len);
	}
	else if(*p == 0X7B) 
	{
		if((*(p+1) == len)&&(*(p+len-1) == 0X7D))
			qt_cmd(p,len);
	}
	else 
		add_to_send_list(p,len);
}
void USART_1_RX_Call_task(void *pvParameters)
{
	while(1)
	{
		uint32_t TimeIncrease_temp=xTaskGetTickCount();
		if(USART_1_RX_STA)
		{
			uint32_t timeout=0;
			timeout=cal_timeout(TimeIncrease_temp,usart_1_time);
			if(timeout > 30)
			{				
				uint16_t len=USART_1_RX_STA;
				usart_1_time=TimeIncrease_temp;
				USART_1_RX_STA =0;	
				memcpy((char *)RX_1_MCU,(char*)USART_1_RX_BUF,len);
				usart1_pack(RX_1_MCU,len);
				LED_PB2_ON();							
			}
		}
		vTaskDelay(10);
	}
	
}
void Check_task(void *pvParameters)
{
	static unsigned char flag;
	while(1)
	{
		IWDG_Feed();
		if(!init_ok_flag)
		{
			uint16_t pro=check_profile();
			if(pro==4841)
			{
				LCD_Str(0,3,"ZB pro ok...",strlen("ZB pro ok..."));
				delay_ms(500);		
				delay_ms(500);
				setpid(gw_confige.net_id);		  //配置网关PID
				delay_ms(500);		
				delay_ms(500);
				setch(gw_confige.net_id);        //配置网关channel
				delay_ms(500);
        delay_ms(500);				
				//Init Complete
				init_ok_flag=1;
				LCD_CLS_Y(1);
				LCD_CLS_Y(2);
				LCD_CLS_Y(3);
				LCD_CLS_Y(4);
				LCD_CLS_Y(5);
				LCD_CLS_Y(6);
				LCD_CLS_Y(7);
				LCD_Str(0,1,"ZB Init.ok.",strlen("ZB Init.ok."));
				
			}
			else
			{
				LCD_Str(0,3,"ZB pro FAIL...",strlen("ZB pro FAIL..."));					
			}
		}
		vTaskDelay(1000);	
		if(init_ok_flag)
		{			
				if(flag)
				{
					flag=0;
					if(one_mode_flag == 1)
					{
						LCD_Str(14,0,"ONE_M..",7);
					}
					else if(one_mode_flag == 0)
						LCD_Str(14,0,"     ..",7);
				}
				else
				{
					flag=1;
					if(one_mode_flag == 1)
					{
						LCD_Str(14,0,"ONE_M. ",7);
					}
					else if(one_mode_flag == 0)
						LCD_Str(14,0,"     . ",7);
				}
		}			
	}
	
}
void KEY_task(void *pvParameters)
{
  //status按键状态机状态
	//key_buf按键缓存
	//key_timeout消抖
	//key_jsq
	static unsigned char status,key_buf;
	static unsigned char key_timeout;
	static unsigned char key_jsq;
	unsigned char key_function=0,key_num=0;
	
	while(1)
	{
		switch(status)
		{
			case 0:
			{
				key_num=check_key_press();
				if(NO_PRESS < key_num) 
				{
					status=1;
					key_buf = key_num;
					key_timeout=0;
				}
				else
				{					
					key_buf=NO_PRESS;				
					status=0;
				}
				break;
			}
			case 1:
			{
				if(key_timeout>=AN_XD_DL)
				{
					status=2;			
				}
				else key_timeout++;
				
				break;
			}
			case 2:
			{
				key_num=check_key_press();
				if(NO_PRESS < key_num)
				{
					if(key_num == key_buf) //means the key  has not changed 
					{
						key_jsq=0;
						status = 3;															
					}
					else 
						status = 0;	
				}
				else
				{
					status = 0;	
				}
				break;
			}
			case 3:
			{
				key_num=check_key_press();
				if(key_buf == key_num)
				{					
					if(key_jsq >= AN_CJ_DL)//time delay for long press is running out
					{
						#ifdef  KEY_LongPress_enable
						key_function=KEY_LongPress;	
						#endif						
					}	
					else key_jsq++;			
				}
				else
				{			
					status = 0;
					if(NO_PRESS == key_num)
					{//release
						if(key_jsq >= AN_CJ_DL) //long press is already processed
						{	
							key_function=KEY_LongPress_Ralease;					
						}
						else//short press
						{
							key_function=KEY_Click_Release;						
						}
					}
					else
					{//other key
						
					}
					
				}
				break;
			}
			default: break;
		}
	  if(key_function)
		{
			struct key_data temp1;
			temp1.key_status=key_function;
			temp1.key_num=key_buf;
			temp1.key_time=xTaskGetTickCount();
			xQueueSend(Key_Queue,&temp1,0);
			key_function=0;		
		}
		vTaskDelay(10);
}
	
		
}
void OLED_task(void *pvParameters)
{
	struct key_data temp2;
	unsigned char key_mode,key_num;
	unsigned int key_time;
	//for double kill
	unsigned char key_mode_new,key_num_new;
	unsigned int key_time_new;
	while(1)
	{
		if(Key_Queue)
		{
			if(xQueueReceive(Key_Queue,&temp2,portMAX_DELAY))
			{
				
				key_mode=temp2.key_status;
				key_num=temp2.key_num;	
				key_time=temp2.key_time;
				if(KEY_Click_Release == key_mode)
				{
					switch(key_num)
					{					
						case S1_PRESS:
						{
							struct key_data temp3;
							if(xQueueReceive(Key_Queue,&temp3,500))
							{
								key_mode_new=temp3.key_status;
								key_num_new=temp3.key_num;	
								key_time_new=temp3.key_time;
								if((key_num_new==key_num)&&(key_mode_new==key_mode)&&((key_time_new-key_time)<500))
								{
									if(!init_ok_flag) return;
									gw_confige.net_id=net_id_temp;
									eeprom_Erase(0);
									eeprom_SerialWrite(0,0,(uint8_t *)&gw_confige,sizeof(gw_confige));
									SoftReset();	
								}
							}						
							else
							{
								char net_str[4]="";		
								uint8_t len;
								if(!init_ok_flag) return;
								net_id_temp++;
								if(net_id_temp>=16) net_id_temp=0;
								len=numtostr(net_id_temp,net_str);
								LCD_CLS_Y(1);
								LCD_Str(0,1,"Net:",4);
								LCD_Str(4,1,net_str,len);	
							}
							break;
						}
						case S2_PRESS:
						{
							if(one_mode_flag) 
								one_mode_flag=0;
							else one_mode_flag=1;
							break;
						}
						case S3_PRESS:
						{
							uint8_t LED[6]={0X7E,0X06,0X01,0X00,0XFF,0X5E};					
							LED[3] = gw_confige.net_id;
							if(led_flag)
							{
								led_flag=0;
								LED[4] =0X01;
								add_to_send_list(LED,6);
							}
							else
							{
								led_flag=1;
								LED[4] =0X00;
								add_to_send_list(LED,6);
							}
							break;
						}
						default:
							break;
					}
				}	
        else if(KEY_LongPress_Ralease == key_mode)	
				{
					switch(key_num)
					{		
						case S2_PRESS:
						{
							char out[10];
							UBaseType_t task1 = uxTaskGetStackHighWaterMark(STARTTask_Handler);
							UBaseType_t task2 = uxTaskGetStackHighWaterMark(OLEDTask_Handler);
							UBaseType_t task3 = uxTaskGetStackHighWaterMark(CheckTask_Handler);
							UBaseType_t task4 = uxTaskGetStackHighWaterMark(USART_TX_CallTask_Handler);
							UBaseType_t task5 = uxTaskGetStackHighWaterMark(KEYTask_Handler);
							UBaseType_t task6 = uxTaskGetStackHighWaterMark(USART_1_RX_CallTask_Handler);
							UBaseType_t task7 = uxTaskGetStackHighWaterMark(USART_RX_CallTask_Handler);

							
							numtostr(task1,out);
							Serial_1_PutString(out);
							Serial_1_PutChar(' ');
							numtostr(task2,out);
							Serial_1_PutString(out);
							Serial_1_PutChar(' ');
							numtostr(task3,out);
							Serial_1_PutString(out);
							Serial_1_PutChar(' ');
							numtostr(task4,out);
							Serial_1_PutString(out);
							Serial_1_PutChar(' ');
							numtostr(task5,out);
							Serial_1_PutString(out);
							Serial_1_PutChar(' ');
							numtostr(task6,out);
							Serial_1_PutString(out);
							Serial_1_PutChar(' ');
							numtostr(task7,out);
							Serial_1_PutString(out);
							Serial_1_PutChar(' ');
							
				
							
						}						
						default:
							break;
					}
				}					
				else //KEY_LongPress
				{
					switch(key_num)
					{
						case S1_PRESS:
						{
							char net_str[4]="";		
							uint8_t len;
							if(!init_ok_flag) return;
							net_id_temp++;
							if(net_id_temp>=16) net_id_temp=0;
							len=numtostr(net_id_temp,net_str);
							LCD_CLS_Y(1);
							LCD_Str(0,1,"Net:",4);
							LCD_Str(4,1,net_str,len);
							break;
						}
						
						default:
							break;
					}
				}
			}
		}
	}
}
