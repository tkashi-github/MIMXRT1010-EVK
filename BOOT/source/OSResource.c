/**
 * @file		OSResouce.c
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
#include "OSResource.h"

/** Standard Header */
#include <stdint.h>
#include <stdbool.h>

/* Other Tasks */
#include "Task/InitialTask.h"
#include "Task/ConsoleTask/ConsoleTask.h"


/** typedef Task Table */
typedef struct{
	osThreadId_t   *pOsId;
	osThreadFunc_t TaskFunc;
	void           *argment;
	osThreadAttr_t Attr;
}stOSdefTable_t;

#if (defined(__GNUC__)) && (defined(__ARM_ARCH_7M__))
#define ALLOCATE_IN_DTCM __attribute__((section(".bss.$SRAM_DTC*"))) 
#else
#define ALLOCATE_IN_DTCM
#endif

/** Task Handle */
ALLOCATE_IN_DTCM alignas(32) osThreadId_t g_InitialTaskHandle;
ALLOCATE_IN_DTCM alignas(32) osThreadId_t g_ConsoleTaskHandle;

/** Task Control Block (STATIC ALLOCATION)*/
ALLOCATE_IN_DTCM alignas(32) static StaticTask_t s_InitialTaskTCB;
ALLOCATE_IN_DTCM alignas(32) static StaticTask_t s_ConsoleTaskTCB;

/** Task Stack (STATIC ALLOCATION)*/
ALLOCATE_IN_DTCM alignas(32) static uint32_t s_InitialTaskStack[2048/sizeof(uint32_t)];
ALLOCATE_IN_DTCM alignas(32) static uint32_t s_ConsoleTaskStack[8192/sizeof(uint32_t)];

/** Task Table */
static const stOSdefTable_t s_stTaskTable[] = {
	{
		&g_InitialTaskHandle,
		(osThreadFunc_t)InitialTask,
		NULL,
		{"InitialTask", osThreadDetached, &s_InitialTaskTCB, sizeof(s_InitialTaskTCB), s_InitialTaskStack, sizeof(s_InitialTaskStack), osPriorityLow, 0, 0},
	},
	{
		&g_ConsoleTaskHandle,
		(osThreadFunc_t)ConsoleTask,
		NULL,
		{"ConsoleTask", osThreadDetached, &s_ConsoleTaskTCB, sizeof(s_ConsoleTaskTCB), s_ConsoleTaskStack, sizeof(s_ConsoleTaskStack), osPriorityBelowNormal, 0, 0},
	},

	// Terminate
	{	
		NULL,
		NULL,
		NULL,
		{0},
	},
};


void CreateTask(void){
	uint32_t i=0;

	while(s_stTaskTable[i].pOsId != NULL){
		*s_stTaskTable[i].pOsId = osThreadNew(s_stTaskTable[i].TaskFunc, s_stTaskTable[i].argment, &s_stTaskTable[i].Attr);
		i++;
	}
}

/** CMSIS RTOS2 Event Flag Table */
typedef struct{
	osEventFlagsId_t *pefID;
	osEventFlagsAttr_t efAttr;
}stEventFlagTable_t;

/** osEventFlagsId_t */
DefALLOCATE_BSS_DTCM alignas(4) osEventFlagsId_t g_efLPUART[1+enLPUART_MAX];

/** StaticEventGroup_t */
DefALLOCATE_BSS_DTCM alignas(4) static StaticEventGroup_t s_xLPUARTEventGroupBuffer[1+enLPUART_MAX];


static stEventFlagTable_t s_stEventFlagTable[] = {
	{&g_efLPUART[enLPUART1], {"EF_LPUART", 0, &s_xLPUARTEventGroupBuffer[enLPUART1], sizeof(StaticEventGroup_t)}},
	{&g_efLPUART[enLPUART2], {"EF_LPUART", 0, &s_xLPUARTEventGroupBuffer[enLPUART2], sizeof(StaticEventGroup_t)}},

	{NULL, {NULL}},
};

void CreateEventGroup(void){	
	uint32_t i=0;

	while(s_stEventFlagTable[i].pefID != NULL){
		*s_stEventFlagTable[i].pefID = osEventFlagsNew(&s_stEventFlagTable[i].efAttr);
		i++;
	}
}


typedef struct{
	osSemaphoreId_t *pbsId;
	osSemaphoreAttr_t bsAttr;
	uint32_t u32MaxCount;
	uint32_t u32InitCount;
}stBinarySemaphoreTable_t;

DefALLOCATE_BSS_DTCM alignas(4) osSemaphoreId_t g_bsIdLPUARTRxSemaphore[1+enLPUART_MAX] = {NULL};
DefALLOCATE_BSS_DTCM alignas(4) osSemaphoreId_t g_bsIdLPUARTTxSemaphore[1+enLPUART_MAX] = {NULL};


DefALLOCATE_BSS_DTCM alignas(32) static StaticSemaphore_t s_xLPUARTRxSemaphoreBuffer[1+enLPUART_MAX];
DefALLOCATE_BSS_DTCM alignas(32) static StaticSemaphore_t s_xLPUARTTxSemaphoreBuffer[1+enLPUART_MAX];


const static stBinarySemaphoreTable_t s_stBinarySemaphoreTable[] = {
	{&g_bsIdLPUARTRxSemaphore[enLPUART1], {"BS_LPUART1RX", 0, &s_xLPUARTRxSemaphoreBuffer[enLPUART1], sizeof(StaticSemaphore_t)}, 1, 1},
	{&g_bsIdLPUARTRxSemaphore[enLPUART2], {"BS_LPUART2RX", 0, &s_xLPUARTRxSemaphoreBuffer[enLPUART2], sizeof(StaticSemaphore_t)}, 1, 1},

	{&g_bsIdLPUARTTxSemaphore[enLPUART1], {"BS_LPUART1TX", 0, &s_xLPUARTTxSemaphoreBuffer[enLPUART1], sizeof(StaticSemaphore_t)}, 1, 1},
	{&g_bsIdLPUARTTxSemaphore[enLPUART2], {"BS_LPUART2TX", 0, &s_xLPUARTTxSemaphoreBuffer[enLPUART2], sizeof(StaticSemaphore_t)}, 1, 1},

	{0},
};

void CreateBinarySemaphore(void){	/** CMSIS RTOS2にすること */
	uint32_t i=0;

	while(s_stBinarySemaphoreTable[i].pbsId != NULL){
		*s_stBinarySemaphoreTable[i].pbsId = osSemaphoreNew(s_stBinarySemaphoreTable[i].u32MaxCount, 
															s_stBinarySemaphoreTable[i].u32InitCount, 
															&s_stBinarySemaphoreTable[i].bsAttr);
		
		i++;
	}
}


typedef struct{
	osMessageQueueId_t *pID;
	uint32_t	u32BlockNum;
	uint32_t	u32SizeofBlock;
	osMessageQueueAttr_t stAttr;	/** 今のところ使い道がない */
}stQueueTable_t;

ALLOCATE_IN_DTCM alignas(32) osMessageQueueId_t g_StorageTaskQueueId = NULL;

static stQueueTable_t s_stQueueTable[] = {
	{&g_StorageTaskQueueId, 32, sizeof(stTaskMsgBlock_t), {0}},
	{NULL, 0, 0, {0}},
};

void CreateQueue(void){
	uint32_t i=0;

	while(s_stQueueTable[i].pID != NULL){
		*s_stQueueTable[i].pID = osMessageQueueNew(s_stQueueTable[i].u32BlockNum, s_stQueueTable[i].u32SizeofBlock, NULL);
		i++;
	}
}

typedef struct{
	StreamBufferHandle_t *pID;
	uint32_t             BufferSize;
	uint32_t             xTriggerLevel;
	uint8_t	             *pu8Buffer;
	StaticStreamBuffer_t *pxStreamBufferStruct;
}stStreamBuffer_t;

ALLOCATE_IN_DTCM alignas(32) StreamBufferHandle_t g_sbhLPUARTTx[1+enLPUART_MAX] = {NULL};
ALLOCATE_IN_DTCM alignas(32) static uint8_t s_u8StorageLPUARTTx[1+enLPUART_MAX][1024+1];	/** +1 はマニュアルの指示 */
ALLOCATE_IN_DTCM alignas(32) static StaticStreamBuffer_t s_ssbLPUARTTx[1+enLPUART_MAX];
ALLOCATE_IN_DTCM alignas(32) StreamBufferHandle_t g_sbhLPUARTRx[1+enLPUART_MAX] = {NULL};
ALLOCATE_IN_DTCM alignas(32) static uint8_t s_u8StorageLPUARTRx[1+enLPUART_MAX][1024+1];	/** +1 はマニュアルの指示 */
ALLOCATE_IN_DTCM alignas(32) static StaticStreamBuffer_t s_ssbLPUARTRx[1+enLPUART_MAX];

static stStreamBuffer_t s_stStreamBufferTable[] = {
	{&g_sbhLPUARTTx[enLPUART1], 1024+1, sizeof(TCHAR), s_u8StorageLPUARTTx[enLPUART1], &s_ssbLPUARTTx[enLPUART1]},
	{&g_sbhLPUARTTx[enLPUART2], 1024+1, sizeof(TCHAR), s_u8StorageLPUARTTx[enLPUART2], &s_ssbLPUARTTx[enLPUART2]},

	{&g_sbhLPUARTRx[enLPUART1], 1024+1, sizeof(TCHAR), s_u8StorageLPUARTRx[enLPUART1], &s_ssbLPUARTRx[enLPUART1]},
	{&g_sbhLPUARTRx[enLPUART2], 1024+1, sizeof(TCHAR), s_u8StorageLPUARTRx[enLPUART2], &s_ssbLPUARTRx[enLPUART2]},

	{NULL, 0, 0, NULL, NULL},
};
void CreateStreamBuffer(void){
	uint32_t i=0;

	while(s_stStreamBufferTable[i].pID != NULL){
		*s_stStreamBufferTable[i].pID = xStreamBufferCreateStatic(
			s_stStreamBufferTable[i].BufferSize, s_stStreamBufferTable[i].xTriggerLevel, 
			s_stStreamBufferTable[i].pu8Buffer, s_stStreamBufferTable[i].pxStreamBufferStruct);
		i++;
	}
}
