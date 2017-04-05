/*
to do list:
Set up an air starve mode (new run mode)
Update microcontroller pins
program the buttons to choose run modes, as well as LEDs to indicate run mode

Air Starve Mode:
	- Disable alarm conditions
	- Turn off fans
	
Manual Depressurize:
	- Close supply valve
	- Open purge valve only for 500 ms
	- Open relays

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


unsigned int error_msg; // Used to check if error occurs
unsigned int fc_state = FC_STATE_STANDBY;
unsigned int past_fc_state = 0;
unsigned int man_depress_check = 0;
unsigned int air_starve_check = 0;
unsigned long btncount = 0;
unsigned long air_starve_LED_time = 0;
int main (void){
	board_init();
	
	StartADC_Sequencers(); //start ADC conversion
	ReadADC_Sequencers(); //read conversion results	
	
//	error_msg |= wdt_scheduler(); //start watchdog timer
	//comment out for debugging (debugger is supposed to disable wdt automatically but it doesn't always)

int led_timer2 = millis();
int led_state = 0;
while (1) {
	StartADC_Sequencers(); //start another conversion
	ReadADC_Sequencers(); //read conversion
	
	switch(millis() - led_timer2)
	{
		
		case 0:
		gpio_set_gpio_pin(LED0);
		break;
		case 250:
		gpio_clr_gpio_pin(LED0);
		gpio_set_gpio_pin(LED1);
		break;
		case 500:
		gpio_clr_gpio_pin(LED1);
		gpio_set_gpio_pin(LED2);
		break;
		case 750:
		gpio_clr_gpio_pin(LED2);
		gpio_set_gpio_pin(LED3);
		break;
		case 1000:
		gpio_clr_gpio_pin(LED3);
		gpio_set_gpio_pin(LED_RUN);
		break;
		case 1250:
		gpio_clr_gpio_pin(LED_RUN);
		gpio_set_gpio_pin(LED_START);
		break;
		case 1500:
		gpio_clr_gpio_pin(LED_START);
		gpio_set_gpio_pin(LED_STOP);
		break;
		case 1750:
		gpio_clr_gpio_pin(LED_STOP);
		gpio_set_gpio_pin(LED_ERROR);
		break;	
		case 2000:
		gpio_clr_gpio_pin(LED_ERROR);
		gpio_set_gpio_pin(LED_STAT1);
		break;
		case 2250:
		gpio_clr_gpio_pin(LED_STAT1);
		gpio_set_gpio_pin(LED_STAT2);
		break;
		case 2500:
		gpio_clr_gpio_pin(LED_STAT2);
		gpio_set_gpio_pin(LED_STAT3);
		break;
		case 2750:
		gpio_clr_gpio_pin(LED_STAT3);
		gpio_set_gpio_pin(LED_STAT4);
		break;
		case 3000:
		gpio_clr_gpio_pin(LED_STAT4);
		gpio_set_gpio_pin(LED_STAT3);
		break;
		case 3250:
		gpio_clr_gpio_pin(LED_STAT3);
		gpio_set_gpio_pin(LED_STAT2);
		break;
		case 3500:
		gpio_clr_gpio_pin(LED_STAT2);
		gpio_set_gpio_pin(LED_STAT1);
		break;
		case 3750:
		gpio_clr_gpio_pin(LED_STAT1);
		gpio_set_gpio_pin(LED_ERROR);
		break;
		case 4000:
		gpio_clr_gpio_pin(LED_ERROR);
		gpio_set_gpio_pin(LED_STOP);
		break;
		case 4250:
		gpio_clr_gpio_pin(LED_STOP);
		gpio_set_gpio_pin(LED_START);
		break;
		case 4500:
		gpio_clr_gpio_pin(LED_START);
		gpio_set_gpio_pin(LED_RUN);
		break;
		case 4750:
		gpio_clr_gpio_pin(LED_RUN);
		gpio_set_gpio_pin(LED3);
		break;
		case 5000:
		gpio_clr_gpio_pin(LED3);
		gpio_set_gpio_pin(LED2);
		break;
		case 5250:
		gpio_clr_gpio_pin(LED2);
		gpio_set_gpio_pin(LED1);
		break;
		case 5500:
		gpio_clr_gpio_pin(LED1);
		gpio_set_gpio_pin(LED0);
		break;
		case 5750:
		gpio_clr_gpio_pin(LED0);
		led_timer2 = millis();
		break;
	
	}

}
	//Start of main loop
	while(0)
	{
		//read analog inputs every time the main loop runs
		StartADC_Sequencers(); //start another conversion
		ReadADC_Sequencers(); //read conversion
		
		//clear wdt value
		wdt_clear();
		//if code gets hung up wdt won't clear and a reset will occur
		
		// The following code checks to see if the button 1 (corresponding to manual depressurize) has been pressed for 5 seconds
		
		if(gpio_get_pin_value(MODEBTN1) && (btncount == 0) && (fc_state == FC_STATE_STANDBY))
		{
			
			btncount = millis();
			
		}
		
		else if(!gpio_get_pin_value(MODEBTN1))
		{
			
			btncount = 0;
			
		}
		
		if((millis() - btncount >= 5000) && (btncount != 0)) 
		{
			
			man_depress_check = 1;
			air_starve_check = 0;
			btncount = 0;
			
		}

		// The following code checks that button 2 has been pressed for 5 seconds
		
		if(gpio_get_pin_value(MODEBTN2) && (btncount == 0) && (fc_state == FC_STATE_STANDBY))
		{
			
			btncount = millis();
			
		}
		
		else if(!gpio_get_pin_value(MODEBTN2))
		{
			
			btncount = 0;
			
		}
		
		if((millis() - btncount >= 5000) && (btncount != 0)) 
		{
			
			man_depress_check = 0;
			air_starve_check = 1;
			btncount = 0;
			
		}
		
		// The following causes the air starve LED to blink
		
		if ((air_starve_check) || (fc_state == FC_STATE_AIR_STARVE))
		{
			if (air_starve_LED_time == 0) // First time
			{
				air_starve_LED_time = millis();
			}
			if (millis() - air_starve_LED_time >= 250) // Set a blink rate of 250 ms 
			{
				if (gpio_get_pin_value(LED_STAT1) == 1) // If on turn off, reset timer
				{
					gpio_clr_gpio_pin(LED_STAT1);
					air_starve_LED_time = millis();
				}
				else if (gpio_get_pin_value(LED_STAT1) == 0) // If off turn on, reset timer don't need
				{
					gpio_set_gpio_pin(LED_STAT1);
					air_starve_LED_time = millis();
				}
			}
		}
		else if ((~air_starve_check) && (fc_state != FC_STATE_AIR_STARVE))
		{
			gpio_clr_gpio_pin(LED_STAT1);
		}
			
		error_msg |= FC_check_alarms(fc_state); // Becomes true if FC_check_alarms returns true, and stays true
		
		if(error_msg)
		{
			if(past_fc_state)
			{
				
			}
			else
			{
				past_fc_state = fc_state; //fc_state before error is triggered
			}
			fc_state = FC_STATE_ALARM;
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
			fc_state = FC_startup_charge(air_starve_check);	
			break;
			
		case FC_STATE_RUN:
			fc_state = FC_run();
			break;
			
		case FC_STATE_AIR_STARVE:
			fc_state = FC_air_starve();
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