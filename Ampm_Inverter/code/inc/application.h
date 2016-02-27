#ifndef _APPLICATION_H_
#define _APPLICATION_H_
#include "stdint.h"
#ifdef __cplusplus
extern "C" {
#endif


#define VOLTAGE_MIN	44.0  //vol
#define VOLTAGE_OK  50.0
#define CURRENT_MAX 50.0 //Ampe
#define TEMPERATURE_MAX 70.0 //	Do C
#define TEMPERATURE_OK 40.0 //	Do C

extern volatile uint32_t delay_timer_counter;
extern float f_Vin_RMS;
extern float f_Iin_RMS;
extern float f_Temp_RMS; 
extern uint32_t flag_voltage_low;
extern uint32_t flag_current_high;
extern uint32_t flag_over_temperature;
extern uint32_t max_i_value_test;	
extern float max_irms_value_test;

void OverCurrentBeep(void);
void LowVoltageBeep(void);
void OverTemperatureBeep(void) ;
uint32_t CalculatorRMSValue(uint32_t *data,uint32_t number_sample);

#ifdef __cplusplus
}
#endif

#endif 

