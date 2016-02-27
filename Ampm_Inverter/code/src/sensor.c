#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_clkpwr.h"

void InitADC(uint32_t frequency)
{	  PINSEL_CFG_Type PinCfg;
	
	 //Init ADC pin connect
	 // AD0.2 on P0.25
	 
//	PinCfg.Funcnum = 1;
//	PinCfg.OpenDrain = 0;
//	PinCfg.Pinmode = 0;
//	PinCfg.Pinnum = 25;
//	PinCfg.Portnum = 0;
//	PINSEL_ConfigPin(&PinCfg);
//	
	// Init ADC pin connect
	// AD0.3 on P0.26
	 
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 26;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);	  
	
//	 //Init ADC pin connect
//	 // AD0.4 on P1.30
//	 
//	PinCfg.Funcnum = 3;
//	PinCfg.OpenDrain = 0;
//	PinCfg.Pinmode = 0;
//	PinCfg.Pinnum = 30;
//	PinCfg.Portnum = 1;
//	PINSEL_ConfigPin(&PinCfg);


//	PinCfg.Funcnum = 3;
//	PinCfg.OpenDrain = 0;
//	PinCfg.Pinmode = 0;
//	PinCfg.Pinnum = 31;
//	PinCfg.Portnum = 1;
//	PINSEL_ConfigPin(&PinCfg);

	//AD6	   P0.3
	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 3;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
  
	//AD7		   P0.2
	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 2;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);	 
	 //Configuration for ADC:
	  //ADC conversion rate = 200KHz
	 
	ADC_Init(LPC_ADC, frequency);
//	ADC_IntConfig(LPC_ADC,ADC_ADINTEN7,ENABLE);
//	ADC_IntConfig(LPC_ADC,ADC_ADINTEN2,ENABLE);
//	ADC_IntConfig(LPC_ADC,ADC_ADINTEN6,ENABLE);
//	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_2,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_3,ENABLE);
//	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_4,ENABLE);
//	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_5,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_6,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_7,ENABLE);
///* preemption = 1, sub-priority = 1 */
//	NVIC_SetPriority(ADC_IRQn, ((0x01<<3)|0x01));
	ADC_BurstCmd(LPC_ADC,ENABLE);
///	/* Enable ADC in NVIC */
//	NVIC_EnableIRQ(ADC_IRQn);
}

