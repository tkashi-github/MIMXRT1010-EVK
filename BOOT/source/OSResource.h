/**
 * @file		OSResouce.h
 * @brief		RTOS resource set up
 * @author		Takashi Kashiwagi
 * @date		2019/11/24
 * @version		1.0.0
 * @copyright
 * --
 * License Type <MIT License>
 * --
 * Copyright 2019 Takashi Kashiwagi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 * 
 * @par Update:
 * - 2019/11/24: Takashi Kashiwagi: for MIMXRT1010-EVK
 */
#ifndef __cplusplus
#if __STDC_VERSION__ < 201112L
#error /* Only C11 */
#endif
#endif
#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

/** User Typedefine */
#include "UserTypedef.h"

/** Interrupt Priority */
#define kIRQ_PRIORITY_LPUART (10u)

/** Task Handle */
extern osThreadId_t g_InitialTaskHandle;
extern osThreadId_t g_ConsoleTaskHandle;

/** Event Group */
extern osEventFlagsId_t g_efLPUART[];

extern osSemaphoreId_t g_bsIdLPUARTRxSemaphore[];
extern osSemaphoreId_t g_bsIdLPUARTTxSemaphore[];

/** Queue */

/** Stream Buffer */
extern StreamBufferHandle_t g_sbhLPUARTTx[1 + enLPUART_MAX];
extern StreamBufferHandle_t g_sbhLPUARTRx[1 + enLPUART_MAX];

extern void CreateTask(void);
extern void CreateQueue(void);
extern void CreateBinarySemaphore(void);
extern void CreateEventGroup(void);
extern void CreateStreamBuffer(void);

#ifdef __cplusplus
}
#endif
