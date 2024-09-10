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

//#include "partest.h"

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

void TaskMain( void *pvParameters )
{
	const char *pcTaskName = "TaskMain";
	struct Param_types *Param;

	pvParameters = pvParameters; // for compiler warning

	/* Print out the name of this task. */
	printf( "%s is running\r\n", pcTaskName );

#if 1
	/* Create the other task in exactly the same way. */
	xTaskCreate(	(TaskFunction_t)Task1,		/* Pointer to the function that implements the task. */
					"Task1",	/* Text name for the task.  This is to facilitate debugging only. */
					256,		/* Stack depth - most small microcontrollers will use much less stack than this. */
					NULL,		/* We are not using the task parameter. */
					TASK_1_PRIO,	/* This task will run at this priority */
					&xHandle1 );		/* We are not using the task handle. */
#endif
	/* Create the other task in exactly the same way. */
	Param = &Param_Tbl;		/* get parameter tbl addr */
	Param->P1 = 111111;		/* set parameter */
	Param->P2 = 222222;
	xTaskCreate( (TaskFunction_t)Task2, "Task2", 256, (void*)Param, TASK_2_PRIO, &xHandle2 );

	/* delete self task */
	/* Print out the name of this task. */
	printf( "%s is deleted\r\n", pcTaskName );
	vTaskDelete (xHandleMain);	// vTaskDelete (NULL);
}

void Task1( void *pvParameters )
{
	const char *pcTaskName = "Task1";

	pvParameters = pvParameters; // for comipler warning

	/* Print out the name of this task. */
	printf( "%s is running\r\n", pcTaskName );

	while(1) {

		vTaskDelay (pdMS_TO_TICKS (1000));
		printf("a"); fflush(stdout);	// 문자 'a' 출력

		task1timer++;
	}
}

void Task2( struct Param_types *Param )
{
	const char *pcTaskName = "Task2";
	/* TODO #1:
		Task2 에서 이미 사용 가능한 크기를 초과하는 큰 배열을 태스크 스택상에 선언한다
		char bbb[1024];
		*/
#if 0
	char bbb[500]; // 800bytes(fail)
	UNUSED(bbb);
#endif

	/* Print out the name of this task. */
	printf( "%s is running\n", pcTaskName );

	printf("\n-------  Task2 parameter passed from main --------\n");
	printf("task2 first parameter = %d \n",Param->P1);
	printf("task2 second parameter = %d \n",Param->P2);
	printf("--------------------------------------------------\n");

	while(1) {

		vTaskDelay (pdMS_TO_TICKS (1000));
		printf("b"); fflush(stdout);	// 문자 'b' 출력

	/* TODO #2:
		스택에 다음처럼 데이터를 넣어본다
		eg. memset(bbb, 'a', 500);
		*/
#if 0
		memset((void*)bbb, 'a', 500);
#endif

		task2timer++;
	}
}

/* TODO #3:*/
#if configCHECK_FOR_STACK_OVERFLOW
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	xTask = xTask; pcTaskName = pcTaskName;
	configASSERT(( volatile void * ) 0);
}
#endif

void vApplicationIdleHook (void)
{
	printf("."); fflush(stdout);
}

/*-----------------------------------------------------------*/


