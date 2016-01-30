/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a> //this site is useless
 *
/*


to do list:
wire everything up
pressure readings
temp conversion
can bus
	data logging
	active fuel cell control
get PID values
test fan pid control code
Fix startup_fans state

*/

//change this file depending on whether you are using the test bench or not
#include "FuelCell_mode_Select.h"

#include "asf.h"
#include "FuelCell_ADC.h"
#include "FuelCell_Functions.h"
#include "FuelCell_PWM.h"
#include "digital_IO_defs.h"
#include "FC_error_codes.h"
#include "FuelCell_USART.h"
#include "FuelCell_CAN.h"
#include "pid.h"
#include "millis_function.h"
#include "FuelCell_check_alarms.h"
#include "wdt_scheduler.h"
#include "FuelCell_DataLogging.h"

unsigned int error_msg;
unsigned int fc_state = FC_STATE_STANDBY;


int main (void)
{
	board_init();
	
	//zero readings
	StartADC_Sequencers(); //start ADC conversion
	ReadADC_Sequencers(); //read conversion results	
	//zero_CAPVOLT(); Not a good idea if caps have left over voltage
	zero_FCCURR(); 
	//zero_FCVOLT(); Not a good idea either
	
	error_msg |= wdt_scheduler(); //start watchdog timer
	//comment out for debugging (debugger is supposed to disable wdt automatically but it doesn't always)
		
	//Start of main loop
	while(1)
	{
		//read analog inputs everytime the main loop runs
		StartADC_Sequencers(); //start another conversion
		ReadADC_Sequencers(); //read conversion
		
		//clear wdt value
		wdt_clear();
		//if code gets hung up wdt won't clear and a reset will occur
		
		error_msg |= FC_check_alarms(fc_state);
		error_msg &= ERROR_MASK;
		if(error_msg)
		{
			fc_state = FC_STATE_ALARM;
		}
		
		//main state machine
		switch (fc_state)
		{
		case FC_STATE_STANDBY:
			fc_state = FC_standby();
			break;
			
		case FC_STATE_SHUTDOWN:
			fc_state = FC_shutdown();
			break;
			
		case FC_STATE_STARTUP_FANS:
			fc_state = FC_startup_fans();
			break;
			
		case FC_STATE_STARTUP_H2:
			fc_state = FC_startup_h2();
			break;
			
		case FC_STATE_STARTUP_PURGE:
			fc_state = FC_startup_purge();
			break;
			
		case FC_STATE_STARTUP_CHARGE:
			fc_state = FC_startup_charge();	
			break;
			
		case FC_STATE_RUN:
			fc_state = FC_run();
			break;
			
		case FC_STATE_ALARM:
			fc_state = FC_alarm();			
			break;
		}	
		
		usart_data_log(fc_state,error_msg);
	}
}