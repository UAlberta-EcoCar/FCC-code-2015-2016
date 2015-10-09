#include "CLKconfig.h"
#include <avr32/io.h>

void ConfigureClock (void)
{
	//configure 120 MHz clock to be used as system clock
	
	AVR32_PM.mcctrl = 7; //120 MHz RC oscillator (RC120M)
	//change clock divider
	//nevermind this register is locked AVR32_PM.CPUSEL = 0; //no divider
	
	//let clock settle
	while((AVR32_PM.sr & (1 << AVR32_PM_SR_CKRDY)) == 0)
	{
		//do nothing important until clock is ready
	}
	
	//configure 8/1 MHz clock that will be used with PWM
	//configuration register is locked. unlocking is a pain
	//I think it defaults to 8MHz
	//so i'll just leave it
	
	//disable unneeded clocks here
}