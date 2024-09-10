#include "logicSniff.h"
#include <stdio.h>

void vLogicSniffMultiSetLED5( unsigned int channel, int xValue );
void vLogicSniffMultiSetLED6( unsigned int channel, int xValue );
void vLogicSniffSetLED( unsigned int channel, int xValue );

BaseType_t prvExampleTaskHook( void * pvParameter )
{
/* Perform an action. This could be anything. In this example, the hook is used to output debug trace information. pxCurrentTCB is the handle of the currently executing task. (vWriteTrace() is not an API function. It's just used as an example.) */
	vLogicSniffMultiSetLED5(LS_CHANNEL_ALL5, 0); // All Leds Clear
	vLogicSniffMultiSetLED6(LS_CHANNEL_ALL6, 0); // All Leds Clear
	vLogicSniffSetLED( pxCurrentTCB->uxTCBNumber, 1 );
	printf("%d ", (int)pxCurrentTCB->uxTCBNumber);
/* This example does not make use of the hook return value so just returns 0 in every case. */
	return 0;
}
