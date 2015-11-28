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

int adcvals_0[7];
int adcvals_1[8];

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
	TEMP_OPT = (53*get_CAPVOLT()) / 100 + 299160;
	AVE_TEMP = (FCTEMP1Reading + FCTEMP2Reading)/2;
	
	//thermistor curve has a linear range and non linear range
	//see excel thermistor curve fit
	if (AVE_TEMP < 1400) //reading is in the linear range
	{
		//temp in mK is
		//44.767 * ADC_reading + 221732
		AVE_TEMP = ( 4475 * AVE_TEMP ) / 100 + 221732;
	}
	else
	{
		//not linear range -> use cubic approximation
		//temp in mK
		//= 0.0088 x^3 - 40.24 x^2 + 61144 x - 30000000
		//mess around with order of multiplication to avoid round off errors and over flow errors (ie: 1700^3 > mx 32bit value)
		//AVE_TEMP = ((88 * (AVE_TEMP * AVE_TEMP / 10000) * AVE_TEMP) - ((4024 * AVE_TEMP / 100) * AVE_TEMP));
		//how fast is this processor at math?
		//this might take a while. I'm going to simplify
		AVE_TEMP = ((9 * (AVE_TEMP * AVE_TEMP / 1000) * AVE_TEMP) - ((402 * AVE_TEMP / 10) * AVE_TEMP));
	}
	
	temp_error = (TEMP_OPT - AVE_TEMP) / 1000; //in Kalvin
	//accumulated_temp_error = accumulated_temp_error + temp_error;
	
	//fan speed = 0 to 20 //should really increase this value
	//max is 20. min is 5. diff is 15
	//run fan at 1% above min per degree K it is above T_OPT
	FANUpdate(5+(unsigned int)temp_error*20/100);
}

unsigned int FC_check_alarms(unsigned int fc_state)
{
	//to do: make alarms smarter based on fc_state.
	unsigned int error_msg;
	if(gpio_get_pin_value(CAPCON) == 0)
	{
		error_msg |= FC_ERR_CAP_DISC;
	}
	if(gpio_get_pin_value(FCCON) == 0)
	{
		error_msg |= FC_ERR_FC_DISC;
	}
	if(gpio_get_pin_value(SYSOK) == 0) 
	{
		error_msg |= FC_ERR_H2OK_LOW;
	}
	if((FCTEMP1Reading <= LOW_TEMP_THRES)|(FCTEMP2Reading <= LOW_TEMP_THRES))
	{
		error_msg |= FC_ERR_TEMP_L;
	}
	if((FCTEMP1Reading >= HIGH_TEMP_THRES)|(FCTEMP2Reading >= HIGH_TEMP_THRES))
	{
		error_msg |= FC_ERR_TEMP_H;
	}
	//only check pressure in purge, charge and run
	if((FCPRESReading >= HIGH_PRES_THRES))
	{
		error_msg |= FC_ERR_PRES_H;
	}
	if((FCPRESReading <= LOW_PRES_THRES))
	{
		error_msg |= FC_ERR_PRES_L;
	}
	if(get_FCCURR() >= OVER_CUR_THRES)
	{
		error_msg |= FC_ERR_OVER_CUR;
	}
	if(get_FCCURR() <= UNDER_CUR_THRES)
	{
		error_msg |= FC_ERR_UND_CUR;
	}
	if(get_FCVOLT() >= OVER_VOLT_THRES)
	{
		error_msg |= FC_ERR_OVER_VOLT;
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
		//fan off?
		//or low
		FANUpdate(4);
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
	FANUpdate(4);
	//read fan tachometer to ensure fans are spinning
	//increment if tach is 0 and reads 1
	if(tachometer_test == 0)
	{
		if(gpio_get_pin_value(FAN_TACH)==1)
		{
			tachometer_test = 1;
			fc_state = FC_STATE_STARTUP_FANS;
		}
	}
	//then wait for it to go low again (then the fan is spinning)
	else if(tachometer_test == 1)
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

unsigned int purge_timer = 0;
unsigned int FC_startup_purge(void)
{
	//Purge step isn't skipped on !Rescon to get air out of lines on startup. what???
	unsigned int fc_state;
	
	//h2 valve still open
	gpio_set_gpio_pin(H2_VALVE);
	//close startup relay
	gpio_set_gpio_pin(START_RELAY);
	//motor relay still open
	gpio_clr_gpio_pin(MOTOR_RELAY);
	
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

//I'm not sure why we do this step the way we do
unsigned int FC_startup_charge(void)
{
	unsigned int fc_state;
	//Skip charge step if resistors aren't connected
	//pseudo code checked RESCON here
	//however, RESCON == 0 will trigger and alarm anyway so I won't check
	
	//charging capacitors through resistor to avoid temporary short circuit
	if (CAPVOLTValue < 40000) //if voltage is less than 40V
	{
		//close resistor relay
		gpio_set_gpio_pin(RES_RELAY);
		gpio_set_gpio_pin(LED1); //make led1 indicate charging state?
		//other relays open still
		//H2_valve open
		//purge valve still closed
		fc_state = FC_STATE_STARTUP_CHARGE;
		//will we have to purge here?
		//2300C * 40V = will have to purge if capacitor is huge ?? is that math correct?
		//don't have to purge
	}
	else //caps are charged
	{
		//open resistor relay
		gpio_clr_gpio_pin(RES_RELAY);
		//delay how long?
		
		//close cap relay
		gpio_set_gpio_pin(CAP_RELAY);
		//delay
		
		//close motor relay
		gpio_set_gpio_pin(MOTOR_RELAY);
		
		//pseudo code is strange here I think we set state to run ??
		fc_state = FC_STATE_RUN;
		gpio_clr_gpio_pin(LED_START);
		gpio_set_gpio_pin(LED_RUN);
	}
	return(fc_state);
}

unsigned int purge_timer1; //used for integrating Amp seconds
unsigned int purge_timer2; //used for timing how long purge valve is open
unsigned int delta_purge_time;
Union64 mAms_since_last_purge;
unsigned int purge_state = FIRST_PURGE_CYCLE;
unsigned int fan_update_timer;
unsigned int FC_run(void)
{
	//this could almost be recoded to use a switch for purge_state
	
	unsigned int fc_state;
	
	if(millis() - fan_update_timer > FANUPDATE_INTERVAL)
	{
		//pid fan control for temp regulation
		pid_temp_control();
		fan_update_timer = millis();
	}
	
	//purge control: //purge based on amount of charge extracted from hydrogen (1 C = amp * sec)
	
	//if this is the first ever purge, set purge timer
	//this is probably incorrect
	//don't we want to purge after charging caps?
	if(purge_state == FIRST_PURGE_CYCLE)
	{
		purge_timer1 = millis();
		purge_state = PURGE_VALVE_CLOSED; //fuel cell running purge valve closed //prevents timer from being reset
	}
	delta_purge_time = millis() - purge_timer1;
	if(delta_purge_time > PURGE_INTEGRATION_INTERVAL)
	{
		mAms_since_last_purge.u64 += delta_purge_time * get_FCCURR();
		purge_timer1 = millis();
	}
	
	if (mAms_since_last_purge.u64 > PURGE_THRESHOLD) //time to purge
	{
		//open purge valve
		gpio_set_gpio_pin(PURGE_VALVE);
		
		//we restart counting mAms as soon as valve opens
		//reset mAms sum
		mAms_since_last_purge.u64 = 0;
		//reset timer1
		purge_timer1 = millis();
		
		//start purge timer 2 to time purge
		purge_timer2 = millis();
		purge_state = PURGE_VALVE_OPEN; //purge valve open
		//set led0 on because why not
		gpio_set_gpio_pin(LED0);
	}
	
	if(purge_state == PURGE_VALVE_OPEN) //if purge valve is open
	{
		if(millis() - purge_timer2 > PURGE_TIME) //if it has completed purge //is there some way to use flow meter instead of a fixed time?
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