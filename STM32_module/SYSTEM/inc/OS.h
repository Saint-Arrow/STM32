#ifndef OS
#define OS
#include"sys.h"
//#define SCH_REPORT_ERRORS
#define SCH_MAX_TASKS  (16)
#define ERROR_SCH_MANY_TASKS  1
#define ERROR_SCH_Invalid_TASK 2
#define ERROR_EMPTY_TASK 3
#define ERROR_Delete_Over_MAX 4
#define ERROR_UPDATE_PERIOD_Over_MAX 5
#define ERROR_UPDATE_PERIOD_EMPTY_TASK 6
typedef struct
{
	void (*pTask)(void);
	uint16_t Delay;
	uint16_t Priod;
	uint16_t RunMe;
}sTask;


extern sTask SCH_tasks_G[SCH_MAX_TASKS];
extern uint16_t Error_code_G;



uint8_t SCH_ADD_Task(void (*pFunction)(),uint16_t Delay,uint16_t Period);
uint8_t SCH_UPDATE_PERIOD_Tasks(uint16_t TASK_Index,uint16_t period);
uint8_t SCH_Delete_Tasks(uint16_t TASK_Index);
void SCH_Report_Status(void);
void SCH_Go_To_Sleep(void);
void SCH_Init(void);
void SCH_Start(void);
void SCH_UPdate(void);
void SCH_Dispatch_Tasks(void);
#endif
