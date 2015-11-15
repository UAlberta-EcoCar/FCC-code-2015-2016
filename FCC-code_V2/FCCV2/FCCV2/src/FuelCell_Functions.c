/*
 * FuelCell_Functions.c
 *
 * Created: 2015-11-09 11:07:53 AM
 *  Author: Reegan
 */ 

#include "asf.h"
#include "FuelCell_Functions.h"

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
	if((FCTEMP1Value <= LOW_TEMP_THRES) | (FCTEMP2Value <= LOW_TEMP_THRES))
	{
		error_msg |= FC_ERR_TEMP_L;
	}
	if((FCTEMP1Value >= HIGH_TEMP_THRES) | (FCTEMP2Value >= HIGH_TEMP_THRES))
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
	if(FCVOLTValue > OVER_VOLT_THRES)
	{
		error_msg |= FC_ERR_OVER_VOLT;
	}
	
	return(error_msg);
}