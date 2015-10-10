#include "myTIMER.h"
#include <avr32/io.h>


//I need to be able to time up to 300 ms accurately and multiple minutes
void SetUpTimer(void)
{
	//enable GCLK_AST (generic clock 3)
	AVR32_SCIF.gcctrl[3] |= 2 << AVR32_SCIF_GCCTRL_OSCSEL; // 8MHz clock source
	//AVR32_SCIF.gcctrl[3] |= 1 << AVR32_SCIF_GCCTRL_DIVEN; //divider enable, should probably use
	
	//set up Asynchronous timer

	//select clock source
	AVR32_AST.clock = 3 << AVR32_AST_CLOCK_CSSEL; //GCLK_AST
	//wait until SR.CLKBUSY reads zero
	while (AVR32_AST.sr & (1 << AVR32_AST_SR_CLKBUSY));
	{
		//do nothing
	}
	//write one to clock.cen without changing clock.cssel
	AVR32_AST.clock |= 1 << AVR32_AST_CLOCK_CEN;
	//Wait until sr.clkbusy reads as zero
	while (AVR32_AST.sr & (1 << AVR32_AST_SR_CLKBUSY));
	{
		//do nothing
	}
	//set AST prescaler
	AVR32_AST.cr |= 1 << AVR32_AST_CR_PCLR; //clear prescaler
	//wait until SR.CLKBUSY reads zero
	while (AVR32_AST.sr & (1 << AVR32_AST_SR_CLKBUSY));
	{
		//do nothing
	}
	//enable AST
	AVR32_AST.cr |= 1 << AVR32_AST_CR_EN;
}


unsigned int millis(void)
{
	unsigned int time = AVR32_AST.cv;
	//time is in usec we want millis
	return(time / 1000);
}
