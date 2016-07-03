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

char str [130]; //buffer for storing strings. set to length of longest string needed to avoid wasting ram
int32_t val;
int32_t val2;
int32_t val3;
int32_t val4;

unsigned int usart_bridge_timer;
void usart_can_bridge(unsigned int fc_state, unsigned int error_msg,unsigned int past_fc_state)
{
	if(millis()-usart_bridge_timer > USART_BRIDGE_INTERVAL)
	{
		sprintf(str,"%d,%d,%d,%d,%d,%llu,",millis(),error_msg,fc_state,get_number_of_purges(),get_time_between_last_purges(),get_J_since_last_purge());
		usart_write_line(USART_BRIDGE,str);
		
		sprintf(str,"%llu,%llu,%llu,%d,%d,",get_total_E(),get_coulumbs_since_last_purge(),get_total_charge_extracted(),get_FCVOLT()/1000,get_FCCURR()/1000);
		usart_write_line(USART_BRIDGE,str);
		val = get_CAPVOLT();
		val2 = get_FCVOLT();
		val3 = get_FCCURR();
		val4 = get_FCPRES();
		sprintf(str,"%d,%d,%d,%d,%d,%d,",get_CAPVOLT()/1000,get_FCTEMP()/1000-273,calc_opt_temp()/1000-273,get_FCPRES()/1000,get_FANSpeed(),gpio_get_gpio_pin_output_value(START_RELAY));
		usart_write_line(USART_BRIDGE,str);

		sprintf(str,"%d,%d,%d,%d,%d,%d\n",gpio_get_gpio_pin_output_value(RES_RELAY),gpio_get_gpio_pin_output_value(CAP_RELAY),gpio_get_gpio_pin_output_value(MOTOR_RELAY),gpio_get_gpio_pin_output_value(PURGE_VALVE),gpio_get_gpio_pin_output_value(H2_VALVE),past_fc_state);
		usart_write_line(USART_BRIDGE,str);
		
		usart_bridge_timer = millis();
	}	
}