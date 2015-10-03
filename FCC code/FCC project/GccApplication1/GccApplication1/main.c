/*
 * GccApplication1.c
 *
 * Created: 2015-09-27 10:02:52 AM
 * Author : Me
 */ 

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


struct avr32_rtc_t rtc;

int main(void)
{
	rtc.ctrl=0;
	AVR32_GPIO.port[3].gpers=OP_EN;
	AVR32_GPIO.port[3].oders=OP_EN;
	
    while(1)
    {
		AVR32_GPIO.port[3].ovrs=OP1;
		AVR32_GPIO.port[3].ovrc=OP1;
        
    }
}

