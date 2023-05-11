#ifndef _MOD_h
#define _MOD_h

/* Version :Vx.x-----------------------------------------------------------------------*/
/* Includes ---------------------------------------------------------------------------*/
/* Private/Exported typedef -----------------------------------------------------------*/
/* Private/Exported define ------------------------------------------------------------*/
/* Private/Exported macro -------------------------------------------------------------*/
/* Private/Exported variables ---------------------------------------------------------*/
/* Private/Exported function prototypes -----------------------------------------------*/
/* Private/Exported functions ---------------------------------------------------------*/



/* Version :V1.0-----------------------------------------------------------------------*/
/* Includes ---------------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "sys.h"
#include "mod_type.h"
#include "INLcpMsg.h"
#include "Dim_Ctrl.h"
#include "motor_task.h"
/* Private/Exported typedef -----------------------------------------------------------*/
/* Private/Exported define ------------------------------------------------------------*/


/* Private/Exported macro -------------------------------------------------------------*/
/* Private/Exported variables ---------------------------------------------------------*/
extern uint8_t const LED_Light[101];
extern uint32_t TimeIncrease;
extern uint32_t usart_time;
extern uint32_t timeout;
extern uint16_t USART_RX_STA; 
extern uint8_t USART_RX_BUF[USART_REC_LEN],RX_MCU[USART_REC_LEN];
/* Private/Exported function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------------- */
void show_reset_reason(void);
void Reset_H_ESP8266(void);
void ReLoad_H_ESP8266(void);
void Release_Reload(void);
void turn_on_led(void);
void turn_off_led(void);
void turn_led(void);
uint8_t Read_IR(void);
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);

void Check_Osc(void);
void Clean_Array(uint8_t a[],uint16_t len);
uint32_t cal_timeout(uint32_t t_sys,uint32_t t_usart);




uint8_t set_light(uint16_t light);
uint16_t get_light(void);
uint8_t set_CCT(uint16_t CCT);
uint16_t get_CCT(void);
uint8_t set_R(uint16_t DR);
uint8_t get_R(void);
uint8_t set_G(uint16_t DG);
uint8_t get_G(void);
uint8_t set_B(uint16_t DB);
uint8_t get_B(void);
int8_t set_basic_1(uint16_t angle);
int8_t set_basic_2(uint16_t angle);
int8_t set_basic_3(uint16_t angle);
int8_t turn_motor_off(uint8_t num);
int8_t Move_V_Up(void);
int8_t Move_V_Down(void);
int8_t Move_V_Stop(void);

int8_t Move_H_Left(void);
int8_t Move_H_Right(void);
int8_t Move_H_Stop(void);
uint32_t Adc_Light(void);
int8_t Judge_Imax(void);
int8_t JudgeByAdc(void);
int8_t Motor_power_on(void);
int8_t Motor_power_down(void);

void Dim_light(uint8_t flag);
void Dim_cct(uint8_t flag);
void Dim_rgb(uint8_t flag);
#endif

