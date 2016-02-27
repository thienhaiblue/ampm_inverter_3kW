/***********************************************************************//**
 * @file		debug_frmwrk.h
 * @brief		Contains some utilities that used for debugging through UART
 * @version		2.0
 * @date		21. May. 2010
 * @author		NXP MCU SW Application Team
 *----------------------------------------------------------------------------
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/

#ifndef DEBUG_FRMWRK_H_
#define DEBUG_FRMWRK_H_

#include <stdarg.h>
#include "lpc17xx_uart.h"

#define USED_UART_DEBUG_PORT	1

#if (USED_UART_DEBUG_PORT==0)
#define DEBUG_UART_PORT	LPC_UART0
#elif (USED_UART_DEBUG_PORT==1)
#define DEBUG_UART_PORT	(LPC_UART_TypeDef *)LPC_UART1
#endif

void  printf_dbg (const  uint8_t *format, ...);

void debug_frmwrk_init(void);

#endif /* DEBUG_FRMWRK_H_ */
