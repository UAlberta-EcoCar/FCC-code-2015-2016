/*
to do list:

pressure readings
temp conversion
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
//#include "FuelCell_CAN.h"
#include "pid.h"
#include "millis_function.h"
#include "FuelCell_check_alarms.h"
#include "wdt_scheduler.h"
#include "FuelCell_DataLogging.h"


unsigned int error_msg;
unsigned int fc_state = FC_STATE_STANDBY;
unsigned int past_fc_state = 0;
unsigned int man_depress_check = 0;
unsigned int mandepress_btncheck = 0;
unsigned long btncount = 0;


int main (void){
	board_init();
	
	StartADC_Sequencers(); //start ADC conversion
	ReadADC_Sequencers(); //read conversion results	
	
//	error_msg |= wdt_scheduler(); //start watchdog timer
	//comment out for debugging (debugger is supposed to disable wdt automatically but it doesn't always)
	
	// The following block is for initializing any LED structs that one wishes to use:
	//******************************************************************************************************//
	struct LED LED_mandepress;
	struct LED *LED_mandepress_ptr = &LED_mandepress;
	
	LED_mandepress.LEDpin = LED3;
	LED_mandepress.LEDstate = 0;
	LED_mandepress.LEDtimer = 0.0;
	//******************************************************************************************************//
	
	//Start of main loop
	while(1)
	{
		//read analog inputs every time the main loop runs
		StartADC_Sequencers(); //start another conversion
		ReadADC_Sequencers(); //read conversion
		
		//clear wdt value
		wdt_clear();
		//if code gets hung up wdt won't clear and a reset will occur
		
		if(!gpio_get_pin_value(MODEBTN1) && (fc_state == FC_STATE_STANDBY)) {
			btncount = millis();
			mandepress_btncheck = 1;
		}
		
		if(((millis() - btncount) >= 5000) && mandepress_btncheck) {
			man_depress_check = 1;
			LEDblink(LED_mandepress_ptr, 500);
		}
		
		error_msg |= FC_check_alarms(fc_state);
		
		if(error_msg)
		{
			if(past_fc_state)
			{
				
			}
			else
			{
				past_fc_state = fc_state; //fc_state before error is triggered
			}
			//fc_state = FC_STATE_ALARM;
		}
		
		//main state machine
		switch (fc_state)
		{
		case FC_STATE_STANDBY:
			fc_state = FC_standby(man_depress_check);
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
		
		case FC_STATE_REPRESSURIZE:
			fc_state = FC_repressurize();
			break;
			
		case FC_STATE_MANUAL_DEPRESSURIZE:
			fc_state = FC_manual_depressurize();
			break;
		}
		
	usart_can_bridge(fc_state, error_msg,past_fc_state);
			
	}
}