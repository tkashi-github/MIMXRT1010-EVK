/**
 * @file DrvLPUART.h
 * @brief TODO
 * @author Takashi Kashiwagi
 * @date 2018/10/28
 * @version     0.1
 * @details 
 * --
 * License Type <MIT License>
 * --
 * Copyright 2018 Takashi Kashiwagi
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
 * - 2018/10/28: Takashi Kashiwagi: v0.1 for IMXRT1060-EVK
 */
#ifndef __cplusplus
#if __STDC_VERSION__ < 201112L
#error /** Only C11 */
#endif
#endif
#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

/** User Typedefine */
#include "UserTypedef.h"
#include "OSResource.h"

#define kStdioPort enLPUART1

#include "fsl_lpuart.h"

extern void LPUART1_IRQHandler(void);
extern void LPUART2_IRQHandler(void);
extern void LPUART3_IRQHandler(void);
extern void LPUART4_IRQHandler(void);
extern void LPUART5_IRQHandler(void);
extern void LPUART6_IRQHandler(void);
extern void LPUART7_IRQHandler(void);
extern void LPUART8_IRQHandler(void);

/**
 * @brief Initialize LPUART
 * Ex:
 * 	lpuart_config_t lpuart_config;
 * 	LPUART_GetDefaultConfig(&lpuart_config);
 * 	lpuart_config.baudRate_Bps = baudRate;
 * 	DrvLPUART_Init(enLPUART1, &lpuart_config);
 * 
 * @param [in]  enLPUARTNo PortNumber
 * @param [in]  config UART parameters
 * @return true Success
 * @return false NG
 */
extern _Bool DrvLPUARTInit(enLPUART_t enLPUARTNo, const lpuart_config_t *config);

/**
 * @brief Send Data via LPUART
 * Example usage:
<pre>
static inline void RTOS_PutChar(TCHAR ch)
{
	DrvLPUARTSend(kStdioPort, (const uint8_t *)&ch, sizeof(TCHAR));
}
</pre>
 * @param [in]  enLPUARTNo PortNumber
 * @param [in]  pu8data TxData Array
 * @param [in]  ByteCnt Tx Size
 * @return true Success
 * @return false NG
 */
extern _Bool DrvLPUARTSend(enLPUART_t enUARTNo, const uint8_t pu8data[], const uint32_t ByteCnt);

/**
 * @brief Send Data via LPUART
 * Example usage:
<pre>
static inline void RTOS_GetChar(TCHAR *ch)
{
	if (ch != NULL)
	{
		DrvLPUARTRecv(kStdioPort, (uint8_t *)ch, sizeof(TCHAR), portMAX_DELAY);
	}
}
</pre>
 * @param [in]  enLPUARTNo PortNumber
 * @param [out]  pu8data RxData Array
 * @param [in]  ByteCnt Rx Size
 * @return true Success
 * @return false NG
 */
extern _Bool DrvLPUARTRecv(enLPUART_t enUARTNo, uint8_t pu8data[], const uint32_t ByteCnt, uint32_t u32Timeout);

static inline _Bool DrvLPUARTIsRxBufferEmpty(enLPUART_t enUARTNo)
{
    _Bool bret = false;
    if ((enUARTNo >= enLPUART_MIN) || (enUARTNo <= enLPUART_MAX))
	{
        bret = (_Bool)xStreamBufferIsEmpty(g_sbhLPUARTRx[enUARTNo]);
    }
    return bret;
}

#ifdef __cplusplus
}
#endif

