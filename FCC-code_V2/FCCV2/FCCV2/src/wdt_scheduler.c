/*
 * wdt_scheduler.c
 *
 * Created: 2015-12-13 6:15:19 PM
 *  Author: Reegan
 */ 
#include "wdt_scheduler.h"
#include "asf.h"
#include "digital_IO_defs.h"
#include "FC_error_codes.h"

int wdt_scheduler(void)
{
	U32 error_msg = 0;
	//enable wdt
	wdt_opt_t opt = {
		.dar   = false,     // After a watchdog reset, the WDT will still be enabled.
		.mode  = WDT_BASIC_MODE,    // The WDT is in basic mode, only PSEL time is used.
		.sfv   = false,     // WDT Control Register is not locked.
		.fcd   = false,     // The flash calibration will be redone after a watchdog reset.
		.cssel = WDT_CLOCK_SOURCE_SELECT_RCSYS,       // 115K system oscillator
		.us_timeout_period = 250000  // Time out value: 250 ms
	};
	//if last reset was power on reset enable watchdog timer
	if(AVR32_PM.RCAUSE.por)
	{
		wdt_enable(&opt);
	}	
	//if last reset was wdt reset reenable wdt and toggle LED3 to notify
	else if(AVR32_PM.RCAUSE.wdt)
	{
		wdt_reenable();
		error_msg |= FC_ERR_WDT;
		gpio_set_gpio_pin(LED3);
	}
	//if last reset was external reset enable wdt
	else if(AVR32_PM.RCAUSE.ext)
	{
		wdt_enable(&opt);
	}
	else if(AVR32_PM.RCAUSE.bod | AVR32_PM.RCAUSE.bod33)
	{
		error_msg |= FC_ERR_BOD;
		gpio_set_gpio_pin(LED3);
	}
	else 
	{
		wdt_enable(&opt);	
	}
	return(error_msg);
}