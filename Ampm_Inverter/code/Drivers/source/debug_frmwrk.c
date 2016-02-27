/***********************************************************************//**
 * @file		debug_frmwrk.c
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

#include "debug_frmwrk.h"
#include "lpc17xx_pinsel.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */
#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc17xx_libcfg.h"
#else
#include "lpc17xx_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */

#ifdef _DBGFWK

/*********************************************************************//**
 * @brief		print function that supports format as same as printf()
 * 				function of <stdio.h> library
 * @param[in]	None
 * @return		None
 **********************************************************************/
void  printf_dbg (const uint8_t *format, ...)
{ 
    static  uint8_t  buffer[256];
     va_list     vArgs;		    
     unsigned int	len;
    va_start(vArgs, format);
    vsprintf((char *)buffer, (char const *)format, vArgs);
    va_end(vArgs);
	len =  strlen((const char *)buffer);
	if(len < 256)
	{
		UART_Send((LPC_UART_TypeDef *)LPC_UART1,(uint8_t *)buffer,len,BLOCKING);
	}
}


/*********************************************************************//**
 * @brief		Initialize Debug frame work through initializing UART port
 * @param[in]	None
 * @return		None
 **********************************************************************/
void debug_frmwrk_init(void)
{
	UART_CFG_Type UARTConfigStruct;
	PINSEL_CFG_Type PinCfg;


	/*
	 * Initialize UART1 pin connect
	 */
	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 0;
	PinCfg.Portnum = 2;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 1;
	PINSEL_ConfigPin(&PinCfg);

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);
	// Re-configure baudrate to 9600bps
	UARTConfigStruct.Baud_rate = 9600;

	// Initialize DEBUG_UART_PORT peripheral with given to corresponding parameter
	UART_Init(DEBUG_UART_PORT, &UARTConfigStruct);

	// Enable UART Transmit
	UART_TxCmd(DEBUG_UART_PORT, ENABLE);
}
#endif /*_DBGFWK */
