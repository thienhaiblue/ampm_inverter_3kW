#ifndef _MAIN_H_
#define _MAIN_H_
#include "stdint.h"
#ifdef __cplusplus
extern "C" {
#endif

#define ADC_GET_FREQUENCY 50000	//Hz
#define PWM_FREQUENCY 20000//Hz
#define DEADTIME      2000 //ns
#define ADC_FREQUENCY 200000//200kHz

#define PWM_OFF_PIN			(1<<6)  //P0.6
#define InitIR2110()	LPC_GPIO0->FIODIR |= PWM_OFF_PIN	
#define DisableIR2110()	LPC_GPIO0->FIOSET |= PWM_OFF_PIN
#define EnableIR2110()	LPC_GPIO0->FIOCLR |= PWM_OFF_PIN

#define DTR_PIN			(1<<4)
#define SetOutputDTR()	LPC_GPIO2->FIODIR |= DTR_PIN	
#define SetDTR()		LPC_GPIO2->FIOSET |= DTR_PIN
#define ClearDTR()		LPC_GPIO2->FIOCLR |= DTR_PIN

#define LED_ALARM_BATTERY			(1<<8)  //P0.8
#define InitLedAlarmBattery()	LPC_GPIO0->FIODIR |= LED_ALARM_BATTERY	
#define EnableLedAlarmBattery()	LPC_GPIO0->FIOSET |= LED_ALARM_BATTERY
#define DisableLedAlarmBattery()	LPC_GPIO0->FIOCLR |= LED_ALARM_BATTERY

#define LED_ALARM_OVERLOAD			(1<<7)  //P0.7
#define InitLedAlarmOverload()	LPC_GPIO0->FIODIR |= LED_ALARM_OVERLOAD	
#define EnableLedAlarmOverload()	LPC_GPIO0->FIOSET |= LED_ALARM_OVERLOAD
#define DisableLedAlarmOverload()	LPC_GPIO0->FIOCLR |= LED_ALARM_OVERLOAD
				   
#ifdef __cplusplus
}
#endif

#endif 

