#ifndef GW
#define GW
#include "sys.h"
#include "OLED.h"
#include "REX_Zigbee.h"
#define VERSION 3
struct GW_FLASH
{
	uint8_t check_str[5];
	uint8_t version;
	uint8_t net_id;
	
};
extern struct GW_FLASH gw_confige;
extern uint8_t init_ok_flag;
extern uint8_t one_mode_flag;
extern uint8_t dev_list[6+1][ZLL_MAX_Dev];
extern uint8_t dev_active[6+1];
uint8_t mac_4B_tostr(uint8_t *mac,uint8_t *p);

void list_add(uint8_t *mac,uint8_t type,uint8_t *mac_buf);
void list_delete(uint8_t *mac,uint8_t *mac_buf);
uint8_t list_read_num(uint8_t *mac_buf);


void SHUNZHOU_reset(void);
uint16_t SHUNZHOU_CFG(void);
uint16_t check_profile(void);
void setpid(uint8_t netid);
void setch(uint8_t netid);

extern void zll_parket_cmd(uint8_t *p,uint8_t len);
#endif
