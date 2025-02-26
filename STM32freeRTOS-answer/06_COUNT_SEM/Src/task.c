/*
 * task.c
 *
 *  Created on: Dec 22, 2020
 *      Author: admin
 */

/* FreeRTOS.org includes. */
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "logicSniff.h"

/* task's priority */
#define TASK_MAIN_PRIO	20
#define TASK_1_PRIO		10
#define TASK_2_PRIO		 9
#define TASK_3_PRIO		 8
#define TASK_4_PRIO		 7

struct Param_types {	/* struct for parameter passing to task */
       char *msg;
       int  P1,P2;
} Param_Tbl;

/* The task functions. */
void TaskMain( void *pvParameters );
void Task1( void *pvParameters );
void Task2( struct Param_types *Param );

TaskHandle_t xHandleMain, xHandle1, xHandle2;
int	task1timer, task2timer;

SemaphoreHandle_t sem1;
char counter;
int buttoncounter;

#ifdef FREERTOS_MODULE_TEST
BaseType_t prvExampleTaskHook( void * pvParameter );
#endif

#define BUFSIZE 1024
static int copyTimes = 0;
static unsigned char b1[BUFSIZE];
static unsigned char b2[BUFSIZE];

/*-----------------------------------------------------------*/

void USER_THREADS( void )
{
	/* Setup the hardware for use with the Beagleboard. */
	//prvSetupHardware();
#ifdef CMSIS_OS
	osThreadDef(defaultTask, TaskMain, osPriorityNormal, 0, 256);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
#else
	/* Create one of the two tasks. */
	xTaskCreate(	(TaskFunction_t)TaskMain,		/* Pointer to the function that implements the task. */
					"TaskMain",	/* Text name for the task.  This is to facilitate debugging only. */
					256,		/* Stack depth - most small microcontrollers will use much less stack than this. */
					NULL,		/* We are not using the task parameter. */
					TASK_MAIN_PRIO,	/* This task will run at this priority */
					&xHandleMain );		/* We are not using the task handle. */
#endif
}
/*-----------------------------------------------------------*/

void TaskMain( void *pvParameters )
{
	const char *pcTaskName = "TaskMain";
	struct Param_types *Param;

	pvParameters = pvParameters; // for compiler warning

	/* Print out the name of this task. */
	printf( "%s is running\n", pcTaskName );

#ifdef FREERTOS_MODULE_TEST
	vTaskSetApplicationTaskTag( NULL, prvExampleTaskHook );
#endif

	/* TODO #1 #2:
		create a binary semaphore
	    use sem_id */
#if 1
// 	sem1=xSemaphoreCreateBinary();
 	sem1 = xSemaphoreCreateCounting( 10, 0 );
if (sem1 == NULL) printf("xSemaphoreCreateBinary error found\n");
#endif // TODO #1

	/* Create the other task in exactly the same way. */
	xTaskCreate(	(TaskFunction_t)Task1,		/* Pointer to the function that implements the task. */
					"Task1",	/* Text name for the task.  This is to facilitate debugging only. */
					1024,		/* Stack depth - most small microcontrollers will use much less stack than this. */
					NULL,		/* We are not using the task parameter. */
					TASK_1_PRIO,	/* This task will run at this priority */
					&xHandle1 );		/* We are not using the task handle. */

	/* Create the other task in exactly the same way. */
	Param = &Param_Tbl;		/* get parameter tbl addr */
	Param->P1 = 111111;		/* set parameter */
	Param->P2 = 222222;
	xTaskCreate( (TaskFunction_t)Task2, "Task2", 1024, (void*)Param, TASK_2_PRIO, &xHandle2 );

	/* delete self task */
	/* Print out the name of this task. */
	printf( "%s is deleted\r\n", pcTaskName );
	vTaskDelete (xHandleMain);	// vTaskDelete (NULL);
}
/*-----------------------------------------------------------*/

void Task1( void *pvParameters )
{
	const char *pcTaskName = "Task1";

	pvParameters = pvParameters; // for compiler warning

#ifdef FREERTOS_MODULE_TEST
	vTaskSetApplicationTaskTag( NULL, prvExampleTaskHook );
#endif

	/* Print out the name of this task. */
	printf( "%s is running\r\n", pcTaskName );

	while(1) {
		if(xSemaphoreTake( sem1, portMAX_DELAY ) == pdTRUE){
//			vTaskDelay(SW_DEBOUNCE_TIME);
			counter++;
		}
		printf("S%d(%d) ", (int)counter, (int)uxSemaphoreGetCount(sem1));
		task1timer++;
	}
}
/*-----------------------------------------------------------*/


void Task2( struct Param_types *Param )
{
	const char *pcTaskName = "Task2";

#ifdef FREERTOS_MODULE_TEST
	vTaskSetApplicationTaskTag( NULL, prvExampleTaskHook );
#endif

	/* Print out the name of this task. */
	printf( "%s is running\n", pcTaskName );

	printf("\n-------  Task2 parameter passed from main --------\n");
	printf("task2 first parameter = %d \n",Param->P1);
	printf("task2 second parameter = %d \n",Param->P2);
	printf("--------------------------------------------------\n");

	while(1) {
#if 1
//vTaskDelay (pdMS_TO_TICKS (1));
//printf("b"); fflush(stdout);	// 문자 'a' 출력
#endif

		task2timer++;
	}
}

// 시간이 많이 소요되는 특징으로써 정의한 함수
void heavyCopyLoader(void)
{
	int i;

	for(i=0; i<450; i++)
		memcpy(b2,b1,BUFSIZE);

	copyTimes++;
}

void vApplicationIdleHook (void)
{
#ifdef FREERTOS_MODULE_TEST
	vLogicSniffMultiSetLED5(LS_CHANNEL_ALL5, 0); // All Leds Clear
	vLogicSniffMultiSetLED6(LS_CHANNEL_ALL6, 0); // All Leds Clear
	vLogicSniffSetLED( LS_CHANNEL_0, 1 ); // LED 'ON'
#endif
}
/*-----------------------------------------------------------*/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
  static portBASE_TYPE xHigherPriorityTaskWoken;
  portBASE_TYPE xSemaStatus;
  UNUSED(xSemaStatus);

#ifdef FREERTOS_MODULE_TEST
//	vLogicSniffMultiSetLED5(LS_CHANNEL_ALL5, 0); // All Leds Clear
//	vLogicSniffMultiSetLED6(LS_CHANNEL_ALL6, 0); // All Leds Clear
	vLogicSniffSetLED( LS_CHANNEL_7, 1 ); // LED 'ON'
#endif

	//printf("o"); fflush(stdout);
	printf("C%d ", (int)buttoncounter);fflush(stdout);
	buttoncounter++;

/* counting semaphore example12 */
	xHigherPriorityTaskWoken = pdFALSE;

	// 시간이 많이 소요되는 함수를 호출
	heavyCopyLoader();

	// Semaphore 'Give Blocked Task Unblock'
	xSemaStatus = xSemaphoreGiveFromISR( sem1, &xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR(&xHigherPriorityTaskWoken);
}
