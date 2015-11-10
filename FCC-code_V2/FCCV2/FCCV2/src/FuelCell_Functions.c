/*
 * FuelCell_Functions.c
 *
 * Created: 2015-11-09 11:07:53 AM
 *  Author: Reegan
 */ 

#include <asf.h>
#include "FuelCell_Functions.h"
#include "FuelCell_ADC.h"


//create value for timer
unsigned long counta;


void FuelCellInit(void)
{
	board_init();	
	ADCInit();
	
	//set up timer for millis
	ast_init_counter(&AVR32_AST,AST_OSC_RC,0,counta);
	ast_enable(&AVR32_AST);
	
	//set up pwm (this will be fun)
}
unsigned long millis(void)
{
	ast_get_counter_value(&AVR32_AST);
	return(counta*60); //60 isn't correct number recalculate later
}