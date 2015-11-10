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
#include "FuelCell_Functions.h"
#include "digital_IO_defs.h"
#include "FuelCell_ADC.h"

uint32_t a;
char b;
unsigned int g;

unsigned int val;
unsigned long time1 = 0;
unsigned long time2 = 0;


int main (void)
{
	FuelCellInit();
	
	
	g=0;
	a=0;
	/* Insert system clock initialization code here (sysclk_init()). */

	while(1)
	{
		StartADC_Sequencers();
		ReadADC_Sequencers();
		gpio_tgl_gpio_pin(LED0);
		val = FCVOLTValue;
		if(millis() - time1 > 1000)
		{
			gpio_tgl_gpio_pin(LED1);
		}
	}
}