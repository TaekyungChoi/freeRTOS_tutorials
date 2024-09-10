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
#include "task.h"
#include "semphr.h"
#include "partest.h"

/* task's priority */
#define TASK_MAIN_PRIO	20
#define TASK_1_PRIO			10

/* The task functions. */
void TaskMain( void *pvParameters );
void Task1( void *pvParameters );

// 1초마다 콘솔에 동작중을 표시(.)
int USE_OSTimeTickHook;

TaskHandle_t xHandleMain, xHandle1;
int	task1timer;

SemaphoreHandle_t sem_id;

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
	int loops;
	UBaseType_t sem_val;	// semaphore control

	pvParameters = pvParameters; // for compiler warning

	/* Print out the name of this task. */
	printf( "%s is running\r\n", pcTaskName );

	USE_OSTimeTickHook = 0;

	/* TODO #1:
		create a binary semaphore
	    use sem_id */
#if 1

	if (sem_id == NULL) printf("xSemaphoreCreateBinary error found\n");
#endif // TODO #1

	/* Create the other task in exactly the same way. */
	xTaskCreate(	(TaskFunction_t)Task1,		/* Pointer to the function that implements the task. */
					"Task1",	/* Text name for the task.  This is to facilitate debugging only. */
					256,		/* Stack depth - most small microcontrollers will use much less stack than this. */
					NULL,		/* We are not using the task parameter. */
					TASK_1_PRIO,	/* This task will run at this priority */
					&xHandle1 );		/* We are not using the task handle. */

	// loop counter
	loops = 10;
    for(;;) {
		// 세마포어 대기
		if (xSemaphoreTake(sem_id, portMAX_DELAY) == pdTRUE)
		{
			printf("."); fflush(stdout);
		}
		sem_val=uxSemaphoreGetCount(sem_id);
    	//printf("sem_val is %d\n", (int)sem_val);

		if(loops == 0)
			break; // exit

		loops--;
	}

	/* TODO #3:
		delete a semaphore */
#if 1

#endif // TODO #3

	/* delete self task */
	/* Print out the name of this task. */
	printf( "%s is deleted\r\n", pcTaskName );

	vTaskDelete (xHandleMain);	// vTaskDelete (NULL);
}
/*-----------------------------------------------------------*/

void Task1( void *pvParameters )
{
	const char *pcTaskName = "Task1";
	BaseType_t err;

	pvParameters = pvParameters; // for comipler warning

	/* Print out the name of this task. */
	printf( "%s is running\r\n", pcTaskName );

	for(;;) {
	/* TODO #2:
		post to a semaphore */
#if 1

#endif // TODO #2

	if (err != pdPASS) printf("xSemaphoreGive error(%d) found\n", (int)err);
	vTaskDelay (pdMS_TO_TICKS (1000));
 	}
}
/*-----------------------------------------------------------*/
