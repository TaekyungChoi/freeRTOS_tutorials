/*
 * task.c
 *
 *  Created on: Dec 22, 2020
 *      Author: YUNGKI HONG (guileschool@gmail.com)
 *      Copyright © 2015 guileschool
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

char counter;
static int buttoncounter;

#ifdef FREERTOS_MODULE_TEST
BaseType_t prvExampleTaskHook( void * pvParameter );
#endif

#define BUFSIZE 1024
SemaphoreHandle_t mutex_id;
static int copyTimes = 0;
static unsigned char b1[BUFSIZE];
static unsigned char b2[BUFSIZE];

void heavyCopyLoader(void);

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

	/* create a mute, use mutex_id */
	mutex_id=xSemaphoreCreateMutex();
	if (mutex_id == NULL) printf("xSemaphoreCreateMutex error found\n");

	/* Create the other task in exactly the same way. */
	xTaskCreate(	(TaskFunction_t)Task1,		/* Pointer to the function that implements the task. */
					"Task1",	/* Text name for the task.  This is to facilitate debugging only. */
					256,		/* Stack depth - most small microcontrollers will use much less stack than this. */
					NULL,		/* We are not using the task parameter. */
					TASK_1_PRIO,	/* This task will run at this priority */
					&xHandle1 );		/* We are not using the task handle. */

	/* Create the other task in exactly the same way. */
	Param = &Param_Tbl;		/* get parameter tbl addr */
	Param->P1 = 111111;		/* set parameter */
	Param->P2 = 222222;
	xTaskCreate( (TaskFunction_t)Task2, "Task2", 256, (void*)Param, TASK_2_PRIO, &xHandle2 );

	/* delete self task */
	/* Print out the name of this task. */
	printf( "%s is deleted\n", pcTaskName );
	vTaskDelete (xHandleMain);	// vTaskDelete (NULL);
}
/*-----------------------------------------------------------*/

void Task1( void *pvParameters )
{
	const char *pcTaskName = "Task1";
	uint32_t ulNotifiedValue;

	pvParameters = pvParameters; // for compiler warning

#ifdef FREERTOS_MODULE_TEST
	vTaskSetApplicationTaskTag( NULL, prvExampleTaskHook );
#endif

	/* Print out the name of this task. */
	printf( "%s is running\n", pcTaskName );

	while(1) {
#if __MY_ENABLE_DEFERRED_INTERRUPT == 1

		/* 인터럽트 처리기로부터 이벤트를 기다립니다 */
		ulNotifiedValue = ulTaskNotifyTake(pdTRUE,
		                                    portMAX_DELAY); // portMAX_DELAY: 영원히 기다림
		printf("N%d ", (int)ulNotifiedValue); fflush(stdout);

		if (ulNotifiedValue > 0)
		{
			/* ulNotifiedValue는 미해결 된 숫자의 수를 저장합니다.
			인터럽트. 차례대로 처리하십시오 */
			while (ulNotifiedValue> 0)
			{
				heavyCopyLoader(); // 시간이 많이 소요되는 함수를 호출

				ulNotifiedValue--;
			}
		}
#else
		vTaskDelay (pdMS_TO_TICKS (1));
#endif // __MY_ENABLE_DEFERRED_INTERRUPT

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

  /* TODO #2:
    터미널 화면에 현재 시간 정보(Tick변수값)을 출력 */
#if 0 // No comment
		printf("%06d ", xTaskGetTickCount()); // 현재의 시간(xTickCount) 출력
		fflush(stdout);
#endif // TODO #2

		task2timer++;
	}
}

// 시간이 많이 소요되는 특징으로써 정의한 함수
void heavyCopyLoader(void)
{
	int i;

#if __MY_ENABLE_DEFERRED_INTERRUPT == 1
	// CRITICAL SECTION(ENTER)
xSemaphoreTake(mutex_id, portMAX_DELAY);
#endif // __MY_ENABLE_DEFERRED_INTERRUPT

	// 수백 메가(1.45MB) 바이트의 데이터를 처리
	for(i=0; i<1450; i++)
	{
		//memcpy(b2,b1,BUFSIZE);
		memset(b2, copyTimes, BUFSIZE);
	}

	copyTimes++;  // 누적 처리 수
	printf("C%02d ", copyTimes); fflush(stdout);

#if __MY_ENABLE_DEFERRED_INTERRUPT == 1
	// CRITICAL SECTION(EXIT)
xSemaphoreGive(mutex_id);
#endif // __MY_ENABLE_DEFERRED_INTERRUPT

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
	static portBASE_TYPE xHigherPriorityTaskWoken;

#ifdef FREERTOS_MODULE_TEST
//	vLogicSniffMultiSetLED5(LS_CHANNEL_ALL5, 0); // All Leds Clear
//	vLogicSniffMultiSetLED6(LS_CHANNEL_ALL6, 0); // All Leds Clear
	vLogicSniffSetLED( LS_CHANNEL_7, 1 ); // LED 'ON'
#endif

	//printf("o"); fflush(stdout);
	buttoncounter++;

/* xHigherPriorityTaskWoken은 pdFALSE로 초기화되어야한다 */
	xHigherPriorityTaskWoken = pdFALSE;

#if __MY_ENABLE_DEFERRED_INTERRUPT == 0
	// 시간이 많이 소요되는 함수를 호출
	heavyCopyLoader();
#endif //__MY_ENABLE_DEFERRED_INTERRUPT

	// Semaphore를 'Give' 하여 Blocked Task를 Unblock 상태가 되게 한다.
	vTaskNotifyGiveFromISR(xHandle1, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(&xHigherPriorityTaskWoken);
}
