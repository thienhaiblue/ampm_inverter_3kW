#include <math.h>
#include "table_sin.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "main.h"
#include "power.h"
#include "filter.h"
#include "application.h"
#include "lpc17xx.h"
#include "tone.h"
								 


#define Vin_SCALE		0.038671	// =3.3/4096*48;
#define Iin_SCALE		0.031594	// = 3.3/4096/51/0.5mhom	 
#define Temp_SCALE		0.080566   // = 3.3/4096*100;
#define CALIBRATION		0.5	

#define VALUE_M_MAX		80

#define MAX_PWM_VALUE  TABLE_SIN_SIZE
//#define MAX_I_VALUE		1582
					 //1582   50A
					 //317   10A
					 //640		20A
					 //158	 //5A
					 //960		//30A
#define MAX_I_PULE_VALUE		3000	//100A
#define ADC7_MAX_VALUE			4000
#define ADC_CALC_SAMPLE_VALUE	400


uint32_t timer_counter = 0;
uint32_t ADC_counter = 0,flag_calc_RMS = 0;
uint32_t Vin_ADC_sample[ADC_CALC_SAMPLE_VALUE];
uint32_t Iin_ADC_sample[ADC_CALC_SAMPLE_VALUE];
uint32_t Temp_ADC_sample[ADC_CALC_SAMPLE_VALUE];

uint32_t Vin_RMS = 0;
uint32_t Iin_RMS = 0;
uint32_t Temp_RMS = 0;

uint32_t ADC3_value = 0;
uint32_t ADC6_value = 0;
uint32_t ADC7_value = 0;
uint32_t flag_voltage_low = 1;
uint32_t flag_current_high = 1;
uint32_t flag_over_temperature = 1;
uint32_t flag_turnoff_inverter = 0;
uint32_t flag_enable_pwm = 0;
uint32_t max_i_value_test = 0;	
float max_irms_value_test = 0;					   

volatile uint32_t delay_timer_counter = 0;

uint32_t counter_temp1 = 0,flag_tone = 1,temp1;

float f_Vin_RMS = 0;
float f_Iin_RMS = 0;
float f_Temp_RMS = 0; 


void TIMER2_IRQHandler(void)
{
	if((GPIO_ReadValue(0) & (1<<9)) == 0)
	{
		flag_turnoff_inverter = 0;
		if(flag_voltage_low)
		{
			DisableLedAlarmBattery();
		}
	
		else 
		{
		   EnableLedAlarmBattery();
		}
	
		if(flag_current_high)
		{
			DisableLedAlarmOverload();
		}
		else
		{
		   EnableLedAlarmOverload();
		}
	
		if(flag_over_temperature)
		{
		
		}
	
		if(flag_voltage_low | flag_current_high | flag_over_temperature)
		{
			buzzer.beep_note = 	Note_A;
			buzzer.beep_speed = 10;// = 10*20ms = 200ms
			buzzer.beep_start = 1;
		}
		else
		{
		 	buzzer.beep_start = 0;
		}
		
		if(buzzer.beep_start)
		{
			if(counter_temp1++ > buzzer.beep_speed)		
			{
				flag_tone ^= 1;
				counter_temp1 = 0;
			}
			if(flag_tone)
			{
			   SetPWM(buzzer.beep_note,50);
			   StartPWM();
			}
			else{
			//	StopPWM();
			   SetPWM(buzzer.beep_note,100);
			}	
		}
		else
		{
		//	StopPWM();
			SetPWM(buzzer.beep_note,100);
		}
	}
	else 
	{
		flag_voltage_low = 0;
		flag_current_high = 0;
		flag_turnoff_inverter = 1;
		DisableLedAlarmBattery();
		DisableLedAlarmOverload();
		buzzer.beep_note = 	Note_A;
		buzzer.beep_speed = 10;// = 10*20ms = 200ms
		buzzer.beep_start = 1;
		StartPWM();
		SetPWM(buzzer.beep_note,100);
	}

	LPC_TIM2->IR = 1;	
}

void TIMER3_IRQHandler(void)
{	  

	ADC6_value = (( LPC_ADC->ADDR6 >> 4 ) & 0x00000FFF);
	ADC3_value = (( LPC_ADC->ADDR3 >> 4 ) & 0x00000FFF);
	temp1 = 	 (( LPC_ADC->ADDR7 >> 4 ) & 0x00000FFF);	
	if(temp1 < ADC7_MAX_VALUE)
	{
		ADC7_value = LowPassFilerInteger(temp1,CONST_A_INTEGER);
		if(ADC7_value > max_i_value_test)
		{
			max_i_value_test = 	ADC7_value;
		}
		if(ADC7_value >= MAX_I_PULE_VALUE)	
		{				
			DisableIR2110();
			flag_current_high = 1;
		}
	//	ADC7_value = (uint32_t)LowPassFilerFloat(temp1,CONST_A_FLOAT);
	}
//	LPC_DAC->DACR  = (ADC7_value/4)<<6;
	//main code here
	LPC_TIM3->IR = 1;	
}


void ADC_IRQHandler(void)
{
	
}

						
void TIMER0_IRQHandler(void)
{
	
	timer_counter++;
	if(timer_counter >= MAX_PWM_VALUE)  timer_counter = 0;
	
	SetDutyPWM(table_sin[timer_counter]*value_m/100,table_sin[MAX_PWM_VALUE - timer_counter]*value_m/100);
	
		 
   	ADC_counter++;
	if(ADC_counter >= ADC_CALC_SAMPLE_VALUE)
	{
		 ADC_counter = 0; 
		 flag_calc_RMS = 1;
	}

	Vin_ADC_sample[ADC_counter]  = ADC6_value;
	Temp_ADC_sample[ADC_counter] = ADC3_value;
	Iin_ADC_sample[ADC_counter]  = ADC7_value;

	if(flag_voltage_low || flag_current_high || flag_over_temperature || flag_turnoff_inverter)
	{
	   	DisableIR2110();
		value_m = 0;	
		flag_enable_pwm = 0;	
	}
	else if(flag_enable_pwm == 0)
	{
		flag_enable_pwm = 1;
	   	EnableIR2110();
		value_m = 0;
		flag_soft_start = 1;
	}
	LPC_TIM0->IR = 1;	
}

void TIMER1_IRQHandler(void)
{

	if(delay_timer_counter)	 delay_timer_counter--;	  // su dung cho ham delay

	if(flag_soft_start)
	{  
		if(value_m > VALUE_M_MAX)	value_m = VALUE_M_MAX;
		else if(value_m < VALUE_M_MAX) value_m++;
		else if(value_m == VALUE_M_MAX)	flag_soft_start = 0;
	}
	if(flag_calc_RMS == 1)
	{
		Vin_RMS = CalculatorRMSValue(Vin_ADC_sample,ADC_CALC_SAMPLE_VALUE);
		Iin_RMS = CalculatorRMSValue(Iin_ADC_sample,ADC_CALC_SAMPLE_VALUE);
		Temp_RMS = CalculatorRMSValue(Temp_ADC_sample,ADC_CALC_SAMPLE_VALUE);
		f_Vin_RMS =  Vin_RMS*Vin_SCALE;
		f_Temp_RMS  =  Temp_RMS*Temp_SCALE;
		f_Iin_RMS  = Iin_RMS*Iin_SCALE*CALIBRATION;
		if(f_Iin_RMS > max_irms_value_test)	 max_irms_value_test = 	f_Iin_RMS;
		if(f_Iin_RMS >= CURRENT_MAX)	
		{
			flag_current_high = 1;
		}
		if(f_Vin_RMS < VOLTAGE_MIN)	 //Vol
		{
			flag_voltage_low = 1;// Bao thap ap
		}
		else if(f_Vin_RMS > VOLTAGE_OK)
		{
			flag_voltage_low = 0;		
		}
		if(f_Temp_RMS > TEMPERATURE_MAX)	 // bao nhiet do cao
		{
		   flag_over_temperature = 1;
		}
		else if(f_Temp_RMS < TEMPERATURE_OK)  	  //do C
		{
			flag_over_temperature = 0;
		}
		flag_calc_RMS = 0;
	}
	LPC_TIM1->IR = 1;	
}


uint32_t CalculatorRMSValue(uint32_t *data,uint32_t number_sample)
{	  
		uint32_t i,temp_value;
		uint64_t  sigma_squared = 0;
		for(i = 0;i < number_sample;i++)
		{
	  		//temp_value = (uint32_t)low_pass_filter2((float)data[i],TIME_CONST2,TIME_SAMPLE2);
			temp_value = data[i];
			sigma_squared += ( temp_value * temp_value );
		}
		sigma_squared /=  number_sample;
		temp_value = (uint32_t)sqrt((double)(sigma_squared));
		return 	temp_value;
}							

void OverCurrentBeep(void)
{
	buzzer.beep_note = 	Note_C;
	buzzer.beep_speed = 10;// = 10*20ms = 200ms
	buzzer.beep_start = 1;
}

void LowVoltageBeep(void)
{
	buzzer.beep_note = 	Note_F;
	buzzer.beep_speed = 10;// = 10*20ms = 200ms
	buzzer.beep_start = 1;
}
void OverTemperatureBeep(void)
{
	buzzer.beep_note = 	Note_B;
	buzzer.beep_speed = 10;// = 10*20ms = 200ms
	buzzer.beep_start = 1;
}





