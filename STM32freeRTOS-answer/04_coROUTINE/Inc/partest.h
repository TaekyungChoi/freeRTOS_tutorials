/*
 * FreeRTOS V202012.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * 1 tab == 4 spaces!
 */

#ifndef PARTEST_H
#define PARTEST_H

#include <stm32f103xb.h>

#define partstDEFAULT_PORT_ADDRESS		( ( uint16_t ) 0x378 )

/* Open Bench Logic Sniff Test Probe */
#define PAD_BROWN 	GPIO_ODR_ODR0	/* GPIO_0 */
#define PAD_ORANGE 	GPIO_ODR_ODR1 	/* GPIO_1 */
#define PAD_YELLOW 	GPIO_ODR_ODR2 	/* GPIO_2 */
#define PAD_GREEN 	GPIO_ODR_ODR3 	/* GPIO_3 */
#define PAD_BLUE 	GPIO_ODR_ODR4 	/* GPIO_4 */
#define PAD_PURPLE 	GPIO_ODR_ODR5 	/* GPIO_5 */
#define PAD_GRAY 	GPIO_ODR_ODR6	/* GPIO_6 */
#define PAD_WHITE 	GPIO_ODR_ODR7 	/* GPIO_7 */

#define LS_CHANNEL_0	0	/* PIN17:GPIO_145 */
#define LS_CHANNEL_1	1	/* PIN7:GPIO_135 */
#define LS_CHANNEL_2	2	/* PIN6:GPIO_134 */
#define LS_CHANNEL_3	3	/* PIN4:GPIO_132 */
#define LS_CHANNEL_4	4	/* PIN3:GPIO_131 */
#define LS_CHANNEL_5	5	/* PIN2:GPIO_130 */
#define LS_CHANNEL_6	6	/* PIN183:GPIO_183 for IDLE TASK */
#define LS_CHANNEL_7	7	/* PIN168:GPIO_168 */
#define LS_CHANNEL_ALL5	(PAD_BROWN | \
							PAD_ORANGE | \
							PAD_YELLOW | \
							PAD_GREEN | \
							PAD_BLUE | \
							PAD_PURPLE )

#define LS_CHANNEL_ALL6	(PAD_GRAY | \
							PAD_WHITE )

void vParTestInitialise( void );
void vParTestSetLED( UBaseType_t uxLED, BaseType_t xValue );
void vParTestToggleLED( UBaseType_t uxLED );

#endif

