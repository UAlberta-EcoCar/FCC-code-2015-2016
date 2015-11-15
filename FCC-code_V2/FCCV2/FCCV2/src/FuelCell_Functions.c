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

//create value for timer
unsigned long counta;

// Start AST generic clock input
static void ast_start_gc(void)
{
	//AST generic clock 8MHz / (2*(249+1) = 16kHz
	scif_gc_setup(AVR32_SCIF_GCLK_AST,SCIF_GCCTRL_RC8M,AVR32_SCIF_GC_DIV_CLOCK,249);
	// Now enable the generic clock
	scif_gc_enable(AVR32_SCIF_GCLK_AST);
}

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

unsigned int FC_check_alarms(void)
{
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
	if((FCTEMP1Value <= LOW_TEMP_THRES)|(FCTEMP2Value <= LOW_TEMP_THRES))
	{
		error_msg |= FC_ERR_TEMP_L;
	}
	if((FCTEMP1Value >= HIGH_TEMP_THRES)|(FCTEMP2Value >= HIGH_TEMP_THRES))
	{
		error_msg |= FC_ERR_TEMP_H;
	}
	//might want to change this depending on run mode
	if(FCPRESValue >= HIGH_PRES_THRES)
	{
		error_msg |= FC_ERR_PRES_H;
	}
	if(FCPRESValue <= LOW_PRES_THRES)
	{
		error_msg |= FC_ERR_PRES_L;
	}
	if(FCCURRValue >= OVER_CUR_THRES)
	{
		error_msg |= FC_ERR_OVER_CUR;
	}
	if(FCCURRValue <= UNDER_CUR_THRES)
	{
		error_msg |= FC_ERR_UND_CUR;
	}
	if(FCVOLTValue >= OVER_VOLT_THRES)
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
		fc_state = FC_STATE_STARTUP_H2;
	}
	else
	{
		fc_state = FC_STATE_STANDBY;
	}
	return(fc_state);
}

unsigned int FC_startup_h2(void)
{
	unsigned int fc_state;
	//set fans to min
	FANUpdate(0);
	//open h2 valve
	gpio_set_gpio_pin(H2_VALVE);
	if (FCVOLTValue < (30*ONE_VOLT))
	{
		fc_state = FC_STATE_STARTUP_H2;
	}
	else
	{
		fc_state = FC_STATE_STARTUP_PURGE;
	}
	return(fc_state);
}

unsigned int last_purge_time = 0;
unsigned int FC_startup_purge(void)
{
	//Purge step isn't skipped on !Rescon to get air out of lines on startup
	unsigned int fc_state;
	
	//close startup relay
	gpio_clr_gpio_pin(START_RELAY);
	
	//open purge valve and start timer
	if(gpio_get_gpio_pin_output_value(PURGE_VALVE) == 0)
	{
		last_purge_time = millis();
	}
	gpio_set_gpio_pin(PURGE_VALVE);
	//hardware timing mechanism should be worked out
	//balazs has pseudo code purge for 3 seconds
	if(millis() - last_purge_time < 3000)
	{
		fc_state = FC_STATE_STARTUP_PURGE;
	}
	else
	{
		//close purge valve
		gpio_clr_gpio_pin(PURGE_VALVE);
		//open startup relay
		gpio_set_gpio_pin(START_RELAY);
		fc_state = FC_STATE_STARTUP_CHARGE;
	}
	return(fc_state);
}


unsigned int FC_startup_charge(void)
{
	unsigned int fc_state;
	//Skip charge step if resistors aren't connected
	if (gpio_get_pin_value(RESCON) == 0)
	{
		fc_state = 17; //return some random number ???
		//17 is a cool number
	}
	else
	{
		if (CAPVOLTValue < (40 * ONE_VOLT))
		{
			//close resistor relay
			gpio_set_gpio_pin(RES_RELAY);
			fc_state = FC_STATE_STARTUP_CHARGE;
		}
		else
		{
			//open resistor relay
			gpio_clr_gpio_pin(RES_RELAY);
			//delay how long?
			
			//close cap relay
			gpio_set_gpio_pin(CAP_RELAY);
			//delay
			
			//close motor relay
			gpio_set_gpio_pin(MOTOR_RELAY);
			
			//pseudo code is strange I think we set state to run
			fc_state = FC_STATE_RUN;
		}
	}
	return(fc_state);
}