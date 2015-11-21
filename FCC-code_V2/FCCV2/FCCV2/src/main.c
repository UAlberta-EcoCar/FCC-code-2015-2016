/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

/*
to do list:
get good temperature conversion
current conversion
voltage conversions

*/

#include "asf.h"
#include "FuelCell_ADC.h"
#include "FuelCell_Functions.h"
#include "FuelCell_PWM.h"
#include "digital_IO_defs.h"
#include "FC_error_codes.h"

unsigned int error_msg;
unsigned int fc_state = FC_STATE_STANDBY;

int main (void)
{
	board_init();
		
	//Start of main loop
	while(1)
	{
		//read analog inputs
		StartADC_Sequencers();
		ReadADC_Sequencers();
		
		error_msg = FC_check_alarms(fc_state);
		if(error_msg)
		{
			fc_state = FC_STATE_SHUTDOWN;
			gpio_clr_gpio_pin(LED_RUN);
			gpio_set_gpio_pin(LED_ERROR);
			gpio_set_gpio_pin(LED_STOP);
		}
		
		switch (fc_state)
		{
		case FC_STATE_STANDBY:
			fc_state = FC_standby();
			
		case FC_STATE_SHUTDOWN:
			fc_state = FC_shutdown();
		
		case FC_STATE_STARTUP_H2:
			fc_state = FC_startup_h2();
			
		case FC_STATE_STARTUP_PURGE:
			fc_state = FC_startup_purge();
		
		case FC_STATE_STARTUP_CHARGE:
			fc_state = FC_startup_charge();	
		
		case FC_STATE_RUN:
			fc_state = FC_run();
		
		case FC_STATE_RUN_PURGE:
			fc_state = FC_run_purge();
		}
	}
}