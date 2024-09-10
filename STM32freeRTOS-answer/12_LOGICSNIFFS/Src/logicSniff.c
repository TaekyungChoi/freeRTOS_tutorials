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
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "logicSniff.h"

int pads_logicsniffs[]={ PAD_ORANGE,
PAD_YELLOW,
PAD_GREEN,
PAD_BLUE,
PAD_PURPLE,
PAD_GRAY,
PAD_WHITE,
PAD_BLACK
};

void vLogicSniffMultiSetLED5( unsigned int channel, int xValue )
{
	if ( xValue ){
		  HAL_GPIO_WritePin(GPIOB, channel, GPIO_PIN_SET);
	}
	else{
		  HAL_GPIO_WritePin(GPIOB, channel, GPIO_PIN_RESET);
	}
}

void vLogicSniffMultiSetLED6( unsigned int channel, int xValue )
{
	if ( xValue ){
		  HAL_GPIO_WritePin(GPIOB, channel, GPIO_PIN_SET);
	}
	else{
		  HAL_GPIO_WritePin(GPIOB, channel, GPIO_PIN_RESET);
	}
}

void vLogicSniffSetLED( unsigned int channel, int xValue )
{
	unsigned int GPIO_PIN = 0;

	if( channel <= LS_CHANNEL_7 ){
		switch(channel){
			case LS_CHANNEL_7:
			case LS_CHANNEL_6:
				{
					GPIO_PIN=pads_logicsniffs[channel];
					if ( xValue ){
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN, GPIO_PIN_SET);
					}
					else{
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN, GPIO_PIN_RESET);
					}
					break;
				}
			case LS_CHANNEL_5:
			case LS_CHANNEL_4:
			case LS_CHANNEL_3:
			case LS_CHANNEL_2:
			case LS_CHANNEL_1:
			case LS_CHANNEL_0:
				{
					GPIO_PIN=pads_logicsniffs[channel];
					if ( xValue ){
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN, GPIO_PIN_SET);
					}
					else{
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN, GPIO_PIN_RESET);
					}
					break;
				}
			default:
				printf("vLogicSniffSetLED: Error Invalid Parameter\n");
				break;
		};
	}
}

/*-----------------------------------------------------------*/

void vLogicSniffToggleLED( unsigned int channel )
{
	volatile unsigned long ulCurrentState, GPIO_PIN=0;
	UNUSED(ulCurrentState);
	if( channel <= LS_CHANNEL_7 ){
		/* Toggle LED Status */
		switch(channel){
			case LS_CHANNEL_7:
			case LS_CHANNEL_6:
				{
					GPIO_PIN=pads_logicsniffs[channel];
					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN);
					break;
				}
			case LS_CHANNEL_5:
			case LS_CHANNEL_4:
			case LS_CHANNEL_3:
			case LS_CHANNEL_2:
			case LS_CHANNEL_1:
			case LS_CHANNEL_0:
				{
					GPIO_PIN=pads_logicsniffs[channel];
					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN);
					break;
				}
			default:
				{
					printf("vLogicSniffSetLED: Error Invalid Parameter\n");
					break;
				}
		};
	}
}
