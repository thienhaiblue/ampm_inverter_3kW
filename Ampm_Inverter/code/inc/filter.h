#ifndef _FILTER_H_
#define _FILTER_H_
#include "stdint.h"
#ifdef __cplusplus
extern "C" {
#endif

#define CONST_A_FLOAT   0.11163	   // use for method 2
#define CONST_A_INTEGER   11163	   // use for method 1
#define SCALE_CONST_A	100000	   // use for method 1

int32_t LowPassFilerInteger(int32_t xt,int32_t a);	  // method 1
float LowPassFilerFloat(float xt,float a);			   //method 2


#ifdef __cplusplus
}
#endif

#endif 

