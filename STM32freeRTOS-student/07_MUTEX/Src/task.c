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
#include "semphr.h"

/* task's priority */
#define TASK_MAIN_PRIO	20
#define TASK_1_PRIO		10
#define TASK_2_PRIO		10
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

/* application specific declaration */
#define TICKET_1MILLION		1000000
#define TICKET_10000		10000
#define TICKET_1000			1000

extern void seedrand(int seed);
extern int randomnumber(void);
int buyTicket(void);

// 1초마다 콘솔에 동작중을 표시(.)
int USE_OSTimeTickHook;
TaskHandle_t xHandleMain, xHandle1, xHandle2;
int	task1timer, task2timer;

volatile int tickets= 0;		// MAX value( 1000 million )
volatile int backupTickets= 0;
volatile int T1_Ticket=0, T2_Ticket= 0;

SemaphoreHandle_t mutex_id;

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

	/* Print out the name of this task. */
	printf( "%s is running\n", pcTaskName );

	/* TODO #1:
		create a mutex
	    use mutex_id */
#if 0

if (mutex_id == NULL) printf("xSemaphoreCreateMutex error found\n");
#endif // TODO #1

	//
	// set random seed
	//
	seedrand( 45 );

	//
	// Set Tickets
	//
	/* TODO #2:
		초기 티켓수를 작은수(1,000 ~ 10,000정도) 입력 하여 테스트 한후
	    그 결과를 설명한다 */
#if 0 // No comment
	tickets= backupTickets = 1 * TICKET_10000; // MAX value( 1 million )
#endif // TODO #2

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
	printf( "%s is deleted\r\n", pcTaskName );
	vTaskDelete (xHandleMain);	// vTaskDelete (NULL);
}
/*-----------------------------------------------------------*/

void Task1( void *pvParameters )
{
	const char *pcTaskName = "Task1";

	/* Print out the name of this task. */
	printf( "%s is running\n", pcTaskName );

	while(1) {
		if(buyTicket() <= 0)
		{
			T1_Ticket++;
			/* delete other task */
			vTaskDelete(xHandle2);
			goto exit1;
		}
		T1_Ticket++;
		/* vTaskDelay() 시간 지연을 적당히 입력하여 티켓 카운트 수가
		오류가 나도록 만든다 */
		vTaskDelay(pdMS_TO_TICKS (1)); // 주의사항! 1 혹은 2를 입력 할 것
	}

exit1:
	// 1초마다 콘솔 출력 '중지'
	USE_OSTimeTickHook = 0;

	printf("\n[TASK1]Total Tickets = %d\n", T1_Ticket + T2_Ticket);
	printf("[TASK1]COUNTER up to %d\n", T1_Ticket);
	printf("[TASK2]COUNTER up to %d\n", T2_Ticket);

	if(backupTickets != T1_Ticket + T2_Ticket)
		printf("Not good. expectation(%d)\n", backupTickets);
	else
		printf("Good!. Matched\n");

	/* delete self task */
	vTaskDelete(NULL);
}
/*-----------------------------------------------------------*/


void Task2( struct Param_types *Param )
{
	const char *pcTaskName = "Task2";
	int i;
	unsigned int timeo;

	/* Print out the name of this task. */
	printf( "%s is running\n", pcTaskName );

	printf("\n-------  Task2 parameter passed from main --------\n");
	printf("task2 first parameter = %d \n",Param->P1);
	printf("task2 second parameter = %d \n",Param->P2);
	printf("--------------------------------------------------\n");

	// 1초마다 콘솔에 동작중을 표시(.)
	USE_OSTimeTickHook = 1;

	while(1) {
		for(i=0;i<1000;i++)
		{
			if(buyTicket() <= 0)
			{
				T2_Ticket++;
				/* delete other task */
				vTaskDelete(xHandle1);
				goto exit2;
			}
			T2_Ticket++;
		}
		timeo = randomnumber();
		vTaskDelay(pdMS_TO_TICKS (timeo % 10));
	}

exit2:
	// 1초마다 콘솔 출력 '중지'
	USE_OSTimeTickHook = 0;

	printf("\n[TASK2]Total Tickets = %d\n", T1_Ticket + T2_Ticket);
	printf("[TASK1]COUNTER up to %d\n", T1_Ticket);
	printf("[TASK2]COUNTER up to %d\n", T2_Ticket);

	if(backupTickets != T1_Ticket + T2_Ticket)
		printf("Not good. expectation(%d)\n", backupTickets);
	else
		printf("Good!. Matched\n");

	/* delete self task */
	vTaskDelete(NULL);
}
/*-----------------------------------------------------------*/

//
// buyTicket
//
int buyTicket(void)
{
	/* TODO #2:
		MUTEX 을 이용하여
		공유변수(tickets)를 보호한다 */
#if 1
	// CRITICAL SECTION(ENTER)

#endif // TODO #2

	tickets --;	// ticket count

#if 1
	// CRITICAL SECTION(EXIT)

#endif // TODO #2

	return(tickets);
}

void vApplicationTickHook( void )
{
	static TickType_t appTick= 0;
	appTick++;
	if( !(appTick % configTICK_RATE_HZ) )
	{
		if( USE_OSTimeTickHook ){
			printf("tickets=%d\n", tickets);
		}
	}
}

