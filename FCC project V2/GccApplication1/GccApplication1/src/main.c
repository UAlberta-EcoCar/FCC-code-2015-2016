/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
uint32_t a;
char b;
unsigned int g;
unsigned long counta;
int main (void)
{
	g=0;
	a=0;
	/* Insert system clock initialization code here (sysclk_init()). */
	scif_start_rc120M();
	board_init();
	
	
	counta=0;
	struct avr32_ast_t myast;
	b=ast_init_counter(&AVR32_AST,AST_OSC_RC,AST_PSEL_RC_1_76HZ,counta);
	ast_enable(&AVR32_AST);
	while(1){
		a=ast_get_counter_value(&AVR32_AST);
	}
	scif_stop_rc120M();


	
	/* Insert application code here, after the board has been initialized. */
}
