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
	//set up timer for millis
	ast_start_gc();
	ast_init_counter(&AVR32_AST,AST_OSC_GCLK,4,counta);  //16kHz / (2^(4+1)) = 1kHz
	ast_enable(&AVR32_AST);
}


unsigned long millis(void)
{
	return(ast_get_counter_value(&AVR32_AST)); 
}