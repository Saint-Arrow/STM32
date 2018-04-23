#ifndef  _ZIGBEE_H_
#define	 _ZIGBEE_H_
#include "sys.h"

//网络信道
#define ZLL_FIRST_CHANNEL 		"0B"
#define ZLL_SECOND_CHANNEL		"0F"
#define ZLL_THIRD_CHANNEL 		"14"
#define ZLL_FOURTH_CHANNEL		"19"

//网络PID
#define ZLL_FIRST_PANID			"686E5180"
#define ZLL_SECOND_PANID		"686E5181"
#define ZLL_THIRD_PANID			"686E5182"
#define ZLL_FOURTH_PANID		"686E5183"
#define ZLL_FIFTH_PANID			"686E5184"
#define ZLL_SIXTH_PANID		    "686E5185"
#define ZLL_SEVENTH_PANID	    "686E5186"
#define ZLL_EIGHTH_PANID		"686E5187"
#define ZLL_NINTH_PANID			"686E5188"
#define ZLL_TENTH_PANID		    "686E5189"
#define ZLL_ELEVENTH_PANID	    "686E518A"
#define ZLL_TWELVETH_PANID		"686E518B"

#define ZLL_MAX_Dev         100
#define ZLL_MSG_HEAD 		0x7E
#define ZLL_MSG_END 		0x5E

#define Channel_time_count 15


struct ZIGBEE_MSG_HEAD
{
	uint8  header;
	uint8  length;
	uint16 Frame_control;
	uint8  Reserved1[6];
	uint8  Source_address[4];
	uint8  Reserved2[4];
	uint8  Target_address[4];
	uint8  Reserved3[6];
	uint16 Cluster_id;
	uint8  Reserved4[2];
	uint16 Write_Index;
	uint8 Write_Sub_Index;
	uint8 Write_Opt;
	uint8 Write_Data_Length;
};

enum
{
	ID_UnLock  			    =0,
	ID_NET_Config  			=1,
	ID_NET_Reset   			=2,	
	ID_NET_HeartBeat   		=3,
	ID_NET_HeartBeat_Answer	=4,
	ID_NET_Change           =5,
	ID_NET_HeartBeat_New   	=6,
	
	
	ID_DATA_Pad      			=10,
	ID_DATA_Pad_set_Rqt		    =11,
	ID_DATA_Set_Scene_Rsp		=12,
	ID_DATA_ReSet_Scene		    =13,
	ID_DATA_Control_scene       =14,//pad-gateway-new
	ID_DATA_Set_Control         =15,//gateway-pad
	ID_DATA_PAD_magic =16,
	ID_DATA_PAD_magic_rsp =17,
	
	MSG_SET_LAMPS				=20,
	MSG_SET_LIGHTNESS			=21,
	MSG_SET_COLORTEMP			=22,
	MSG_SET_RGB				    =23,
	MSG_LIGHT_STATUS			=24,
	
	MSG_SET_MOTOR				=25,
	
	MSG_GET_STATUS				=30,
	MSG_GET_STATUS_Answer		=31,

	MSG_SET_Answer				=35,

	MSG_update_start			=40,
	MSG_update_ask				=41,
	MSG_update_answer			=42,
	MSG_update_finish			=43,

	MSG_Lamp_Bind_Ctrl		     =50,
	MSG_Lamp_Bind_Ctrl_Rsp	     =51,
	MSG_Lamp_Delete_Ctrl		=52,
	MSG_Lamp_Delete_Ctrl_Rsp	=53,
	MSG_Lamp_Bind_info_Read     =54,
	MSG_Lamp_Bind_info_Read_Rsp =55,

	MSG_Scene_Sync				=60,
	MSG_Scene_Sync_Rsp		    =61,
	MSG_Scene_Read              =62,
	MSG_Scene_Read_Rsp          =63,
	
	MSG_Frame                   =70,
	MSG_Frame_Rsp               =71,

	
};


#pragma pack(1)
struct ZLL_UnLock
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char ROT_mac[4];
	unsigned char CCR1;
	unsigned char CCR2;
	unsigned char CCR3;
	unsigned char CCR4;
	unsigned char msg_end;
};
struct ZLL_CONFIG
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char net_id;
	unsigned char on_off;
	unsigned char msg_end;
};

struct ZLL_HeartBeat
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char ROT_mac[4];
	unsigned char type;//CCT-1  RGB-2  MOTOR-3
	unsigned char brightness;
	unsigned char colortemp;	
	unsigned char lightness;
	unsigned char valu_r;
	unsigned char valu_g;
	unsigned char valu_b;
	unsigned char msg_end;
};
struct ZLL_HeartBeat_New
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char ROT_mac[6];
	unsigned char type;//CCT-1  RGB-2  MOTOR-3
	unsigned char brightness;
	unsigned char colortemp;	
	unsigned char lightness;
	unsigned char valu_r;
	unsigned char valu_g;
	unsigned char valu_b;
	unsigned char msg_end;
};
struct ZLL_HeartBeat_Answer
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char ROT_mac[4];
	unsigned char msg_end;
};
struct ZLL_RESET_Answer
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_mac[4];
	unsigned char msg_end;
};
struct ZLL_Change_Net
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_mac[4];
	unsigned char net_id;
	unsigned char msg_end;
};
struct Lamp_Info
{
	unsigned char lamp_id[4];
	unsigned char valu_1;
	unsigned char valu_2;
	unsigned char valu_3;
	unsigned char valu_4;
};

struct Lamps_Info
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_num;
	struct Lamp_Info *info;
	unsigned char msg_end;
};

struct Lightness_Set
{
	unsigned char 	msg_head;
	unsigned char 	msg_len;
	unsigned char 	msg_id;
	unsigned char 	lamp_num;
	unsigned char 	lamp_id[4];		//长度可变，为4*lamp_num
	unsigned char 	lightness;		//0-100：色温亮度  101-201：RGB亮度
	unsigned char 	msg_end;
};

struct Colortemp_Set
{
	unsigned char 	msg_head;
	unsigned char 	msg_len;
	unsigned char 	msg_id;
	unsigned char 	lamp_num;
	unsigned char 	lamp_id[4];		//长度可变，为4*lamp_num
	unsigned char   colortemp;		//色温值 0-255
	unsigned char 	msg_end;
};

struct RGB_Set
{
	unsigned char 	msg_head;
	unsigned char 	msg_len;
	unsigned char 	msg_id;
	unsigned char 	lamp_num;
	unsigned char 	lamp_id[4];		//长度可变，为4*lamp_num
	unsigned char 	R;				//R:0-255
	unsigned char 	G;				//G:0-255
	unsigned char 	B;				//B:0-255
	unsigned char 	msg_end;
};

struct Motor_set
{
	unsigned char 	msg_head;
	unsigned char 	msg_len;
	unsigned char 	msg_id;
	unsigned char 	lamp_num;
	unsigned char 	lamp_id[4];		//长度可变，为4*lamp_num
	unsigned short 	motor_num;		// 0-停止   1-水平角度电机  2-垂直角度电机  3-调焦角度电机   4-垂直移动电机   5-水平移动电机
	unsigned char 	morot_angle;		//角度电机值，0-2000
	unsigned char 	msg_end;
};
struct Ctrl_led_updata
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char ctrl_id[4];
	unsigned char scene_id;
	unsigned char ctrl_msg_index;	
	unsigned char ctrl_msg_total;
	unsigned char lamp_num;
	struct Lamp_Info *info;	
	unsigned char msg_end;
};
struct Lamp_Set_Answer
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_id[4];
	unsigned char brightness;
	unsigned char colortemp;	
	unsigned char lightness;
	unsigned char valu_r;
	unsigned char valu_g;
	unsigned char valu_b;	
	unsigned char msg_end;
};

struct Get_Lamp_Status
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_id[4];
	unsigned char msg_end;
};

struct Get_Lamp_Status_Answer
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_id[4];
	unsigned char version[6];
  unsigned char type[4];
  unsigned char log[4];
	unsigned char brightness;
	unsigned char colortemp;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned short H_angle;		//水平角度值 0-2000
	unsigned short V_angle;		//垂直角度值
	unsigned short F_angle;		 //焦距角度值
	unsigned char  V_move_flag; //上下移动状态，1-最下位置 2-中 3-上
	unsigned char msg_end;
};

struct lamp_status
{
    unsigned char msg_head;
    unsigned char msg_len;
    unsigned char msg_id;
    unsigned char lamp_id[4];
    unsigned char version[8];
    unsigned char type;
    unsigned char motor_type;
    unsigned int time;
    unsigned char reloadnum;
    unsigned char brightness;
    unsigned char colortemp;
    unsigned char lightness;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned short H_angle;
    unsigned short V_angle;
    unsigned short F_angle;
    unsigned char  V_move_flag;
    unsigned char  msg_end;
};
struct update_start
{
    unsigned char head;
    unsigned char len;
    unsigned char msg_id;
    unsigned char mac[4];
    unsigned int  filelen;
    unsigned short total_packet;
    unsigned char  end;
};

struct update_packet_ask
{
    unsigned char head;
    unsigned char len;
    unsigned char msg_id;
    unsigned char mac[4];
    unsigned short packet_current;
    unsigned char  end;
};

struct update_packet_answer
{
    unsigned char head;
    unsigned char len;
    unsigned char msg_id;
    unsigned char mac[4];
    unsigned short packet_current;
    unsigned char  packet_len;
    unsigned short  crc;
    unsigned char  packet_buf[64];
    unsigned char  end;
};

struct update_finish
{
    unsigned char head;
    unsigned char len;
    unsigned char msg_id;
    unsigned char mac[4];
    unsigned char  end;
};

struct scene_sync
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_id[4];
	unsigned char lamp_value_1[12];
	unsigned char lamp_value_2[12];
	unsigned char lamp_value_3[12];
	unsigned char lamp_value_4[12];	
	unsigned char msg_end;
};
struct scene_sync_rsp
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_id[4];	
	unsigned char msg_end;
};
struct scene_req
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_id[4];
	unsigned char msg_end;
};
struct scene_rsp
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_id[4];
	unsigned char lamp_value_1[12];
	unsigned char lamp_value_2[12];
	unsigned char lamp_value_3[12];
	unsigned char lamp_value_4[12];	
	unsigned char msg_end;
};

struct MSG_Lamp_Bind_Ctrl
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_num;
	unsigned char lamp_id[4];
	unsigned char ctrl_id[4];
	unsigned char msg_end;
};
struct MSG_Lamp_Bind_Ctrl_rsp
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_id[4];
	unsigned char ctrl_id[4];
	unsigned char msg_end;
};
struct MSG_Lamp_Bind_del_and_rsp
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_id[4];
	unsigned char ctrl_id[4];
	unsigned char msg_end;
};

struct MSG_Lamp_Bind_read
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_id[4];
	unsigned char msg_end;
};
struct MSG_Lamp_Bind_read_rsp
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_id[4];
	unsigned char bind_num;//<20
	unsigned char bind_id[4];
	unsigned char msg_end;
};

struct ID_DATA_Set_Scene_and_rsp
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char ctrl_id[4];
	unsigned char key;
	unsigned char msg_end;
};
struct ID_DATA_Set_Scene_new
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char ctrl_id[4];
	unsigned char key;
	unsigned char key_ser;
	unsigned char msg_end;
};
struct ID_DATA_Set_Scene_MAGIC
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char ctrl_id[4];
	unsigned char key_cmd;
	unsigned char key_on_off;
	unsigned char key_dim_l;
	unsigned char key_dim_cct;
	unsigned char key_dim_r;
	unsigned char key_dim_g;
	unsigned char key_dim_b;
	unsigned char key_magic;
	unsigned char key_magic_speed;
	unsigned char key_ser;
	unsigned char msg_end;
};
struct ID_DATA_Set_Scene_MAGIC_rsp
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char ctrl_id[4];
	unsigned char key_cmd;
	unsigned char key_on_off;
	unsigned char key_dim_l;
	unsigned char key_dim_cct;
	unsigned char key_dim_r;
	unsigned char key_dim_g;
	unsigned char key_dim_b;
	unsigned char key_magic;
	unsigned char key_magic_speed;
	unsigned char key_ser;
	unsigned char msg_end;
};
struct scene_again
{
	unsigned char 	msg_head;
	unsigned char 	msg_len;
	unsigned char 	msg_id;
	unsigned char 	lamp_num;
	unsigned char 	lamp_id[5];	//5*N			
	unsigned char 	msg_end;	
};


struct FRAME_RQT
{
	unsigned char msg_head;
	unsigned char msg_len;
	unsigned char msg_id;
	unsigned char lamp_id[4];
	
	unsigned char frame_action;
	unsigned char frame_id;//save data from here
	unsigned short time_to_run_again;
	unsigned short time_to_finish;
	unsigned char  action;
	unsigned char  brightness;
	unsigned char  colortempature;
	unsigned char  lightness;
	unsigned char Red_value;
	unsigned char Green_value;
	unsigned char Blue_value;
	
	unsigned short motor_basic_1;
	unsigned short motor_basic_2;
	unsigned short motor_basic_3;
	unsigned char motor_move;
	
	unsigned char mode_id;//
	unsigned char 	msg_end;
};
#pragma pack()




#endif

  
  

