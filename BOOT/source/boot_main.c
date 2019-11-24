/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "UserTypedef.h"
#include "OSResource.h"

#include "UART/DrvLPUART.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif
extern void CMSIS_OS2_SysTick_Handler (void);
extern void vPortSVCHandler( void );
extern void xPortPendSVHandler( void );
extern void prvForceHeapInit(void);
#ifdef __cplusplus
}
#endif

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

	SysTick_Config(SystemCoreClock / 1000u);
	InstallIRQHandler(SysTick_IRQn, (uint32_t)CMSIS_OS2_SysTick_Handler);
	//EnableIRQ(SysTick_IRQn);
	//NVIC_SetPriority(SVCall_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	InstallIRQHandler(SVCall_IRQn, (uint32_t)vPortSVCHandler);
	//EnableIRQ(SVCall_IRQn);
	//NVIC_SetPriority(PendSV_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
	InstallIRQHandler(PendSV_IRQn, (uint32_t)xPortPendSVHandler);
	//EnableIRQ(PendSV_IRQn);

	BOARD_InitBootPeripherals();

	
	while(osOK != osKernelInitialize());

	CreateTask();
	CreateQueue();
	CreateBinarySemaphore();
	CreateEventGroup();
	CreateStreamBuffer();

	{
		lpuart_config_t lpuart_config;
		LPUART_GetDefaultConfig(&lpuart_config);
		lpuart_config.baudRate_Bps = 115200;
		DrvLPUARTInit(enLPUART1, &lpuart_config);
	}
	osKernelStart();

	for(;;);
    return 0 ;
}


/** CPU Idle用 */
#ifdef __cplusplus
extern "C"
{
#endif
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);
extern void GetRunCount(uint32_t *p32Last, uint32_t *pu32Max);
#ifdef __cplusplus
}
#endif

extern uint32_t g_u32CurrentRun;
static uint32_t s_u32LastRun = 0u;
static uint32_t s_u32RunMax = 0u;

_Bool g_bInitEnd = false;
void vApplicationTickHook(void)
{
	static uint32_t cnt = 0u;
	if(g_bInitEnd != false){
		if (cnt >= 1000u)
		{	/** 1 sec */
			cnt = 0u;
			uint32_t primask = DisableGlobalIRQ();

			s_u32LastRun = g_u32CurrentRun;
			g_u32CurrentRun = 0u;
			if (s_u32LastRun > s_u32RunMax)
			{
				s_u32RunMax = s_u32LastRun;
			}
			EnableGlobalIRQ(primask);
		}else{
			cnt++;
		}
		GPIOMonitor();
	}else{
		g_u32CurrentRun = 0u;
	}
}

void GetRunCount(uint32_t *p32Last, uint32_t *pu32Max)
{
	if ((p32Last != NULL) && (pu32Max != NULL))
	{
		uint32_t primask = DisableGlobalIRQ();
		*p32Last = s_u32LastRun;
		*pu32Max = s_u32RunMax;
		EnableGlobalIRQ(primask);
	}
}

