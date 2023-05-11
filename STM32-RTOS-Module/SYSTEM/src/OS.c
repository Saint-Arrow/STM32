#include "OS.h"
sTask SCH_tasks_G[SCH_MAX_TASKS];
uint16_t Error_code_G = 0;


uint8_t SCH_ADD_Task(void (*pFunction)(),uint16_t Delay,uint16_t Period)
{
	uint8_t Index = 0;
	while((SCH_tasks_G[Index].pTask != 0) && (Index <SCH_MAX_TASKS))
	{
		Index ++;
	}
	if(Index == SCH_MAX_TASKS )
	{
		Error_code_G = ERROR_SCH_MANY_TASKS;
		return SCH_MAX_TASKS;
	}
	if(pFunction == 0) 
	{
		Error_code_G = ERROR_SCH_Invalid_TASK;
		return SCH_MAX_TASKS;
	}
	SCH_tasks_G[Index].pTask = pFunction;
	SCH_tasks_G[Index].Delay=Delay;
	SCH_tasks_G[Index].Priod=Period;
	SCH_tasks_G[Index].RunMe=0;
	return Index;
}
uint8_t SCH_UPDATE_PERIOD_Tasks(uint16_t TASK_Index,uint16_t period)
{
	uint8_t RERUNR_CODE;
	if(TASK_Index >= SCH_MAX_TASKS)
	{
		RERUNR_CODE = ERROR_UPDATE_PERIOD_Over_MAX;
		return RERUNR_CODE;
	}
	if(SCH_tasks_G[TASK_Index].pTask == 0)
	{
		RERUNR_CODE = ERROR_UPDATE_PERIOD_EMPTY_TASK;
		return RERUNR_CODE;
	}
	else
	{
		RERUNR_CODE = 0;
	}

	SCH_tasks_G[TASK_Index].Priod=period;
	SCH_tasks_G[TASK_Index].Delay=period;
	return RERUNR_CODE;
	
}
uint8_t SCH_Delete_Tasks(uint16_t TASK_Index)
{
	uint8_t RERUNR_CODE;
	if(TASK_Index >= SCH_MAX_TASKS)
	{
		RERUNR_CODE = ERROR_Delete_Over_MAX;
		return RERUNR_CODE;
	}
	if(SCH_tasks_G[TASK_Index].pTask == 0)
	{
		RERUNR_CODE = ERROR_EMPTY_TASK;
		return RERUNR_CODE;
	}
	else
	{
		RERUNR_CODE = 0;
	}
	SCH_tasks_G[TASK_Index].pTask =0;
	SCH_tasks_G[TASK_Index].Delay=0;
	SCH_tasks_G[TASK_Index].Priod=0;
	SCH_tasks_G[TASK_Index].RunMe=0;
	return RERUNR_CODE;
	
}
void SCH_Report_Status(void)
{
	#ifdef SCH_REPORT_ERRORS
	static uint16_t count;
	count ++;
	if(count > 100000)
	{
		count=0;
		if(Error_code_G == ERROR_SCH_MANY_TASKS )
		{
			Serial_PutString("ERROR_SCH_MANY_TASKS\r\n");
			Error_code_G = 0;
		}
		if(Error_code_G == ERROR_SCH_Invalid_TASK )
		{
			Serial_PutString("ERROR_SCH_Invalid_TASK\r\n");
			Error_code_G = 0;
		}
		if(Error_code_G == ERROR_EMPTY_TASK )
		{
			Serial_PutString("ERROR_EMPTY_TASK\r\n");
			Error_code_G = 0;
		}
	}
	
	#endif
}
void SCH_Go_To_Sleep(void)
{
	
}

void SCH_Init(void)
{
	uint8_t Index;
	for(Index = 0; Index <SCH_MAX_TASKS;Index++)
	{
		SCH_tasks_G[Index].pTask =0;
	}
}
void SCH_Start(void)
{
	STM32_SysTickInit(1000);
}
void SCH_UPdate(void)
{
	uint8_t Index;
	for(Index = 0; Index <SCH_MAX_TASKS;Index++)
	{
		if(SCH_tasks_G[Index].pTask)
		{
			if(SCH_tasks_G[Index].Delay ==0)
			{				
				SCH_tasks_G[Index].RunMe = 1;
				if(SCH_tasks_G[Index].Priod)
				{
					SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Priod;
				}
			}
			else
			{
				SCH_tasks_G[Index].Delay --;
			}
		}
		
	}
}
void SCH_Dispatch_Tasks(void)
{
	uint8_t Index=0;
	for(Index =0;Index < SCH_MAX_TASKS;Index++)
	{
		if(SCH_tasks_G[Index].RunMe>0)
		{	
			SCH_tasks_G[Index].RunMe=0;
			if(SCH_tasks_G[Index].pTask)
				(SCH_tasks_G[Index].pTask)();			
			if(SCH_tasks_G[Index].Priod ==0)
			{
				SCH_tasks_G[Index].pTask=0;				
			}
			
		}
	}
	SCH_Report_Status();
	SCH_Go_To_Sleep();
}
