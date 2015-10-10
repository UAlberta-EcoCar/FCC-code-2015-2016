#include <avr32/io.h>
#include "FuelCell_Outputs.h"
/*
 * CPPFile1.cpp
 *
 * Created: 2015-10-10 1:46:16 PM
 *  Author: Reegan
 */ 

void EnableLeds(void)
{
	AVR32_GPIO.port[2].gpers = LED_START | LED_ERROR | LED_STOP;
	AVR32_GPIO.port[2].oders = LED_START | LED_ERROR | LED_STOP;
	AVR32_GPIO.port[1].gpers = LED_RUN;
	AVR32_GPIO.port[1].oders = LED_RUN;
	
	DLED_GPERS = LED0 | LED1 | LED2 | LED3;
	DLED_ODERS = LED0 | LED1 | LED2 | LED3;
}