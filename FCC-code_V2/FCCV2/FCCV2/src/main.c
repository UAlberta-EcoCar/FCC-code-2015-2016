/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a> //this site is useless
 */

/*
to do list:
get good adc conversions (test readings)
temp conversion
can bus
test fan pid control code
test test test
*/

//change this file depending on whether you are using the test bench or not
//
#include "FuelCell_mode_Select.h"

#include <stdlib.h>
#include <stdio.h>
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

char str [20]; //buffer for storing strings. set to length of longest string needed to avoid wasting ram

unsigned int error_msg;
unsigned int fc_state = FC_STATE_STANDBY;

unsigned int data_log_stagger = 0;
unsigned int data_log_timer;
#define DATA_LOG_INTERVAL 250


int main (void)
{
	board_init();
	
	//zero readings
	StartADC_Sequencers(); //start another conversion
	ReadADC_Sequencers(); //read conversion	
	//zero_CAPVOLT(); Not a good idea if caps have left over voltage
	zero_FCCURR(); //test bench mode: make sure this is zeroed at 5 V
	//zero_FCVOLT(); Not a good idea either
	
	error_msg |= wdt_scheduler(); //start watchdog timer
	//comment out for debugging
		
	//Start of main loop
	while(1)
	{
		//read analog inputs
		StartADC_Sequencers(); //start another conversion
		ReadADC_Sequencers(); //read conversion
		
		//clear wdt value
		//if code gets hung up wdt won't clear and a reset will occur
		wdt_clear();
		
		error_msg = FC_check_alarms(fc_state);
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
		
		
		//all data logging occurs after state machine executes
		//that way if alarm is triggered it is dealt with before
		//sending data over serial
		//stagger messages to be able to continue checking levels
		//without a huge serial delay (every message is about a millisecond)
		
		switch (data_log_stagger)
		{
			case 0:
			if(millis()- data_log_timer > DATA_LOG_INTERVAL)
			{
				data_log_stagger = 1;
				data_log_timer = millis();
				sprintf(str,"\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r");
				usart_write_line(EXAMPLE_USART,str);
				if(error_msg)
				{
					sprintf(str,"ERROR %d\n\r",error_msg);
					usart_write_line(EXAMPLE_USART,str);
				}
				sprintf(str,"LASTPURGE %d\n\r",get_time_between_last_purges());
				usart_write_line(EXAMPLE_USART,str);
				sprintf(str,"STATE %d\n\r",fc_state);
				usart_write_line(EXAMPLE_USART,str);
				sprintf(str,"\n\r");
				usart_write_line(EXAMPLE_USART,str);
			}
			break;
			
			case 1:
			sprintf(str,"FCVOLT %d\n\r",get_FCVOLT());
			usart_write_line(EXAMPLE_USART,str);
			sprintf(str,"FCCURR %d\n\r",get_FCCURR());
			usart_write_line(EXAMPLE_USART,str);
			sprintf(str,"FCPRES %d\n\r",get_FCPRES());
			usart_write_line(EXAMPLE_USART,str);
			sprintf(str,"\n\r");
			usart_write_line(EXAMPLE_USART,str);
			data_log_stagger = 2;
			break;
			
			case 2:
			sprintf(str,"FCTEMP1 %d\n\r",get_FCTEMP1());
			usart_write_line(EXAMPLE_USART,str);
			sprintf(str,"FCTEMP2 %d\n\r",get_FCTEMP2());
			usart_write_line(EXAMPLE_USART,str);
			sprintf(str,"FANSpeed %d\n\r",get_FANSpeed());
			usart_write_line(EXAMPLE_USART,str);
			sprintf(str,"\n\r");
			usart_write_line(EXAMPLE_USART,str);
			data_log_stagger = 3;
			break;
			
			case 3:
			/* AMBTEMP not connected
			sprintf(str,"AMBTEMP0 %d\n\r",get_AMBTEMP0());
			usart_write_line(EXAMPLE_USART,str);
			sprintf(str,"AMBTEMP1 %d\n\r",get_AMBTEMP1());
			usart_write_line(EXAMPLE_USART,str);
			sprintf(str,"AMBTEMP2 %d\n\r",get_AMBTEMP2());
			usart_write_line(EXAMPLE_USART,str);
			sprintf(str,"AMBTEMP3 %d\n\r",get_AMBTEMP3());
			usart_write_line(EXAMPLE_USART,str);
			sprintf(str,"\n\r\n\r\n\r");
			usart_write_line(EXAMPLE_USART,str);
			*/
			data_log_stagger = 4;
			break;
			
			case 4:
			sprintf(str,"CAPVOLT %d\n\r",get_CAPVOLT());
			usart_write_line(EXAMPLE_USART,str);
			sprintf(str,"\n\r");
			usart_write_line(EXAMPLE_USART,str);
			data_log_stagger = 0;
			break;
		}
	}
}