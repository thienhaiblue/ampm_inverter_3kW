#ifndef _POWER_H_
#define _POWER_H_
#include "stdint.h"
#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t value_m;//  from 1 to 100
extern uint32_t flag_soft_start;

extern void InitPWM(uint32_t frnquency,uint32_t deadtime);
extern void SetDutyPWM(uint32_t duty_channelA,uint32_t duty_channelB);


#ifdef __cplusplus
}
#endif

#endif 

