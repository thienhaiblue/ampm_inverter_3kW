#include <stdio.h>                /* prototype declarations for I/O functions */
#include <math.h>
#include "filter.h"
#include "stdint.h"
/******************************************************************************
ut/et = 1/(as+1)   
x[(k+1)T] = Adx(kT) + Bde(kT)	;T la thoi gian lay mau		R
c(kT) = Cdx(kT)									    ______/\/\/\_________
trong do:							 <==>  		 				  _|_				  
Ad = exp(-aT)								e(t) 	a =1/RC		  _ _ C		r(t)
Bd = (1-Ad)/a							   		 	_______________|_____
Cd =  a   										 
******************************************************************************/

int32_t LowPassFilerInteger(int32_t xt,int32_t a)
{	 static int32_t yt;
	yt = yt + a*(xt - yt)/SCALE_CONST_A;
	return yt;
}

float LowPassFilerFloat(float xt,float a)
{	 static float yt;
	yt = yt + a*(xt - yt);
	return yt;
}


