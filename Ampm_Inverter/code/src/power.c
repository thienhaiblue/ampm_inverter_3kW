#include "power.h"
#include "lpc17xx_MCPWM.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_clkpwr.h"

uint32_t PWM_deadtime_value;
uint32_t PWM_period_value;
uint32_t value_m   = 0;//  from 1 to 100
uint32_t flag_soft_start = 1;

void InitPWM(uint32_t frnquency,uint32_t deadtime)
{
		PINSEL_CFG_Type PinCfg;
		uint32_t val_f;
		// MCPWM Channel configuration data
		MCPWM_CHANNEL_CFG_Type channelsetup[3];

//	 * 			- P1.25 as MCOA1 - Motor Control Channel 1 Output A
//	 * 			- P1.26 as MCOB1 - Motor Control Channel 1 Output B
//	 * 			- P1.28 as MCOA2 - Motor Control Channel 2 Output A
//	 * 			- P1.29 as MCOB2 - Motor Control Channel 2 Output B
//	 * 			- P1.20 as MCI0	 - Motor Control Feed Back Channel 0
//	 * Warning: According to Errata.lpc1768-18.March.2010: Input pin (MIC0-2)
//	 * on the Motor Control PWM peripheral are not functional

	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 1;
	PinCfg.Pinnum = 25;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 26;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 28;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 29;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 20;
	PINSEL_ConfigPin(&PinCfg);
//	 Disable interrupt for MCPWM  
	NVIC_DisableIRQ(MCPWM_IRQn);
//	 Init MCPWM peripheral 
	MCPWM_Init(LPC_MCPWM);
	val_f = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_MC);	  //Tan so PCLK_PWM
	PWM_deadtime_value = (val_f/1000)*deadtime/1000000 - 1;
	channelsetup[1].channelType = MCPWM_CHANNEL_EDGE_MODE;
	channelsetup[1].channelPolarity = MCPWM_CHANNEL_PASSIVE_LO;
	channelsetup[1].channelDeadtimeEnable = ENABLE;
	channelsetup[1].channelDeadtimeValue = PWM_deadtime_value;
	channelsetup[1].channelUpdateEnable = ENABLE;
	channelsetup[1].channelTimercounterValue = 0;
	channelsetup[1].channelPeriodValue = val_f/frnquency - 1;
	channelsetup[1].channelPulsewidthValue = 0xffffffff;

	channelsetup[2].channelType = MCPWM_CHANNEL_EDGE_MODE;
	channelsetup[2].channelPolarity = MCPWM_CHANNEL_PASSIVE_LO;
	channelsetup[2].channelDeadtimeEnable = ENABLE;
	channelsetup[2].channelDeadtimeValue = PWM_deadtime_value;
	channelsetup[2].channelUpdateEnable = ENABLE;
	channelsetup[2].channelTimercounterValue = (val_f/1000)*deadtime/1000000 - 1;
	channelsetup[2].channelPeriodValue = val_f/frnquency - 1;
	channelsetup[2].channelPulsewidthValue = 0xffffffff;

	MCPWM_ConfigChannel(LPC_MCPWM, 1, &channelsetup[1]);
	MCPWM_ConfigChannel(LPC_MCPWM, 2, &channelsetup[2]);

	MCPWM_DCMode(LPC_MCPWM, DISABLE, DISABLE, (MCPWM_PATENT_A1|MCPWM_PATENT_B1 | MCPWM_PATENT_A2|MCPWM_PATENT_B2));
	MCPWM_Start(LPC_MCPWM, DISABLE, ENABLE, ENABLE);
	LPC_MCPWM->MCPW1 = 0xffffffff;
	LPC_MCPWM->MCPW2 = 0xffffffff;
}


void SetDutyPWM(uint32_t duty_channelA,uint32_t duty_channelB)
{
	
	duty_channelA = LPC_MCPWM->MCPER1 - duty_channelA;
	if(duty_channelA <= (((LPC_MCPWM->MCDEADTIME>>10) & 0x000003ff)))	  duty_channelA = 0;
	else if(duty_channelA >=(LPC_MCPWM->MCPER1 - ((LPC_MCPWM->MCDEADTIME>>10) & 0x000003ff)))
		LPC_MCPWM->MCPW1 = 0xffffffff; 	
	else  LPC_MCPWM->MCPW1 = duty_channelA;	

	duty_channelB = LPC_MCPWM->MCPER2 - duty_channelB;
	if(duty_channelB <= (((LPC_MCPWM->MCDEADTIME>>20) & 0x000003ff)))	  duty_channelB = 0;
	else if(duty_channelB >=(LPC_MCPWM->MCPER2 - ((LPC_MCPWM->MCDEADTIME>>20) & 0x000003ff)))
		LPC_MCPWM->MCPW2 = 0xffffffff; 	
	else  LPC_MCPWM->MCPW2 = duty_channelB;	
}


//void SetDutyPWM(uint32_t duty_channelA,uint32_t duty_channelB)
//{
//	if(duty_channelA <= LPC_MCPWM->MCPER1)
//	{
//		duty_channelA = LPC_MCPWM->MCPER1 - duty_channelA;
//		if(duty_channelA  <= (((LPC_MCPWM->MCDEADTIME>>10) & 0x000003ff) + 0))	
//		{
//			 LPC_MCPWM->MCPW1 = 0; 
//		}
//		else if(duty_channelA  >= ((LPC_MCPWM->MCPER1 - ((LPC_MCPWM->MCDEADTIME>>10) & 0x000003ff)) - 0))	
//		{
//			 LPC_MCPWM->MCPW1 = 0xffffffff; 
//		}
//		else LPC_MCPWM->MCPW1 = duty_channelA;	
//	}
//	if(duty_channelB <= LPC_MCPWM->MCPER2)
//	{
//		duty_channelB = LPC_MCPWM->MCPER2 - duty_channelB;
//		if(duty_channelB  <= (((LPC_MCPWM->MCDEADTIME>>20) & 0x000003ff)))	
//		{
//			 LPC_MCPWM->MCPW2 = 0; 
//		}
//		else if(duty_channelB  >= ((LPC_MCPWM->MCPER2 - ((LPC_MCPWM->MCDEADTIME>>20) & 0x000003ff))- 0))	
//		{
//			 LPC_MCPWM->MCPW2 = 0xffffffff; 
//		}
//		else LPC_MCPWM->MCPW2 = duty_channelA;	
//   }
//}


