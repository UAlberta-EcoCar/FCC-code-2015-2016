/*
 * FuelCell_DataLogging.c
 *
 * Created: 2016-01-16 9:07:07 AM
 *  Author: Reegan
 */ 
#include <stdlib.h>
#include <stdio.h>
#include "asf.h"
#include "FuelCell_USART.h"
#include "FuelCell_ADC.h"
#include "FuelCell_Functions.h"
#include "FuelCell_DataLogging.h"
#include "millis_function.h"
#include "FuelCell_PWM.h"
#include "FuelCell_mode_Select.h"
#include "digital_IO_defs.h"

char str [100]; //buffer for storing strings. set to length of longest string needed to avoid wasting ram

unsigned int usart_data_display_stagger = 0;
unsigned int usart_data_display_timer;

void usart_data_display(unsigned int fc_state, unsigned int error_msg)
{
	//all data logging occurs after state machine executes
	//that way if alarm is triggered it is dealt with before
	//sending data over serial
	//stagger messages to be able to continue checking levels
	//without a huge serial delay (every message is about a millisecond)
	
	switch (usart_data_display_stagger)
	{
		case 0:
		if(millis()- usart_data_display_timer > USART_DATA_DISPLAY_INTERVAL)
		{
			usart_data_display_stagger = 1;
			usart_data_display_timer = millis();
			sprintf(str,"\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r");
			usart_write_line(DISPLAY_USART,str);
			
			#ifdef TEST_BENCH_MODE
			usart_write_line(DISPLAY_USART,"You are in test bench mode\n\r");
			usart_write_line(DISPLAY_USART,"Using the power board might give you issues!\n\r");
			#endif
			
			#ifdef FAKE_INPUT_MODE
			usart_write_line(DISPLAY_USART,"You are in fake input mode\n\r");
			usart_write_line(DISPLAY_USART,"Do NOT connect the fuel cell!!\n\r");
			#endif
			
			if(error_msg)
			{
				sprintf(str,"ERROR %d\n\r",error_msg);
				usart_write_line(DISPLAY_USART,str);
			}
			sprintf(str,"NUMBER OF PURGES %d\n\r",get_number_of_purges());
			usart_write_line(DISPLAY_USART,str);
			sprintf(str,"LASTPURGE %d\n\r",get_time_between_last_purges());
			usart_write_line(DISPLAY_USART,str);
			sprintf(str,"STATE %d\n\r",fc_state);
			usart_write_line(DISPLAY_USART,str);
			sprintf(str,"\n\r");
			usart_write_line(DISPLAY_USART,str);
		}
		break;
		
		case 1:
		sprintf(str,"FCVOLT %d\n\r",get_FCVOLT());
		usart_write_line(DISPLAY_USART,str);
		sprintf(str,"FCCURR %d\n\r",get_FCCURR());
		usart_write_line(DISPLAY_USART,str);
		sprintf(str,"FCPRES %d\n\r",get_FCPRES());
		usart_write_line(DISPLAY_USART,str);
		sprintf(str,"\n\r");
		usart_write_line(DISPLAY_USART,str);
		usart_data_display_stagger = 2;
		break;
		
		case 2:
		sprintf(str,"FCTEMP %d\n\r",get_FCTEMP());
		usart_write_line(DISPLAY_USART,str);
		sprintf(str,"FANSpeed %d\n\r",get_FANSpeed());
		usart_write_line(DISPLAY_USART,str);
		sprintf(str,"OPT_TEMP %d\n\r",calc_opt_temp());
		usart_write_line(DISPLAY_USART,str);
		sprintf(str,"\n\r");
		usart_write_line(DISPLAY_USART,str);
		usart_data_display_stagger = 3;
		break;
		
		case 3:
		/* AMBTEMP not connected
		sprintf(str,"AMBTEMP0 %d\n\r",get_AMBTEMP0());
		usart_write_line(DISPLAY_USART,str);
		sprintf(str,"AMBTEMP1 %d\n\r",get_AMBTEMP1());
		usart_write_line(DISPLAY_USART,str);
		sprintf(str,"AMBTEMP2 %d\n\r",get_AMBTEMP2());
		usart_write_line(DISPLAY_USART,str);
		sprintf(str,"AMBTEMP3 %d\n\r",get_AMBTEMP3());
		usart_write_line(DISPLAY_USART,str);
		sprintf(str,"\n\r\n\r\n\r");
		usart_write_line(DISPLAY_USART,str);
		*/
		usart_data_display_stagger = 4;
		break;
		
		case 4:
		sprintf(str,"CAPVOLT %d\n\r",get_CAPVOLT());
		usart_write_line(DISPLAY_USART,str);
		sprintf(str,"\n\r");
		usart_write_line(DISPLAY_USART,str);
		usart_data_display_stagger = 0;
		break;
	}
}

///Logging data over serial using csv format

unsigned int usart_data_log_timer = 0;
//at the start of program make table header
void usart_data_log_start(unsigned int fc_state, unsigned int error_msg)
{
	//data log program starts logging data when it receives a $
	usart_write_line(LOG_USART,"$");
	usart_write_line(LOG_USART,"\n\r");

	usart_write_line(LOG_USART,"MILLIS,ERROR,STATE,PURGE_COUNT,LAST_PURGE_TIME,ENERGY,TOTAL_ENERGY,");

	usart_write_line(LOG_USART,"CHARGE,TOTAL_CHARGE,FCVOLT,FCCURR,CAPVOLT,");
	
	usart_write_line(LOG_USART,"FCTEMP1,OPT_TEMP,FCPRES,");
	
	usart_write_line(LOG_USART,"START_RELAY,RES_RELAY,CAP_RELAY,MOTOR_RELAY,");

	usart_write_line(LOG_USART,"PURGE_VALVE,H2_VALVE\n\r");
}
void usart_data_logging(unsigned int fc_state, unsigned int error_msg)
{
	if(millis()-usart_data_log_timer > USART_DATA_LOG_INTERVAL)
	{
		sprintf(str,"%lu,%d,%d,%d,%d,%llu,%llu,",millis(),error_msg,fc_state,get_number_of_purges(),get_time_between_last_purges(),get_J_since_last_purge(),get_total_E());
		usart_write_line(LOG_USART,str);
		sprintf(str,"%llu,%llu,%d,%d,%d,",get_coulumbs_since_last_purge(),get_total_charge_extracted(),get_FCVOLT(),get_FCCURR(),get_CAPVOLT());
		usart_write_line(LOG_USART,str);
		sprintf(str,"%d,%d,%d,",get_FCTEMP(),calc_opt_temp(),get_FCPRES());
		usart_write_line(LOG_USART,str);
		sprintf(str,"%d,%d,%d,%d,",gpio_get_gpio_pin_output_value(START_RELAY),gpio_get_gpio_pin_output_value(RES_RELAY),gpio_get_gpio_pin_output_value(CAP_RELAY),gpio_get_gpio_pin_output_value(MOTOR_RELAY));
		usart_write_line(LOG_USART,str);
		sprintf(str,"%d,%d\n\r",gpio_get_gpio_pin_output_value(PURGE_VALVE),gpio_get_gpio_pin_output_value(H2_VALVE));
		usart_write_line(LOG_USART,str);
		usart_data_log_timer = millis();
	}
}

unsigned int usart_bridge_timer;
void usart_can_bridge(unsigned int fc_state, unsigned int error_msg)
{
	if(millis()-usart_bridge_timer > USART_BRIDGE_INTERVAL)
	{
		sprintf(str,"%d,%d,%d,%d,%llu,",error_msg,fc_state,get_number_of_purges(),get_time_between_last_purges(),get_J_since_last_purge());
		usart_write_line(USART_BRIDGE,str);
		
		sprintf(str,"%llu,%llu,%llu,%d,%d,",get_total_E(),get_coulumbs_since_last_purge(),get_total_charge_extracted(),get_FCVOLT(),get_FCCURR());
		usart_write_line(USART_BRIDGE,str);
		
		sprintf(str,"%d,%d,%d,%d,%d,",get_FCTEMP(),get_FCPRES(),get_CAPVOLT(),get_FANSpeed(),gpio_get_gpio_pin_output_value(START_RELAY));
		usart_write_line(USART_BRIDGE,str);
		
		sprintf(str,"%d,%d,%d,%d,%d\n",gpio_get_gpio_pin_output_value(RES_RELAY),gpio_get_gpio_pin_output_value(CAP_RELAY),gpio_get_gpio_pin_output_value(MOTOR_RELAY),gpio_get_gpio_pin_output_value(PURGE_VALVE),gpio_get_gpio_pin_output_value(H2_VALVE));
		usart_write_line(USART_BRIDGE,str);
		
		usart_bridge_timer = millis();
	}	
}


