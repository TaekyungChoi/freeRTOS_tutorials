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
#include "timers.h"

/* The periods assigned to the one-shot and auto-reload timers are 3.333 second and half a second respectively. */
#define mainONE_SHOT_TIMER_PERIOD		pdMS_TO_TICKS( 100 )
#define mainAUTO_RELOAD_TIMER_PERIOD	pdMS_TO_TICKS( 50 )

static void prvOneShotTimerCallback( TimerHandle_t xTimer );
static void prvAutoReloadTimerCallback( TimerHandle_t xTimer );

static int ulCallCount;

/*-----------------------------------------------------------*/

/*
 * Starts all the other tasks, then starts the scheduler.
 */
void USER_THREADS( void )
{
	TimerHandle_t xAutoReloadTimer, xOneShotTimer;
	BaseType_t xTimer1Started, xTimer2Started;

	/* TODO #1:
		원샷 소프트웨어 타이머(xOneShotTimer)의 구현 */
#if 0
	/* Create the one shot timer, storing the handle to the created timer in xOneShotTimer. */
	xOneShotTimer = xTimerCreate(
					/* Text name for the software timer - not used by FreeRTOS. */
					"OneShot",
					/* The software timer's period in ticks. */
					mainONE_SHOT_TIMER_PERIOD,
					/* Setting uxAutoRealod to pdFALSE creates a one-shot software timer. */
					pdFALSE,
					/* This example does not use the timer id. */
					0,
					/* The callback function to be used by the software timer being created. */
					prvOneShotTimerCallback );
#endif

	/* TODO #2:
		자동 반복 소프트웨어 타이머(xAutoReloadTimer)의 구현 */
#if 0

#endif
	/* Check the software timers were created. */
	if( ( xOneShotTimer != NULL ) || ( xAutoReloadTimer != NULL ) )
	{
	/* Start the software timers, using a block time of 0 (no block time).
		The scheduler has not been started yet so any block time specified here would be ignored anyway. */
		xTimer1Started = xTimerStart( xOneShotTimer, 0 );
		xTimer2Started = xTimerStart( xAutoReloadTimer, 0 );

	/* The implementation of xTimerStart() uses the timer command queue, and xTimerStart()
		will fail if the timer command queue gets full.
		The timer service task does not get created until the scheduler is started, so all commands sent to the command queue will
		stay in the queue until after the scheduler has been started.
		Check both calls to xTimerStart() passed. */

		if( ( xTimer1Started != pdPASS ) || ( xTimer2Started != pdPASS ) )
		{
			printf("[WARNING!]timer is not working\n");
		}
	}
}

static void prvOneShotTimerCallback( TimerHandle_t xTimer )
{
	TickType_t xTimeNow;

	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();

	/* Output a string to show the time at which the callback was executed. */
	printf( "One-shot timer callback executing %d\n", (int)xTimeNow );

	/* File scope variable. */
	ulCallCount++;
}

static void prvAutoReloadTimerCallback( TimerHandle_t xTimer )
{
	TickType_t xTimeNow;

	/* Obtain the current tick count. */

	xTimeNow = xTaskGetTickCount();

	/* Output a string to show the time at which the callback was executed. */

	printf( "Auto-reload timer callback executing %d\n", (int)xTimeNow );

	ulCallCount++;
}

void vApplicationIdleHook (void)
{
}
