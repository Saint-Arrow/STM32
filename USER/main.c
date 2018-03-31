/* Version :V1.0-----------------------------------------------------------------------*/
/* Includes ---------------------------------------------------------------------------*/
#include  "main.h"

#define START_TASK_PRIO     1
#define START_STK_SIZE      128
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);



#define LED_TASK_PRIO     2
#define LED_STK_SIZE      128
TaskHandle_t ledTask_Handler;
void led_task(void *pvParameters);

int main(void)
{
	GPIO_Conf();
	xTaskCreate((TaskFunction_t   )start_task,
	            (const char*      )"start_task",
	            (uint16_t         )START_STK_SIZE,
	            (void*            )NULL,
	            (UBaseType_t      )START_TASK_PRIO,
	            (TaskHandle_t*    )&StartTask_Handler
	           );
	vTaskStartScheduler();
}


void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();
	xTaskCreate((TaskFunction_t   )led_task,
	            (const char*      )"start_task",
	            (uint16_t         )LED_STK_SIZE,
	            (void*            )NULL,
	            (UBaseType_t      )LED_TASK_PRIO,
	            (TaskHandle_t*    )&ledTask_Handler
	           );
	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL();
}

void led_task(void *pvParameters)
{
	while(1)
	{
		LED_PB2_TOG();
		vTaskDelay(500);
	}
}
