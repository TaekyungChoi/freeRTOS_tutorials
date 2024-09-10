/*
 * FreeRTOS Kernel V10.1.1
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 2 spaces!
 *
 * Created by YUNG KI HONG on 2018-12-13
 * Copyright (c) 2015 guileschool.com All rights reserved.
 */

/*-----------------------------------------------------------
 * Simple parallel port IO routines.
 *-----------------------------------------------------------*/

/* FreeRTOS.org includes. */
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "partest.h"

#define partstMAX_OUTPUT_LED	( 1 )
#define partstFIRST_LED			LD2_Pin

#define partstNUM_LEDS			( 1 )
#define partstALL_OUTPUTS_OFF	( ( unsigned long ) ~(0xFFFFFFFF << partstNUM_LEDS) )

/*-----------------------------------------------------------*/

void vParTestInitialise( void )
{

}

/*-----------------------------------------------------------*/
void vParTestSetLED( unsigned portBASE_TYPE uxLED, signed portBASE_TYPE xValue )
{
unsigned short usBit;

	vTaskSuspendAll();
	{
		if( uxLED < partstMAX_OUTPUT_LED )
		{
			usBit = partstFIRST_LED << uxLED;

			if( xValue == pdFALSE )
			{
				HAL_GPIO_WritePin(LD2_GPIO_Port, usBit, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(LD2_GPIO_Port, usBit, GPIO_PIN_SET);
			}
		}
	}
	xTaskResumeAll();
}
/*-----------------------------------------------------------*/

void vParTestToggleLED( unsigned portBASE_TYPE uxLED )
{
unsigned short usBit;

	vTaskSuspendAll();
	{
		if( uxLED < partstMAX_OUTPUT_LED )
		{
			usBit = partstFIRST_LED << uxLED;

			HAL_GPIO_TogglePin(LD2_GPIO_Port, usBit);
		}
	}
	xTaskResumeAll();
}

