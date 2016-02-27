#include <stdio.h>
#include "lpc17xx_libcfg.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"
#include "power.h"
#include "delay.h"
#include "sensor.h"
#include "interrupt_priority.h"
#include "table_sin.h"
#include "tone.h"
#include "main.h"
#include "application.h"


uint32_t temp = 0;
uint32_t fist_startup = 1;
uint8_t buff[32];
uint32_t len = 0;



void DelayTimer20ms(uint32_t time)
{
	delay_timer_counter = time;
	while(delay_timer_counter);
}


void InitTimerMainProcess(void)
{
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
   // Initialize timer 1, prescale count time of tickval
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1;

	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	TIM_MatchConfigStruct.MatchValue   = 20000;	//20000us
	TIM_MatchConfigStruct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM2, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM2,&TIM_MatchConfigStruct);

	/* preemption = 1, sub-priority = 0 */
	NVIC_SetPriority(TIMER2_IRQn, ((0x01<<3)|PRIORITY_TIMER2));
	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER2_IRQn);
	// To start timer 0
	TIM_Cmd(LPC_TIM2,ENABLE);
}


void InitTimerProcessSensorRMSValue(void)
{
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
   // Initialize timer 1, prescale count time of tickval
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1;

	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	TIM_MatchConfigStruct.MatchValue   = 20000;	//20000us
	TIM_MatchConfigStruct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM1, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM1,&TIM_MatchConfigStruct);

	/* preemption = 1, sub-priority = 0 */
	NVIC_SetPriority(TIMER1_IRQn, ((0x01<<3)|PRIORITY_TIMER1));
	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER1_IRQn);
	// To start timer 0
	TIM_Cmd(LPC_TIM1,ENABLE);
}

void InitTimerADCRead(void)
{
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
   // Initialize timer 1, prescale count time of tickval
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_TICKVAL;
	TIM_ConfigStruct.PrescaleValue	= 1;

	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	TIM_MatchConfigStruct.MatchValue   = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_TIMER0)/ADC_GET_FREQUENCY;	
	TIM_MatchConfigStruct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM3, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM3,&TIM_MatchConfigStruct);

	/* preemption = 1, sub-priority = 0 */
	NVIC_SetPriority(TIMER3_IRQn, ((0x01<<3)|PRIORITY_TIMER3));
	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER3_IRQn);
	// To start timer 0
	TIM_Cmd(LPC_TIM3,ENABLE);
}

void InitTimerProcessSinwave(void)
{
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
   // Initialize timer 1, prescale count time of tickval
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_TICKVAL;
	TIM_ConfigStruct.PrescaleValue	= 1;

	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	TIM_MatchConfigStruct.MatchValue   = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_TIMER0)/PWM_FREQUENCY;	//40us
	TIM_MatchConfigStruct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM0,&TIM_MatchConfigStruct);

	/* preemption = 1, sub-priority = 0 */
	NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3)|PRIORITY_TIMER0));
	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER0_IRQn);
	// To start timer 0
	TIM_Cmd(LPC_TIM0,ENABLE);
}

void InitDAC(void)
{  	PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 26;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg); 					

}

/*-------------------------MAIN FUNCTION------------------------------
**********************************************************************/
int c_entry(void)
{	
	InitIR2110();  // Use for Driver PWM IC IR2110
//	EnableIR2110();
	DisableIR2110(); 

	flag_soft_start = 1;

	#ifdef RUN_OK
	Delayms(3000);
	#endif
//	InitDAC();
			//	 Initialize debug via UART0
//	  – 9600bps
//	  – 8 data bit
//	  – No parity
//	  – 1 stop bit
//	  – No flow control	 
	debug_frmwrk_init();
	SetOutputDTR();
	ClearDTR();  //data receive
	InitLedAlarmBattery();
	InitLedAlarmOverload();
	InitPWM(PWM_FREQUENCY,DEADTIME);
	SetDutyPWM(0,0); 
	InitADC(ADC_FREQUENCY); 
	
	InitTimerADCRead();
	
	InitTimerProcessSinwave();
	
	InitTimerProcessSensorRMSValue();
	InitTimerMainProcess();
	InitBuzzer(10000);
	
	#ifdef RUN_OK
	while(fist_startup)
	{
		if(f_Vin_RMS > VOLTAGE_MIN/*Vol*/)	
		{
			flag_voltage_low = 0;
			flag_current_high = 0;
			fist_startup = 0;
		}
	}
	#endif
	while (1)
	{	
		if ((LPC_UART1->LSR & UART_LSR_RDR)) 
		{
			buff[0] = UART_ReceiveByte((LPC_UART_TypeDef *)LPC_UART1);
			if(buff[0] == 'I')
			{
				Delayms(100);
				SetDTR();	  //data transmission
				printf_dbg("I,%d,\r",(uint32_t)(f_Vin_RMS*f_Iin_RMS));
				Delayms(10);
				ClearDTR();	  //data receive
				buff[0] = 0;
			}
		}
	}
    return 1;
}

int main(void)
{
    return c_entry();
}


#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif



