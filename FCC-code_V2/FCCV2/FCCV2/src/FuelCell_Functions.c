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

//create value for timer
unsigned long counta;
void millis_init(void)
{
	//AST generic clock 8MHz / (2*(249+1) = 16kHz
	scif_gc_setup(AVR32_SCIF_GCLK_AST,SCIF_GCCTRL_RC8M,AVR32_SCIF_GC_DIV_CLOCK,249);
	// Now enable the generic clock
	scif_gc_enable(AVR32_SCIF_GCLK_AST);
	//set up timer
	ast_init_counter(&AVR32_AST,AST_OSC_GCLK,4,counta);  //16kHz / (2^(4+1)) = 1kHz
	ast_enable(&AVR32_AST);
}

unsigned long millis(void)
{
	return(ast_get_counter_value(&AVR32_AST)); 
}

unsigned int TEMP_OPT;
unsigned int AVE_TEMP; 
unsigned int temp_error;
//float accumulated_temp_error = 0;
//unsigned int PIDP; //speed proportional to temp error
//unsigned int PIDI; //integral -> accumulated error
//unsigned int PIDD; //Derivative -> change in temp
//I'm only going to make speed depend on temp error
unsigned int pid_temp_control(void)
{
	//Topt = 0.53I + 26.01 in C and Amps
	//= (53 * I) / 100 + 299160 in mK and mA
	TEMP_OPT = (53*get_FCCURR()) / 100 + 299160;
	AVE_TEMP = (get_FCTEMP1() + get_FCTEMP2())/2;
	
	temp_error = (TEMP_OPT - AVE_TEMP) / 1000; //in Kalvin
	//accumulated_temp_error = accumulated_temp_error + temp_error;
	
	//fan speed = 0 to 20 //should really increase this value
	//run fan at 1% above min per degree K it is above T_OPT
	FANUpdate(temp_error*20/100);
	return(1);
}

unsigned int FC_check_alarms(unsigned int fc_state)
{
	unsigned int error_msg = 0;
	//check fccon sysok capcon always
	if(gpio_get_pin_value(FCCON) == 0)
	{
		error_msg |= FC_ERR_FC_DISC;
	}
	if(gpio_get_pin_value(CAPCON) == 0)
	{
		error_msg |= FC_ERR_CAP_DISC;
	}
	if(gpio_get_pin_value(SYSOK) == 0)
	{
		error_msg |= FC_ERR_H2OK_LOW;
	}
	//check temp H L and pressure H always
	if((get_FCTEMP1() > HIGH_TEMP_THRES) | (get_FCTEMP2() > HIGH_TEMP_THRES))
	{
		error_msg |= FC_ERR_TEMP_H;
	}
	if((get_FCTEMP1() < LOW_TEMP_THRES) | (get_FCTEMP2() < LOW_TEMP_THRES))
	{
		error_msg |= FC_ERR_TEMP_L;
	}
	if(get_FCPRES() > FC_HIGH_PRES_THRES)
	{
		error_msg |= FC_ERR_PRES_H;
	}
	switch (fc_state)
	{
		//case FC_STATE_STANDBY:
		//case FC_STATE_SHUTDOWN:
		
		case FC_STATE_STARTUP_FANS:
			if(gpio_get_pin_value(RESCON) == 0)
			{
				error_msg |= FC_ERR_RES_DISC;
			}
		
		case FC_STATE_STARTUP_H2:
			if(gpio_get_pin_value(RESCON) == 0)
			{
				error_msg |= FC_ERR_RES_DISC;
			}	
		
		case FC_STATE_STARTUP_PURGE:
			if(gpio_get_pin_value(RESCON) == 0)
			{
				error_msg |= FC_ERR_RES_DISC;
			}
			if(get_FCPRES() < FC_LOW_PRES_THRES)
			{
				error_msg |= FC_ERR_PRES_L;
			}	
			if(get_FCCURR() < UNDER_CUR_THRES)
			{
				error_msg |= FC_ERR_UND_CUR;
			}
			if(get_FCCURR() > OVER_CUR_THRES)
			{
				error_msg |= FC_ERR_OVER_CUR;
			}
			
		
		case FC_STATE_STARTUP_CHARGE:
			if(gpio_get_pin_value(RESCON) == 0)
			{
				error_msg |= FC_ERR_RES_DISC;
			}
			
		case FC_STATE_RUN:
			if(get_FCPRES() < FC_LOW_PRES_THRES)
			{
				error_msg |= FC_ERR_PRES_L;
			}
			if(get_FCCURR() < UNDER_CUR_THRES)
			{
				error_msg |= FC_ERR_UND_CUR;
			}
			if(get_FCCURR() > OVER_CUR_THRES)
			{
				error_msg |= FC_ERR_OVER_CUR;
			}
			
		case FC_STATE_ALARM:
			if(get_FCPRES() < FC_LOW_PRES_THRES)
			{
				error_msg |= FC_ERR_PRES_L;
			}
			if(get_FCCURR() < UNDER_CUR_THRES)
			{
				error_msg |= FC_ERR_UND_CUR;
			}
			if(get_FCCURR() > OVER_CUR_THRES)
			{
				error_msg |= FC_ERR_OVER_CUR;
			}
	}

	return(error_msg);
}

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

unsigned int tachometer_test = 0;
unsigned int FC_startup_fans(void)
{
	unsigned int fc_state;
	//relays open
	gpio_clr_gpio_pin(START_RELAY);
	gpio_clr_gpio_pin(MOTOR_RELAY);
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
	return(fc_state);
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
	
	//input h2 until voltage reaches 30
	if (get_FCVOLT() < (30000)) //if voltage is less than 30V
	{
		//keep the hydrogen coming
		fc_state = FC_STATE_STARTUP_H2;
	}
	else
	{
		//voltage is 30 then do start up purge
		fc_state = FC_STATE_STARTUP_PURGE;
	}
	return(fc_state);
}


unsigned int purge_timer = 0; //used for timing how long purge valve is open
unsigned int FC_startup_purge(void)
{
	//Purge step isn't on !Rescon to get air out of lines on startup. what???
	unsigned int fc_state;
	
	//h2 valve still open
	gpio_set_gpio_pin(H2_VALVE);
	//close startup relay
	gpio_set_gpio_pin(START_RELAY);
	//motor relay still open
	gpio_clr_gpio_pin(MOTOR_RELAY);
	//RES relay still open
	gpio_clr_gpio_pin(RES_RELAY);
	
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
		
		//open startup relay
		gpio_clr_gpio_pin(START_RELAY);
		//other relays still open
		fc_state = FC_STATE_STARTUP_CHARGE;
	}
	return(fc_state);
}


unsigned int time_between_last_purges;
unsigned int get_time_between_last_purges(void)
{
	return(time_between_last_purges);
}


unsigned int purge_integration_timer; //using for integrating time between purges 
unsigned int delta_purge_time;
U64 mAms_since_last_purge;
unsigned int time_since_last_purge; //keep track of time between purges
unsigned int purge_state = FIRST_PURGE_CYCLE;
unsigned int fan_update_timer;
unsigned int FC_startup_charge(void)
{
	unsigned int fc_state = FC_STATE_STARTUP_CHARGE; //will keep charging until state exits
	//Skip charge step if resistors aren't connected
	//pseudo code checked RESCON here
	//however, RESCON == 0 will trigger and alarm anyway so I won't check

	if(purge_state == FIRST_PURGE_CYCLE)
	{
		purge_integration_timer = millis(); //start timing
		purge_state = PURGE_VALVE_CLOSED; //fuel cell running purge valve closed //prevents timer from being reset
	}
	//keep track of coulombs of charge produced since last purge
	delta_purge_time = millis() - purge_integration_timer;
	if(delta_purge_time > PURGE_INTEGRATION_INTERVAL)
	{
		mAms_since_last_purge += delta_purge_time * get_FCCURR();
		time_since_last_purge += delta_purge_time;
		purge_integration_timer = millis();
	}
	//we shouldn't need to purge in this state because 2300C/40V = 57.5F of capacitance we can fill
	//but it still impacts time till next purge
	
	//pid fan control to maintain temperature
	if(millis() - fan_update_timer > FANUPDATE_INTERVAL)
	{
		//pid fan control for temp regulation
		FANUpdate(PID( ((get_FCTEMP1()+get_FCTEMP2())/2) , ((53*get_FCCURR()) / 100 + 299160) ));
		fan_update_timer = millis();
	}
	
	//charging capacitors through resistor to avoid temporary short circuit
	if (get_CAPVOLT() < 40000) //if voltage is less than 40V
	{
		//close resistor relay
		gpio_set_gpio_pin(RES_RELAY);
		gpio_set_gpio_pin(LED1); //make led1 indicate charging state?
		//other relays open still
		//H2_valve open
		//purge valve still closed
		//fc_State still charge
	}
	else //caps are charged
	{
		//turn off led1
		gpio_clr_gpio_pin(LED1);
		
		//open resistor relay
		//delay how long?
		//why not one state machine cycle?
		if(gpio_get_gpio_pin_output_value(RES_RELAY) == 1)
		{
			gpio_clr_gpio_pin(RES_RELAY);	
			return(fc_state); //state will run again
		}
		
		//close cap relay
		//another delay
		if(gpio_get_gpio_pin_output_value(CAP_RELAY == 0))
		{
			gpio_set_gpio_pin(CAP_RELAY);
			return(fc_state);
		}
		
		//close motor relay
		gpio_set_gpio_pin(MOTOR_RELAY);
		
		//go to main run state
		fc_state = FC_STATE_RUN;
		gpio_clr_gpio_pin(LED_START);
		gpio_set_gpio_pin(LED_RUN);
	}
	return(fc_state);
}


unsigned int fan_update_timer;
unsigned int FC_run(void)
{
	unsigned int fc_state;
	
	//pid fan control to maintain temperature
	if(millis() - fan_update_timer > FANUPDATE_INTERVAL)
	{
		//pid fan control for temp regulation
		FANUpdate(PID( ((get_FCTEMP1()+get_FCTEMP2())/2) , ((53*get_FCCURR()) / 100 + 299160) ));
		fan_update_timer = millis();
	}
	
	//purge control: //purge based on amount of charge extracted from hydrogen (1 C = amp * sec)
	
	delta_purge_time = millis() - purge_integration_timer;
	if(delta_purge_time > PURGE_INTEGRATION_INTERVAL)
	{
		mAms_since_last_purge += delta_purge_time * get_FCCURR();
		purge_integration_timer = millis();
		time_since_last_purge += delta_purge_time;
	}
	
	if (mAms_since_last_purge > PURGE_THRESHOLD) //time to purge
	{
		//open purge valve
		gpio_set_gpio_pin(PURGE_VALVE);
		
		//we restart counting mAms as soon as valve opens
		//reset mAms sum
		mAms_since_last_purge = 0;
		//record time
		time_between_last_purges = time_since_last_purge;
		time_since_last_purge = 0; //reset timer		
		//reset timer1
		purge_integration_timer = millis();
		
		//start purge timer 2 to time purge
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
	
	fc_state = FC_STATE_RUN;
	return(fc_state);
}

unsigned int FC_shutdown(void)
{
	unsigned int fc_state;
	gpio_clr_gpio_pin(LED_RUN);
	gpio_set_gpio_pin(LED_STOP);
	//close valves
	gpio_clr_gpio_pin(H2_VALVE);
	gpio_clr_gpio_pin(PURGE_VALVE);
	//close relays
	gpio_clr_gpio_pin(MOTOR_RELAY);
	gpio_clr_gpio_pin(START_RELAY);
	
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
	
	fc_state = FC_STATE_ALARM;
	//manual reset required to exit alarm state
	return(fc_state);
}
