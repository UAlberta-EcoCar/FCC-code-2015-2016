#include "myTIMER.h"
#include <avr32/io.h>


//I need to be able to time up to 300 ms accurately and multiple minutes
void SetUpTimer(void)
{
	//enable GCLK_AST (generic clock 3)
	//AVR32_SCIF.gcctrl[3] |= 1 << AVR32_SCIF_GCCTRL_DIVEN; //divider enable, should probably use
	AVR32_AST.clock = (0 << AVR32_AST_CLOCK_CEN);
	//set up Asynchronous timer
	AVR32_AST.cr |= 0 << AVR32_AST_CR_EN;
	//select clock sourcecccccc 
	//wait until SR.CLKBUSY reads zero
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
	AVR32_AST.cr |= 1 << AVR32_AST_CR_EN;
}


unsigned int millis(void)
{
	unsigned int time = AVR32_AST.cv;
	//time is in usec we want millis
	return(time / 1000);
}
