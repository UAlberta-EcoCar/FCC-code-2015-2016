/*
 * FCC_code_V1.cpp
 *
 * Created: 2015-10-09 3:54:27 PM
 *  Author: Reegan
 */ 


#include <avr32/io.h>
#include <avr32/rtc_100.h>

#include "FuelCell_Outputs.h"
#include "CLKconfig.h"
#include "FuelCell_Inputs.h"
#include "mySerial.h"
#include "myTIMER.h"

avr32_rtc_isr_t rtcint;
avr32_rtc_imr_t rtcimr;

unsigned int x;
unsigned int y;

int main(void)
{
	ConfigureClock();
	InputsInit();
	EnableLeds();
	
	rtcint.topi=0;
	rtcimr.topi=0;
	
	//Set coil driving outputs as outputs
	OP_GPERS = OP1 | OP2 | OP3 | OP4 | OP5 | OP6 | OP7 | OP8 | OP9 | OP10;
	OP_ODERS = OP1 | OP2 | OP3 | OP4 | OP5 | OP6 | OP7 | OP8 | OP9 | OP10;
	
	
	while(1)
	{
		
	}
}