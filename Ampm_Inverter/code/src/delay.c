#include "system_LPC17xx.h"
#include "delay.h"

void Delay(unsigned long loop)
{ 
	while(loop--);
}
void Delayus(unsigned long time)
{	  unsigned long system_clock,loop;
	system_clock  =  SystemCoreClock/5500000;	
   while(time--) {
   	loop = system_clock;
   	while(loop--);	
	}
}
void Delayms(unsigned long time)
{
	unsigned long system_clock,loop;
	system_clock  =  SystemCoreClock/5000;
	
   while(time--) {
   	loop = system_clock;
   	while(loop--);	
	}
}
