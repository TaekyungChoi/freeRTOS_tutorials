/*
 * task.c
 *
 *  Created on: Dec 22, 2020
 *      Author: YUNGKI HONG (guileschool@gmail.com)
 *      Copyright © 2015 guileschool
 */

/* FreeRTOS.org includes. */
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

/* task's priority */
#define TASK_MAIN_PRIO	20
#define TASK_1_PRIO		10
#define TASK_2_PRIO		  9
#define TASK_3_PRIO		  8
#define TASK_4_PRIO		  7

#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */

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

/* ...........................................................................
 *
 * 이벤트 컨트롤 & 사용자 정의 블럭 정의
 * ===================
 */
#define ENGINE_OIL_PRES_OK 0x01
#define ENGINE_OIL_TEMP_OK 0x02
#define ENGINE_START 0x04

EventGroupHandle_t group_id;


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
	EventBits_t uxBits;

	/* TODO #1:
		create a event flag
	    use group_id */
#if 0
group_id= ?????;
if (group_id == NULL) printf("xEventGroupCreate error found\n");
#endif // TODO #1

	/* Create the other task in exactly the same way. */
	xTaskCreate(	(TaskFunction_t)Task1,		/* Pointer to the function that implements the task. */
					"Task1",	/* Text name for the task.  This is to facilitate debugging only. */
					256,		/* Stack depth - most small microcontrollers will use much less stack than this. */
					NULL,		/* We are not using the task parameter. */
					TASK_1_PRIO,	/* This task will run at this priority */
					&xHandle1 );		/* We are not using the task handle. */

	for(;;) {
	/* TODO #2:
		get flag 'ENGINE_OIL_PRES_OK' OR 'ENGINE_OIL_TEMP_OK' from TASK2 */
#if 0
uxBits = ????;
UNUSED(uxBits);
#endif // TODO #2

		printf("."); fflush(stdout);
	}

	// 이벤트 플래그 삭제
	vEventGroupDelete(group_id);

	/* delete self task */
	/* Print out the name of this task. */
	printf( "%s is deleted\n", pcTaskName );
	vTaskDelete (xHandleMain);	// vTaskDelete (NULL);
}
/*-----------------------------------------------------------*/

void Task1( void *pvParameters )
{
	const char *pcTaskName = "Task1";
	EventBits_t uxBits;

	/* Print out the name of this task. */
	printf( "%s is running\n", pcTaskName );

	for(;;) {
	/* TODO #2:
		post flag 'ENGINE_OIL_PRES_OK' to TASKMain */
#if 0
uxBits = ????;
UNUSED(uxBits);
#endif // TODO #2
	vTaskDelay (pdMS_TO_TICKS (1000));
	}
}
