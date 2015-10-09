/*
 * FCC_code_V1.cpp
 *
 * Created: 2015-10-09 3:54:27 PM
 *  Author: Reegan
 */ 

#include <avr32/io.h>
#include <avr32/rtc_100.h>
#include "CLKconfig.h"
#include "Output_defs.h"
#include "FuelCell_Inputs.h"
#include "myTIMER.h"
#include "mySerial.h"

avr32_rtc_isr_t rtcint;
avr32_rtc_imr_t rtcimr;

int main(void)
{
	ConfigureClock();
	InputsInit();
	
	//not sure what these do
	rtcint.topi=0;
	rtcimr.topi=0;
	
	//Set coil driving outputs and leds as outputs
	OP_GPERS |= OP1 | OP2 | OP3 | OP4 | OP5 | OP6 | OP7 | OP8 | OP9 | OP10;
	OP_ODERS |= OP1 | OP2 | OP3 | OP4 | OP5 | OP6 | OP7 | OP8 | OP9 | OP10;
	LED_GPERS |= LED0 | LED1 | LED2 | LED3;
	LED_ODERS |= LED0 | LED1 | LED2 | LED3;
	
	while(1)
	{
		OP_OVRS = OP1; //turn on an output
		LED_OVRS = LED1; //turn on a LED
		
		SendNumAsASCII(ReadCAPVOLT());
		
		SendNumAsASCII(ReadAMBTEMP1());
		//this might not work due to issues with this Analog pin being an negative input.
		//not sure if it just returns a negative value (i'm using unsigned int :)  ) or will just not work
		if(ReadAMBTEMP1() < 0)
		{
			LED_OVRS = LED2;
		}
	}
}