#include "GW.h"
struct GW_FLASH gw_confige;
uint8_t key_isr,key_ser_num;
uint8_t pritf_y=2;

uint8_t dev_list[6+1][ZLL_MAX_Dev]; //MAC + TYPE
uint8_t dev_active[6+1]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
extern TimerHandle_t	OneShotTimer_Handle;	

uint8_t mac_4B_tostr(uint8_t *mac,uint8_t *p)
{
	uint8_t len=0;
	uint8_t dh,dl;
	for(len=0;len<4;len++)
	{
		dh=*mac>>4;
		dl=*mac &0X0F;
		if(dh<=9) *p='0'+dh;
		if(dh>9) *p='A'+dh-10;
		p++;
		if(dl<=9) *p='0'+dl;
		if(dl>9) *p='A'+dl-10;
		p++;
		mac++;
	}
	return 0;
}

void list_add(uint8_t *mac,uint8_t type,uint8_t *mac_buf)
{
	uint8_t *p=mac_buf; //登记入表的设备
	uint8_t i=0;
	uint8_t FF_Flag=ZLL_MAX_Dev;
	uint8_t mac_ff[6]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
	for(i=0;i<ZLL_MAX_Dev;i++)
	{
			if(memcmp(mac,p,6)==0) //已登记入表
			{
				if(type != *(p+6))  
					*(p+6) = type;
				return;
			}
			if(memcmp(mac_ff,p,6)==0) //新设备
			{
				FF_Flag=i;
				break;
			}			
			p+=7;	
	}

	if(FF_Flag != ZLL_MAX_Dev)
	{
		memcpy(p,mac,6);
		*(p+6)=type;
	}
}
void list_delete(uint8_t *mac,uint8_t *mac_buf)
{
	uint8_t *p=mac_buf;
	uint8_t i,status=0;
	for(i=0;i<ZLL_MAX_Dev;i++)
	{
		if(memcmp(mac,p,6)==0)
		{
			status=1;
			break;
		}	
		p+=7;
			
	}

	if(status)
	{
		memset(p,0XFF,7);
		for(;i<(ZLL_MAX_Dev);i++)
		{
			*p=*(p+7);
			*(p+1)=*(p+8);
			*(p+2)=*(p+9);
			*(p+3)=*(p+10);
			*(p+4)=*(p+11);
			*(p+5)=*(p+12);
			*(p+6)=*(p+13);
			p+=7;
		}
		memset(p,0XFF,7);
	}
}
uint8_t list_read_num(uint8_t *mac_buf)
{
  uint8_t *p=mac_buf;
	uint8_t i;
	uint8_t mac_ff[6]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
	for(i=0;i<ZLL_MAX_Dev;i++)
	{
			if(memcmp(mac_ff,p,6)==0)
			{
				if(i==0) 
					return (ZLL_MAX_Dev+1);
				else
				  return (i);
			}
			
			p+=7;	
	}
	return ZLL_MAX_Dev;
}

void SHUNZHOU_reset(void)
{
	uint8_t reset_cmd[2]={0X23,0X23};
	Serial_3_PutData(reset_cmd,2);
}
uint16_t SHUNZHOU_CFG(void)
{
	uint8_t read_cmd[2]={0X23,0XA0};
	
  uint32_t profile_timeout=TimeIncrease;
	Serial_3_PutData(read_cmd,2);
	USART_RX_STA=0;
	
	while(1)
	{
		if(USART_RX_STA)
		{
			uint32_t timeout=0;
			timeout=cal_timeout(TimeIncrease,usart_time);
			if(timeout >30)
			{
				memcpy((char *)&RX_MCU[0],(char*)USART_RX_BUF,USART_RX_STA);
				usart_time   = TimeIncrease;
				Serial_1_PutData(RX_MCU,USART_RX_STA);
				USART_RX_STA =0;
				LED_PB2_ON();
				//check
				if(0XA2==RX_MCU[0])
				{
					uint8_t w_cmd[16];
					w_cmd[0]=0X23;
					w_cmd[1]=0XFE;
					memcpy(&w_cmd[2],&RX_MCU[1],14);
					
					//modify the data
					w_cmd[1+3]=0X01;//NET ID
					w_cmd[1+4]=0X01;//NET ID
					w_cmd[1+5]=0X01;//node type
					w_cmd[1+6]=0X01;
					w_cmd[1+7]=0X06;
					w_cmd[1+8]=0X01;
					w_cmd[1+9]=0X01;
					w_cmd[1+12]=0X04;//freq id
					
					Serial_3_PutData(w_cmd,16);
					Serial_1_PutData(w_cmd,16);
					return 1;
				}
				
				return 0;
			}
	 }
	 if(cal_timeout(TimeIncrease,profile_timeout)>500)  return 0;
 }
	 
}
uint16_t check_profile(void)
{
	uint8_t str[]="AT+SHOWPARA=01";
	uint32_t profile_timeout=xTaskGetTickCount();
	USART_RX_STA=0;
	Serial_3_PutString(str);
	
	
	while(1)
	{
		uint32_t TimeIncrease_temp=xTaskGetTickCount();
		if(cal_timeout(TimeIncrease_temp,profile_timeout)>1000)  return 0;
		if(USART_RX_STA)
		{
			uint32_t timeout=0;			
			timeout=cal_timeout(TimeIncrease_temp,usart_time);
			if(timeout >30)
			{
				memcpy((char *)&RX_MCU[0],(char*)USART_RX_BUF,USART_RX_STA);
				usart_time   = TimeIncrease_temp;
				if(USART_RX_STA==68)
				{
					if((RX_MCU[0]==0X0D)&&(RX_MCU[1]==0X0A)&&(RX_MCU[66]==0X0D)&&(RX_MCU[67]==0X0A))
					{
						uint16_t profile=(RX_MCU[34]-'0')*10+(RX_MCU[35]-'0')+(RX_MCU[36]-'0')*1000+(RX_MCU[37]-'0')*100;
						LCD_Str(0,2,"ZB pro=",strlen("ZB pro="));
						LCD_Str(7,2,&RX_MCU[36],2);
						LCD_Str(9,2,&RX_MCU[34],2);
						return profile;
					}				
				}
				USART_RX_STA =0;				
			}	
		}
		else vTaskDelay(10);
		
	}
}
void setpid(uint8_t netid)
{
	uint8_t str[25]="AT+SETPID=686E518C";
	switch(netid%4)
	{
		case 0:str[17]='C';break;
		case 1:str[17]='D';break;
		case 2:str[17]='E';break;
		case 3:str[17]='F';break;	  
	}
	Serial_3_PutString(str);
}
void setch(uint8_t netid)
{
	uint8_t str[25]="AT+SETCH=";
	switch(netid/4)
	{
		case 0:str[9]='0';str[10]='B';break;
		case 1:str[9]='0';str[10]='F';break;
		case 2:str[9]='1';str[10]='4';break;
		case 3:str[9]='1';str[10]='9';break;
	}
	Serial_3_PutString(str);
}
void check_netid(void)
{
	
}

void zb_heartbeat_call(uint8_t *p ,uint8_t len)
{
	 unsigned char mac_addr[6] = {0X6F,0X00,0,0,0,0};
	 struct ZLL_HeartBeat *p_new=(struct ZLL_HeartBeat *)p;
	
	  struct ZLL_HeartBeat_Answer rsp;
		rsp.msg_head=ZLL_MSG_HEAD;
		rsp.msg_id=ID_NET_HeartBeat_Answer;
		rsp.msg_len=sizeof(rsp);
		memcpy(rsp.ROT_mac,p_new->ROT_mac,4);
		rsp.msg_end=ZLL_MSG_END;
		add_to_send_list((uint8_t *)&rsp,sizeof(rsp));
		memcpy(&mac_addr[2],p_new->ROT_mac,4);

		list_add(mac_addr,p_new->type,(uint8_t *)dev_list);

		memcpy(dev_active,mac_addr,6);
		dev_active[6]=p_new->type;
    xTimerReset(OneShotTimer_Handle,0);
}
void zb_heartbeat_call_New(uint8_t *p ,uint8_t len)
{
	struct ZLL_HeartBeat_New *p_new=(struct ZLL_HeartBeat_New *)p;
	struct ZLL_HeartBeat_Answer rsp;
	rsp.msg_head=ZLL_MSG_HEAD;
	rsp.msg_id=ID_NET_HeartBeat_Answer;
	rsp.msg_len=sizeof(rsp);
	memcpy(rsp.ROT_mac,&(p_new->ROT_mac[2]),4);	
	rsp.msg_end=ZLL_MSG_END;
	add_to_send_list((uint8_t *)&rsp,sizeof(rsp));
	
	list_add(p_new->ROT_mac,p_new->type,(uint8_t *)dev_list);
	
	memcpy(dev_active,p_new->ROT_mac,6);
	dev_active[6]=p_new->type;
	xTimerReset(OneShotTimer_Handle,0);
}



void zb_pad_call(uint8_t *p ,uint8_t len)
{
	uint8_t key,key_ser;
	uint8_t str[8+1];
	struct ID_DATA_Set_Scene_and_rsp *p_new=(struct ID_DATA_Set_Scene_and_rsp *)p;
	
	mac_4B_tostr(p_new->ctrl_id,str);
	key=p_new->key+'0';
	key_ser='-';
	if(pritf_y>7) 
	{
		pritf_y=2;
		LCD_CLS_Y(2);
		LCD_CLS_Y(3);
		LCD_CLS_Y(4);
		LCD_CLS_Y(5);
		LCD_CLS_Y(6);
		LCD_CLS_Y(7);
	}
	LCD_Str(0,pritf_y,str,8);
	LCD_Str(15,pritf_y,&key,1);
	LCD_Str(17,pritf_y,&key_ser,1);
	pritf_y++;
	
	{
		struct ID_DATA_Set_Scene_and_rsp rsp;
		rsp.msg_head=ZLL_MSG_HEAD;
		rsp.msg_id=ID_DATA_Pad_set_Rqt;
		rsp.msg_len=sizeof(rsp);
		memcpy(rsp.ctrl_id,p_new->ctrl_id,4);
		rsp.key=p_new->key;
		rsp.msg_end=ZLL_MSG_END;
		add_to_send_list((uint8_t *)&rsp,sizeof(rsp));
	}
	
	
}
void zb_pad_call_2(uint8_t *p ,uint8_t len)
{
	uint8_t key,key_ser;
	uint8_t str[8+1];
	uint8_t key_ser_str[3];
	uint8_t ser_len;
	struct ID_DATA_Set_Scene_new *p_new=(struct ID_DATA_Set_Scene_new *)p;
	mac_4B_tostr(p_new->ctrl_id,str);
	key=p_new->key+'0';
	key_ser=p_new->key_ser;

	ser_len=numtostr(key_ser,key_ser_str);
	if(pritf_y>7) 
	{
		pritf_y=2;
		LCD_CLS_Y(2);
		LCD_CLS_Y(3);
		LCD_CLS_Y(4);
		LCD_CLS_Y(5);
		LCD_CLS_Y(6);
		LCD_CLS_Y(7);
	}
	LCD_Str(0,pritf_y,str,8);
	LCD_Str(15,pritf_y,&key,1);
	LCD_Str(17,pritf_y,key_ser_str,ser_len);
	pritf_y++;
	
	{
		struct ID_DATA_Set_Scene_and_rsp rsp;
		rsp.msg_head=ZLL_MSG_HEAD;
		rsp.msg_id=ID_DATA_Pad_set_Rqt;
		rsp.msg_len=sizeof(rsp);
		memcpy(rsp.ctrl_id,p_new->ctrl_id,4);
		rsp.key=p_new->key;
		rsp.msg_end=ZLL_MSG_END;
		add_to_send_list((uint8_t *)&rsp,sizeof(rsp));
	}
	
}
void zb_pad_call_3(uint8_t *p ,uint8_t len)
{
	uint8_t key,key_ser;
	uint8_t str[8+1];
	uint8_t key_ser_str[3];
	uint8_t ser_len;
	struct ID_DATA_Set_Scene_MAGIC *p_new=(struct ID_DATA_Set_Scene_MAGIC *)p;
	mac_4B_tostr(p_new->ctrl_id,str);
	key=p_new->key_cmd+'0';
	key_ser=p_new->key_ser;
	ser_len=numtostr(key_ser,key_ser_str);
	if(pritf_y>7) 
	{
		pritf_y=2;
		LCD_CLS_Y(2);
		LCD_CLS_Y(3);
		LCD_CLS_Y(4);
		LCD_CLS_Y(5);
		LCD_CLS_Y(6);
		LCD_CLS_Y(7);
	}
	LCD_Str(0,pritf_y,str,8);
	LCD_Str(15,pritf_y,&key,1);
	LCD_Str(16,pritf_y,"*",1);
	LCD_Str(17,pritf_y,key_ser_str,ser_len);
	pritf_y++;
	
	{
		struct ID_DATA_Set_Scene_MAGIC_rsp rsp;
		memcpy(&rsp,p_new,sizeof(struct ID_DATA_Set_Scene_MAGIC_rsp));
		rsp.msg_id=ID_DATA_PAD_magic_rsp;
		add_to_send_list((uint8_t *)&rsp,sizeof(rsp));
	}
}
void get_status_back(uint8_t *p,uint8_t len)
{
	//check version 
	if(one_mode_flag)
	{
		if(*(p+10)>=18)//year 2018
		{//ok
			beep_10ms();
		}
	}
	Serial_1_PutData(p,len);
}

void zb_update_ask(uint8_t *p,int len)
{
	Serial_1_PutData(p,len);
}

void zb_update_finished(uint8_t *p,int len)
{
	Serial_1_PutData(p,len);
}
void zll_parket_cmd(uint8_t *p,uint8_t len)
{
	uint8_t cmd=*(p+2);
	switch(cmd)
	{
		case ID_NET_HeartBeat:       zb_heartbeat_call(p,len);break;
		case ID_NET_HeartBeat_New:   zb_heartbeat_call_New(p,len);break;
			
		case ID_DATA_Pad:            zb_pad_call(p,len);break;                   //ID_DATA_Set_Scene_and_rsp
		case ID_DATA_Control_scene:  zb_pad_call_2(p,len);break;                //ID_DATA_Set_Scene_new
		case ID_DATA_PAD_magic:      zb_pad_call_3(p,len);break;                //ID_DATA_Set_Scene_MAGIC
		//cha xun
		case MSG_GET_STATUS_Answer:  get_status_back(p,len);break;
		//update
		case MSG_update_ask:		 zb_update_ask(p,len);break;
		case MSG_update_finish:		 zb_update_finished(p,len);break;
		default:break;
		
	}
	
}
