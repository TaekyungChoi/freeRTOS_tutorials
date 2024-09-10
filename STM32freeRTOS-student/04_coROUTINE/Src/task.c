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
#include "croutine.h"
#include "partest.h"

void vFlashCoRoutine (CoRoutineHandle_t xHandle,
                 UBaseType_t uxIndex);

#define PRIORITY_0 0

/*-----------------------------------------------------------*/

void USER_THREADS( void )
{

vParTestSetLED( 0, 0 ); // LD2 'OFF'

	printf("vFlashCoRoutine\n");
	//vStartLEDFlashTasks( mainLED_TASK_PRIORITY );
	xCoRoutineCreate (vFlashCoRoutine, PRIORITY_0, 0);

  /* TODO #2:
    두번째 코루틴 함수 생성 */
#if 1

#endif // TODO #2

vParTestSetLED( 0, 1 ); // LD2 'ON'

	/* Should never reach here! */
	return;
}


void vApplicationIdleHook (void)
{
  /* TODO #1:
    코루틴 스케줄러 실행
    vCoRoutineSchedule */
#if 0
   vCoRoutineSchedule ();
#endif // TODO #1
}


void vFlashCoRoutine (CoRoutineHandle_t xHandle,
                 UBaseType_t uxIndex)
{
   // 공동 루틴은 crSTART () 호출로 시작해야합니다.
   crSTART (xHandle);

   for (;;)
   {
      // 고정 된 기간 동안 지연.
      crDELAY (xHandle, 100);
      if (uxIndex)
    	  printf("uxIndex is %d\n", (int)uxIndex);
      else
      // LED를 플래시합니다.
    	  vParTestToggleLED (0);
   }

   // 공동 루틴은 crEND ()에 대한 호출로 끝나야합니다.
   crEND ();
}
/*-----------------------------------------------------------*/
