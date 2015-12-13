/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include "FuelCell_ADC.h"
#include "FuelCell_PWM.h"
#include "FuelCell_Functions.h"
#include "FuelCell_USART.h"
#include "millis_function.h"

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	scif_start_rc120M();
	scif_start_rc8M();
	
	
	//set cpu divide by 2^(1+1) = 4
	pm_set_clk_domain_div(AVR32_PM_CLK_GRP_CPU,1);
	//switch main clock source
	pm_set_mclk_source(PM_CLK_SRC_RC120M);
	//cpu frequency is now 30 MHz

	//set up pba, pbb, pbc. must be less than fcpu/4
	//cpu is divide by 4 --> need divide by 16.   2^(3+1) = 16
	pm_set_clk_domain_div(AVR32_PM_CLK_GRP_PBA,3);
	pm_set_clk_domain_div(AVR32_PM_CLK_GRP_PBB,3);
	pm_set_clk_domain_div(AVR32_PM_CLK_GRP_PBB,3);
	//120MHz / 16 = 7.5MHz

	//setup adc
	ADCInit();
	
	//setup millis()
	millis_init();
	
	//set up pwm 
	PWMInit();
	
	//setup usart
	USARTInit();
}
