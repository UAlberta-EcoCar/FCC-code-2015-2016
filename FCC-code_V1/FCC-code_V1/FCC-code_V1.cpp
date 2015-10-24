/*
 * FCC_code_V1.cpp
 *
 * Created: 2015-10-09 3:54:27 PM
 *  Author: Reegan
 */ 


#include <avr32/io.h>

#include "FuelCell_Outputs.h"
#include "CLKconfig.h"
#include "FuelCell_Inputs.h"
#include "mySerial.h"
#include "myTIMER.h"
#include "ast.h"

unsigned int x;
unsigned int y;

int main(void)
{
	opt.startup = AVR32_SCIF_OSCCTRL32_STARTUP_128_RCOSC;
	ConfigureClock();
	InputsInit();
	EnableLeds();
	AVR32_AST.cr |= 1 << AVR32_AST_CR_EN;
	AVR32_AST.clock = (0 << AVR32_AST_CLOCK_CEN);
	//set up Asynchronous timer
	//select clock sourcecccccc 
	//wait until SR.CLKBUSY reads zero
	x=AVR32_AST.sr;
	y=AVR32_AST.cr;
	while (AVR32_AST.sr & (1 << AVR32_AST_SR_CLKBUSY));
	//write one to clock.cen without changing clock.cssel
	
	//Wait until sr.clkbusy reads as zero
	while (AVR32_AST.sr & (1 << AVR32_AST_SR_CLKBUSY));
	//set AST prescaler
	AVR32_AST.cr |= 1 << AVR32_AST_CR_PCLR; //clear prescaler
	//wait until SR.CLKBUSY reads zero
	while (AVR32_AST.sr & (1 << AVR32_AST_SR_CLKBUSY));
	//enable AST
	AVR32_AST.clock |= 1 << AVR32_AST_CLOCK_CEN;
	
	DLED_GPERS=(LED0 | LED1 | LED2 | LED3);
	DLED_ODERS=(LED0 | LED1 | LED2 | LED3);
	
	
	while(1)
	{
		x=AVR32_AST.cv;
		x++;
	}
}