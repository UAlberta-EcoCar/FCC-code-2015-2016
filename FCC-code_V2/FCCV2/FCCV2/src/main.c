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

#include "asf.h"
#include "FuelCell_ADC.h"
#include "FuelCell_Functions.h"
#include "FuelCell_PWM.h"
#include "digital_IO_defs.h"
#include "FC_error_codes.h"

unsigned int error_msg;
unsigned int fc_state = FC_STATE_STANDBY;

unsigned int read_timer = 0;
#define READ_TIME_INTERVAL 10

unsigned int data_log_timer = 0;
#define DATA_LOG_INTERVAL 200

int main (void)
{
	board_init();
		
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
		if(millis() - data_log_timer > DATA_LOG_INTERVAL)
		{
			//send data over CAN-BUS
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