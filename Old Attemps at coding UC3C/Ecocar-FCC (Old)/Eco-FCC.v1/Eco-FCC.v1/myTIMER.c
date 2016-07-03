#include "myTIMER.h"
#include <avr32/io.h>



//I need to be able to time up to 300 ms and multiple minutes
void SetUpTimer(void)
{
	//disable AST
	AVR32_AST.cr = 0;
	//wait until SR.CLKBUSY reads zero
	while (AVR32_AST.sr & (1 << AVR32_AST_SR_CLKBUSY));
	{
		//do nothing
	}
	//enable GCLK_AST (generic clock 3)
	AVR32_SCIF.gcctrl[3] |= 2 << AVR32_SCIF_GCCTRL_OSCSEL; // 1MHz clock source
	//AVR32_SCIF.gcctrl[3] |= 1 << AVR32_SCIF_GCCTRL_DIVEN; //divider enable
	//set up Asynchronous timer
	//first disable
	AVR32_AST.clock = 0;
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
	return(AVR32_AST.cv / 1000);
}

