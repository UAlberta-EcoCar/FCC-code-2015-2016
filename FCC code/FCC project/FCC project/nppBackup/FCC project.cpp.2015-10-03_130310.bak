/*
 * FCC_project.cpp
 *
 * Created: 2015-09-19 2:00:27 PM-9
 *  Author: Me
 */ 


#define F_CPU 80000000

#include <avr32/io.h>
#include <avr32/rtc_100.h>
#include "FCCutils.h"


avr32_rtc_isr_t rtcint;
avr32_rtc_imr_t rtcimr;

int main(void)
{
	rtcint.topi=0;
	rtcimr.topi=0;
	AVR32_GPIO.port[3].gpers=OP_EN;
	AVR32_GPIO.port[3].oders=OP_EN;
	
    while(1)
    {
		AVR32_GPIO.port[3].ovrs=OP1;
		AVR32_GPIO.port[3].ovrc=OP1;
        
    }
}