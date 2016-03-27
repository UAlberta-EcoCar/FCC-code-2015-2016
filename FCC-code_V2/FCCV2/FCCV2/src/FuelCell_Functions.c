/*
 * FuelCell_Functions.c
 *
 * Created: 2015-11-09 11:07:53 AM
 *  Author: Reegan
 */ 

#include "asf.h"
#include "FuelCell_Functions.h"
#include "FC_error_codes.h"
#include "digital_IO_defs.h"
#include "FuelCell_ADC.h"
#include "FuelCell_PWM.h"
#include <math.h>
#include "pid.h"
#include "millis_function.h"

unsigned long delay_timer1;
unsigned long delay_timer2;

unsigned int FC_standby(void)
{
	unsigned int fc_state;
	if (gpio_get_pin_value(START))
	{
		fc_state = FC_STATE_STARTUP_FANS;
		gpio_clr_gpio_pin(LED_STOP);
		gpio_set_gpio_pin(LED_START);
	}
	else
	{
		//make sure fuel cell stays off
		//Supply valve closed
		gpio_clr_gpio_pin(H2_VALVE);
		//purge valve closed
		gpio_clr_gpio_pin(PURGE_VALVE);
		//relays open
		gpio_clr_gpio_pin(MOTOR_RELAY);
		gpio_clr_gpio_pin(RES_RELAY);
		gpio_clr_gpio_pin(CAP_RELAY);
		//led's off
		gpio_clr_gpio_pin(LED_RUN);
		gpio_clr_gpio_pin(LED_START);
		gpio_clr_gpio_pin(LED_STOP);
		//fan low
		FANUpdate(0);
		fc_state = FC_STATE_STANDBY;
	}
	return(fc_state);
}

//this function doesn't work
unsigned int tachometer_test = 0;
unsigned int FC_startup_fans(void)
{
	unsigned int fc_state;
	//relays open
	gpio_clr_gpio_pin(START_RELAY);
	gpio_clr_gpio_pin(MOTOR_RELAY);
	gpio_clr_gpio_pin(RES_RELAY);
	gpio_clr_gpio_pin(CAP_RELAY);
	//valves closed
	gpio_clr_gpio_pin(H2_VALVE);
	gpio_clr_gpio_pin(PURGE_VALVE);
	
	//set fans to min
	FANUpdate(0);
	//read fan tachometer to ensure fans are spinning
	//increment if tach is 0 and reads 1
	if(tachometer_test == 0)
	{
		if(gpio_get_pin_value(FAN_TACH)==1)
		{
			tachometer_test = 1;
		}
	}
	fc_state = FC_STATE_STARTUP_FANS; //keep looping in this function
	
	//then wait for it to go low again (then the fan is spinning)
	if(tachometer_test == 1)
	{
		if(gpio_get_pin_value(FAN_TACH) == 0)
		{
			//fan is spinning go to startup
			fc_state = FC_STATE_STARTUP_H2;
		}
	}
	return(FC_STATE_STARTUP_H2); //don't run this function again it is broken
}


unsigned int FC_startup_h2(void)
{
	unsigned int fc_state;
	
	//open h2 valve
	gpio_set_gpio_pin(H2_VALVE);
	//purge valve closed
	gpio_clr_gpio_pin(PURGE_VALVE);
	//relays open
	gpio_clr_gpio_pin(START_RELAY);
	gpio_clr_gpio_pin(MOTOR_RELAY);
	gpio_clr_gpio_pin(RES_RELAY);
	gpio_clr_gpio_pin(CAP_RELAY);
	
	//input h2 until voltage reaches 30
	if (get_FCVOLT() < 30000) //if voltage is less than 30V
	{
		//keep the hydrogen coming
		fc_state = FC_STATE_STARTUP_H2;
	}
	else
	{
		//voltage is 30 then go to start up purge
		fc_state = FC_STATE_STARTUP_PURGE;
	}
	return(fc_state);
}


unsigned int purge_timer = 0; //used for timing how long purge valve is open
unsigned int FC_startup_purge(void)
{
	unsigned int fc_state;
	
	//h2 valve still open
	gpio_set_gpio_pin(H2_VALVE);
	//close startup relay
	if((gpio_get_gpio_pin_output_value(RES_RELAY)|gpio_get_gpio_pin_output_value(CAP_RELAY))==0)
		{
			gpio_set_gpio_pin(START_RELAY);
		}
	//motor relay still open
	gpio_clr_gpio_pin(MOTOR_RELAY);
	//RES relay open
	gpio_clr_gpio_pin(RES_RELAY);
	//CAP relay still open
	gpio_clr_gpio_pin(CAP_RELAY);
	
	//open purge valve and start timer
	if(gpio_get_gpio_pin_output_value(PURGE_VALVE) == 0)
	{
		purge_timer = millis();
	}

	gpio_set_gpio_pin(PURGE_VALVE);
	gpio_set_gpio_pin(LED0);
	
	//balazs has pseudo code purge for 3 seconds
	if(millis() - purge_timer < 3000)
	{
		fc_state = FC_STATE_STARTUP_PURGE;
	}
	else //3 seconds are over
	{
		//close purge valve
		gpio_clr_gpio_pin(PURGE_VALVE);
		gpio_clr_gpio_pin(LED0);
		
		//supply valve still open
		gpio_set_gpio_pin(H2_VALVE);
		
		//open start up relay
		gpio_clr_gpio_pin(START_RELAY);
		
		//other relays still open
		gpio_clr_gpio_pin(RES_RELAY);
		gpio_clr_gpio_pin(MOTOR_RELAY);
		gpio_clr_gpio_pin(CAP_RELAY);
		
		delay_timer1 = millis(); //reset delay timer to have delay at start of charge
		
		//go to charge state
		fc_state = FC_STATE_STARTUP_CHARGE;
	}
	return(fc_state);
}

unsigned int total_charge_energy_integration_timer;

unsigned int time_between_last_purges;
unsigned int get_time_between_last_purges(void)
{
	return(time_between_last_purges);
}

unsigned int purge_counter = 0;
unsigned int get_number_of_purges(void)
{
	return(purge_counter);
}

U64 estimated_total_charge_extracted = 0;
U64 get_total_charge_extracted(void)
{
	return(estimated_total_charge_extracted / 1000);
}

U64 uJ_since_last_purge;
U64 get_J_since_last_purge(void)
{
	return(uJ_since_last_purge / 1000 / 1000);
}

U64 estimated_total_E;
U64 get_total_E(void)
{
	return(estimated_total_E / 1000);
}

unsigned int purge_integration_timer; //using for integrating time between purges 
unsigned int delta_purge_time; //used for calculating integration interval
U64 mAms_since_last_purge; //sum of coulombs since last purge

U64 get_coulumbs_since_last_purge(void)
{
	return(mAms_since_last_purge/1000/1000);
}

unsigned int time_since_last_purge; //keep track of time between purges
unsigned int purge_state = FIRST_PURGE_CYCLE; //used for keeping track of switching b/w purge valve open closed
unsigned int fan_update_timer; //used for timing pwm code
unsigned int charge_thres = 35000;
unsigned int FC_startup_charge(void)
{
	unsigned int fc_state = FC_STATE_STARTUP_CHARGE; //will keep charging until state exits
	
	if(millis() - delay_timer1 < 1000)
	{
		return(fc_state);
	}
	//this won't run until above timer times out
	
	if(purge_state == FIRST_PURGE_CYCLE)
	{
		purge_integration_timer = millis(); //start timing
		purge_state = PURGE_VALVE_CLOSED; //fuel cell running purge valve closed //prevents timer from being reset
	}

	//keep track of coulombs of charge produced since last purge
	//also energy
	delta_purge_time = millis() - purge_integration_timer;
	if(delta_purge_time > PURGE_INTEGRATION_INTERVAL)
	{
		mAms_since_last_purge += delta_purge_time * get_FCCURR();
		time_since_last_purge += delta_purge_time;
		uJ_since_last_purge += delta_purge_time * get_FCCURR() * get_FCVOLT() / 1000;
		purge_integration_timer = millis();
	}
	
	if (mAms_since_last_purge > PURGE_THRESHOLD) //time to purge
	{
		//open purge valve
		gpio_set_gpio_pin(PURGE_VALVE);
		
		purge_counter++; //increment number of purges
		
		//we restart counting mAms as soon as valve opens
		//reset mAms sum
		mAms_since_last_purge = 0;
		
		//reset energy
		uJ_since_last_purge = 0; 
		
		//record time
		time_between_last_purges = time_since_last_purge;
		time_since_last_purge = 0; //reset timer
		//reset timer
		purge_integration_timer = millis();
		
		//start purge timer to time purge
		purge_timer = millis();
		purge_state = PURGE_VALVE_OPEN; //purge valve open
		//set led0 on because why not
		gpio_set_gpio_pin(LED0);
	}
	
	if(purge_state == PURGE_VALVE_OPEN) //if purge valve is open
	{
		if(millis() - purge_timer > PURGE_TIME) //if it has completed purge 
		{
			//close purge valve
			gpio_clr_gpio_pin(PURGE_VALVE);
			purge_state = PURGE_VALVE_CLOSED; //purge valve closed
			
			//turn led0 off
			gpio_clr_gpio_pin(LED0);
		}
	}
		
	//pid fan control to maintain temperature
	if(millis() - fan_update_timer > FANUPDATE_INTERVAL)
	{
		//pid fan control for temp regulation
		FANUpdate(PID( get_FCTEMP() , ((53*get_FCCURR()) / 100 + 299160)));
		fan_update_timer = millis();
	}
	
	//energy and charge integration for datalogging
	unsigned int delta_t = millis() - total_charge_energy_integration_timer;
	if(delta_t > TOTAL_CHARGE_ENERGY_INTEGRATION_INTERVAL)
	{
		estimated_total_E += get_FCVOLT() * get_FCCURR() * delta_t / 1000 / 1000;
		estimated_total_charge_extracted += get_FCCURR() * delta_t / 1000;
		total_charge_energy_integration_timer = millis();
	}
	
	//charging capacitors through resistor to avoid temporary short circuit
	if (get_CAPVOLT() < charge_thres) //if voltage is less than 35V
	{
		//close resistor relay
		gpio_set_gpio_pin(RES_RELAY);
		gpio_set_gpio_pin(LED1); //make led1 indicate charging state?
		//other relays open still
		gpio_clr_gpio_pin(START_RELAY);
		gpio_clr_gpio_pin(MOTOR_RELAY);
		gpio_clr_gpio_pin(CAP_RELAY);
		//H2_valve open
		gpio_set_gpio_pin(H2_VALVE);
		//purge valve still closed
		//gpio_clr_gpio_pin(PURGE_VALVE);
		//fc_State still charge
		delay_timer2 = millis();
	}
	else //caps are charged
	{
		charge_thres = 30000; //set threshold low to stop above if from running
		//turn off led1
		gpio_clr_gpio_pin(LED1);
		
		//start relay still open
		gpio_clr_gpio_pin(START_RELAY);
		
		//open resistor relay
		gpio_clr_gpio_pin(RES_RELAY);
		
		if(millis() - delay_timer2 < 2000)
		{
			return(fc_state);
		}
		
		gpio_set_gpio_pin(CAP_RELAY);
		
		if(millis() - delay_timer2 < 4000)
		{
			return(fc_state);
		}
		
		//close motor relay
		gpio_set_gpio_pin(MOTOR_RELAY);
		
		if(millis() - delay_timer2 < 6000)
		{
			return(fc_state);
		}
		
		//go to main run state
		fc_state = FC_STATE_RUN;
		gpio_clr_gpio_pin(LED_START);
		gpio_set_gpio_pin(LED_RUN);
		
		if(millis() - delay_timer2 < 8000)
		{
			return(fc_state);
		}
		//send some sort of START signal over can
	}
	return(fc_state);
}

//optimum temp changes with current draw
int calc_opt_temp(void)
{
	return((53*get_FCCURR()) / 100 + 299160);
}
//minimum current changes with current draw
int calc_min_temp(void)
{
	//this is a very rough approximation
	return((53*get_FCCURR()) / 100 + 279248);
}
//maximum current changes with current draw
int calc_max_temp(void)
{
	return((355*get_FCCURR())/1000 + 325150);
}

unsigned int fan_update_timer;
unsigned int FC_run(void)
{
	unsigned int fc_state;
	if(millis() - delay_timer2 < 12000)
	{
		return(fc_state);
	}
	//pid fan control to maintain temperature
	if(millis() - fan_update_timer > FANUPDATE_INTERVAL)
	{
		//pid fan control for temp regulation
		FANUpdate(PID(get_FCTEMP() , calc_opt_temp()));
		fan_update_timer = millis();
	}
	
	//purge control: //purge based on amount of charge extracted from hydrogen (1 C = amp * sec)
	delta_purge_time = millis() - purge_integration_timer;
	if(delta_purge_time > PURGE_INTEGRATION_INTERVAL)
	{
		mAms_since_last_purge += delta_purge_time * (get_FCCURR());
		uJ_since_last_purge += delta_purge_time * get_FCCURR() * get_FCVOLT() / 1000;
		purge_integration_timer = millis();
		time_since_last_purge += delta_purge_time;
	}
	
	if (mAms_since_last_purge > PURGE_THRESHOLD) //time to purge
	{
		//open purge valve
		gpio_set_gpio_pin(PURGE_VALVE);
		
		purge_counter++; //increment number of purges
				
		//we restart counting mAms as soon as valve opens
		//reset mAms sum
		mAms_since_last_purge = 0;
		
		//reset energy
		uJ_since_last_purge = 0; 
		
		//record time
		time_between_last_purges = time_since_last_purge;
		time_since_last_purge = 0; //reset timer		
		//reset timer
		purge_integration_timer = millis();
		
		//start purge timer to time purge
		purge_timer = millis();
		purge_state = PURGE_VALVE_OPEN; //purge valve open
		//set led0 on because why not
		gpio_set_gpio_pin(LED0);
	}
	
	if(purge_state == PURGE_VALVE_OPEN) //if purge valve is open
	{
		if(millis() - purge_timer > PURGE_TIME) //if it has completed purge //is there some way to use flow meter instead of a fixed time?
		{
			//close purge valve
			gpio_clr_gpio_pin(PURGE_VALVE);
			purge_state = PURGE_VALVE_CLOSED; //purge valve closed
			
			//turn led0 off
			gpio_clr_gpio_pin(LED0);
		}
	}
	
	//energy and charge integration for datalogging
	unsigned int delta_t = millis() - total_charge_energy_integration_timer;
	if(delta_t > TOTAL_CHARGE_ENERGY_INTEGRATION_INTERVAL)
	{
		estimated_total_E += get_FCVOLT() * get_FCCURR() * delta_t / 1000 / 1000;
		estimated_total_charge_extracted += get_FCCURR() * delta_t / 1000;
		total_charge_energy_integration_timer = millis();
	}
	
	fc_state = FC_STATE_RUN;
	return(fc_state);
}


unsigned int FC_shutdown(void)
{
	unsigned int fc_state;
	gpio_clr_gpio_pin(LED_START);
	gpio_clr_gpio_pin(LED_RUN);
	gpio_set_gpio_pin(LED_STOP);
	//close valves
	gpio_clr_gpio_pin(H2_VALVE);
	gpio_clr_gpio_pin(PURGE_VALVE);
	//close relays
	gpio_clr_gpio_pin(MOTOR_RELAY);
	gpio_clr_gpio_pin(START_RELAY);
	gpio_clr_gpio_pin(RES_RELAY);
	gpio_clr_gpio_pin(CAP_RELAY);
	//fans to max
	FANUpdate(1024);	
	if(0)
	{
		//option of leaving shutdown state and restarting
		fc_state = FC_STATE_STANDBY;
	}
	else
	{
		fc_state = FC_STATE_SHUTDOWN;
	}
	return(fc_state);
}

unsigned int FC_alarm(void)
{
	unsigned int fc_state;
	gpio_clr_gpio_pin(LED_RUN);
	gpio_set_gpio_pin(LED_ERROR);
	gpio_set_gpio_pin(LED_STOP);
	//close valves
	gpio_clr_gpio_pin(H2_VALVE);
	gpio_clr_gpio_pin(PURGE_VALVE);
	//close relays
	gpio_clr_gpio_pin(MOTOR_RELAY);
	gpio_clr_gpio_pin(START_RELAY);
	gpio_clr_gpio_pin(RES_RELAY);
	gpio_clr_gpio_pin(CAP_RELAY);
	FANUpdate(1024);
	fc_state = FC_STATE_ALARM;
	//manual reset required to exit alarm state
	return(fc_state);
}