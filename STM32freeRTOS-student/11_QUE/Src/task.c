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
#include "queue.h"

/* task's priority */
#define TASK_MAIN_PRIO	20
#define TASK_1_PRIO		10
#define TASK_2_PRIO		  9
#define TASK_3_PRIO		  8
#define TASK_4_PRIO		  7

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
 * 메시지큐 & 사용자 정의 블럭 정의
 * ===================
 */
QueueHandle_t qid;

typedef struct tag_qBuffer {
	char ucMessageID;
	char ucData[10]; // bugfix. 배열의 크기에 따른 스택오버플로우 발생 위험(2021/9/5)
}qBuffer;

#define QUEUE_LENGTH	5
#define QUEUE_ITEM_SIZE sizeof(qBuffer)
#define TASK_ID_M	45229
#define TASK_ID_1	45230
#define TASK_ID_2	45231

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

	/* TODO #1:
		create a Queue
	    use qid */
#if 0
qid = ????;
if (qid == NULL) printf("xQueueCreate error found\n");
#endif // TODO #1

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

	/* Print out the name of this task. */
	printf( "%s is running\r\n", pcTaskName );

	/* delete self task */
	/* Print out the name of this task. */
	printf( "%s is deleted\r\n", pcTaskName );
	vTaskDelete (xHandleMain);	// vTaskDelete (NULL);
}
/*-----------------------------------------------------------*/

void Task1( void *pvParameters )
{
	const char *pcTaskName = "Task1";
	qBuffer xMessage;
	BaseType_t p;

	/* Print out the name of this task. */
	printf( "%s is running\n", pcTaskName );

	for(;;) {
		// 메시지 큐 대기
		p =xQueueReceive( qid, &xMessage, portMAX_DELAY ) ;/* Wake up By Queue Broadcast */
		if (p != pdPASS) printf("xQueueReceive error found\n");
		printf("get message \"%d\" from TASK qpost\n",xMessage.ucData[0]);
	}
}
/*-----------------------------------------------------------*/


void Task2( struct Param_types *Param )
{
	const char *pcTaskName = "Task2";
	qBuffer xMessage;
	BaseType_t p;
	int i;

	/* Print out the name of this task. */
	printf( "%s is running\n", pcTaskName );

	printf("\n-------  Task2 parameter passed from main --------\n");
	printf("task2 first parameter = %d \n",Param->P1);
	printf("task2 second parameter = %d \n",Param->P2);
	printf("--------------------------------------------------\n");

	printf("[qpost]waiting...\n");

	xMessage.ucMessageID = TASK_ID_1;

	for(i=1; i< 10000; i++) {
		xMessage.ucData[0] = i; xMessage.ucData[1]= NULL;

	/* TODO #2:
		post a message to TASK qpend
	    use msg[] */
#if 0
p=?????;
#endif // TODO #2
		if (p != pdPASS) printf("xQueueSendToBack error found\n");
		vTaskDelay (pdMS_TO_TICKS (1000));
	}
}
