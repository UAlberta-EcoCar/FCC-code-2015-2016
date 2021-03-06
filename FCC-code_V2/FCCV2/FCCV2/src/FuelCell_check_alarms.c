/*
 * FuelCell_check_alarms.c
 * Created: 2015-12-13 4:16:34 PM
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
#include "FuelCell_check_alarms.h"

unsigned int FC_check_alarms(unsigned int fc_state)
{
	unsigned int error_msg = 0;
	//brownout detection
	if(scif_bod50_get_irq_status())
	{
		error_msg |= FC_ERR_BOD;
	}
	if(gpio_get_pin_value(SYSOK) == 0)
	{
		error_msg |= FC_ERR_H2OK_LOW;
	}
	//check temp H L and pressure H always
	if(get_FCTEMP() > HIGH_TEMP_THRES)
	{
		error_msg |= FC_ERR_TEMP_H;
	}
	if(get_FCTEMP() < LOW_TEMP_THRES)
	{
		error_msg |= FC_ERR_TEMP_L;
	}
	if(get_FCPRES() > FC_HIGH_PRES_THRES)
	{
		error_msg |= FC_ERR_PRES_H;
	}
	switch (fc_state)
	{		
		case FC_STATE_STARTUP_PURGE:
		if(get_FCPRES() > FC_HIGH_PRES_THRES)
		{
			error_msg |= FC_ERR_PRES_H;
		}
		if(get_FCCURR() > OVER_CUR_THRES)
		{
			error_msg |= FC_ERR_OVER_CUR;
		}
		if(get_FCVOLT() > OVER_VOLT_THRES)
		{
			error_msg |= FC_ERR_OVER_VOLT;
		}
		if(get_FCPRES() < PURGE_PRESS_LOW_THRES)
		{
			error_msg |= FC_ERR_PURGE_PRESS_LOW;
		}
		break;
		
		case FC_STATE_STARTUP_CHARGE:
		if(get_FCPRES() < FC_LOW_PRES_THRES)
		{
			error_msg |= FC_ERR_PRES_L;
		}
		if (get_FCPRES() > FC_HIGH_PRES_THRES)
		{
			error_msg |= FC_ERR_PRES_H;
		}
		if(get_FCCURR() > OVER_CUR_THRES)
		{
			error_msg |= FC_ERR_OVER_CUR;
		}
		if(get_FCVOLT() > OVER_VOLT_THRES)
		{
			error_msg |= FC_ERR_OVER_VOLT;
		}
		break;
		
		case FC_STATE_RUN:
		if(get_FCPRES() < FC_LOW_PRES_THRES)
		{
			error_msg |= FC_ERR_PRES_L;
		}
		if(get_FCPRES() > FC_HIGH_PRES_THRES)
		{
			error_msg |= FC_ERR_PRES_H;
		}
		if(get_FCCURR() > OVER_CUR_THRES)
		{
			error_msg |= FC_ERR_OVER_CUR;
		}
		if(get_FCVOLT() > OVER_VOLT_THRES)
		{
			error_msg |= FC_ERR_OVER_VOLT;
		}
		if(get_FCVOLT() < LOW_VOLT_THRES)
		{
			error_msg |= FC_ERR_VOLT_LOW;
		}
		break;
		
		case FC_STATE_AIR_STARVE:
		if(get_FCPRES() < FC_LOW_PRES_THRES)
		{
			error_msg |= FC_ERR_PRES_L;
		}
		if(get_FCPRES() > FC_HIGH_PRES_THRES)
		{
			error_msg |= FC_ERR_PRES_H;
		}
		if(get_FCCURR() > OVER_CUR_THRES)
		{
			error_msg |= FC_ERR_OVER_CUR;
		}
		if(get_FCVOLT() > OVER_VOLT_THRES)
		{
			error_msg |= FC_ERR_OVER_VOLT;
		}
		break;
		
		case FC_STATE_ALARM:
		if(get_FCCURR() > OVER_CUR_THRES)
		{
			error_msg |= FC_ERR_OVER_CUR;
		}
		break;
	}

	return(error_msg);
}