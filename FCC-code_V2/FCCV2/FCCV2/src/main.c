/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a> //this site is useless
 */

/*
to do list:
get good temperature conversion (actually test readings)
can bus
serial
fan pid control code
test

*/

#include <stdlib.h>
#include <stdio.h>
#include "asf.h"
#include "FuelCell_ADC.h"
#include "FuelCell_Functions.h"
#include "FuelCell_PWM.h"
#include "digital_IO_defs.h"
#include "FC_error_codes.h"
#include "FuelCell_USART.h"

unsigned int error_msg;
unsigned int fc_state = FC_STATE_STANDBY;

unsigned int read_timer = 0;
#define READ_TIME_INTERVAL 10

unsigned int data_log_stagger = 0;
unsigned int data_log_timer = 0;
#define DATA_LOG_INTERVAL 250

int main (void)
{
	board_init();
	USARTInit();
	//Start of main loop
	while(1)
	{
		//read analog inputs
		StartADC_Sequencers();
		ReadADC_Sequencers();
		read_timer = millis();

		
		if(1)
		{
			error_msg = FC_check_alarms(fc_state);
		}
		
		if(error_msg)
		{
			fc_state = FC_STATE_ALARM;
		}
		
		char str [20]; //buffer for storing strings
		//send data over serial
		//stagger messages to be able to continue checking levels
		//to do: different values have different priorities
		//use that instead of this wierd switch
		switch (data_log_stagger)
		{
			case 0:
				if(millis()- data_log_timer > DATA_LOG_INTERVAL)
				{
					data_log_stagger = 1;
					data_log_timer = millis();				}
			case 1:
				sprintf(str,"FCVOLT %d\n",get_FCVOLT());
				usart_write_line(EXAMPLE_USART,str);
				sprintf(str,"FCCURR %d\n",get_FCCURR());
				usart_write_line(EXAMPLE_USART,str);
				sprintf(str,"FCPRES %d\n",get_FCPRES());
				usart_write_line(EXAMPLE_USART,str);
				data_log_stagger = 2;
			case 2:
				sprintf(str,"FCTEMP1 %d\n",get_FCTEMP1());
				usart_write_line(EXAMPLE_USART,str);
				sprintf(str,"FCTEMP2 %d\n",get_FCTEMP2());
				usart_write_line(EXAMPLE_USART,str);
				data_log_stagger = 3;
			case 3:
				sprintf(str,"AMBTEMP0 %d\n",get_AMBTEMP0());
				usart_write_line(EXAMPLE_USART,str);
				sprintf(str,"AMBTEMP1 %d\n",get_AMBTEMP1());
				usart_write_line(EXAMPLE_USART,str);
				sprintf(str,"AMBTEMP2 %d\n",get_AMBTEMP2());
				usart_write_line(EXAMPLE_USART,str);
				sprintf(str,"AMBTEMP3 %d\n",get_AMBTEMP3());
				usart_write_line(EXAMPLE_USART,str);
				data_log_stagger = 4;
			case 4:
				sprintf(str,"CAPVOLT %d\n",get_CAPVOLT());
				usart_write_line(EXAMPLE_USART,str);
				data_log_stagger = 0;
		}
		
		
		
		//main state machine
		switch (fc_state)
		{
		case FC_STATE_STANDBY:
			fc_state = FC_standby();
			
		case FC_STATE_SHUTDOWN:
			fc_state = FC_shutdown();
		
		case FC_STATE_STARTUP_FANS:
			fc_state = FC_startup_fans();
		
		case FC_STATE_STARTUP_H2:
			fc_state = FC_startup_h2();
			
		case FC_STATE_STARTUP_PURGE:
			fc_state = FC_startup_purge();
		
		case FC_STATE_STARTUP_CHARGE:
			fc_state = FC_startup_charge();	
		
		case FC_STATE_RUN:
			fc_state = FC_run();
			
		case FC_STATE_ALARM:
			fc_state = FC_alarm();			
		}
	}
}