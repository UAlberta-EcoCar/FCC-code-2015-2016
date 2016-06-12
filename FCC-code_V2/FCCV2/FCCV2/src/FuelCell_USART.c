/*
 * FuelCell_USART.c
 *
 * Created: 2015-12-05 6:00:06 PM
 *  Author: Reegan
 */ 
#include "FuelCell_USART.h"
#include "asf.h"

void USARTInit(void)
{
		//start usart 3
	 	static const gpio_map_t USART_GPIO_MAP =
	 	{
		 	{DISPLAY_USART_RX_PIN, DISPLAY_USART_RX_FUNCTION},
		 	{DISPLAY_USART_TX_PIN, DISPLAY_USART_TX_FUNCTION},
			{LOG_USART_RX_PIN, LOG_USART_RX_FUNCTION},
			{LOG_USART_TX_PIN, LOG_USART_TX_FUNCTION}
	 	};

	 	// USART options.
	 	static const usart_options_t USART_OPTIONS =
	 	{
		 	.baudrate     = 57600,
		 	.charlength   = 8,
		 	.paritytype   = USART_NO_PARITY,
		 	.stopbits     = USART_1_STOPBIT,
		 	.channelmode  = USART_NORMAL_CHMODE
	 	};

	 	// Assign GPIO to USART.
	 	gpio_enable_module(USART_GPIO_MAP,
	 	sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));


	 	// Initialize USART in RS232 mode.
	 	usart_init_rs232(DISPLAY_USART, &USART_OPTIONS, TARGET_PBCCLK_FREQ_HZ);


	//start usart 4
		
	// Initialize USART in RS232 mode.
	usart_init_rs232(LOG_USART, &USART_OPTIONS, TARGET_PBCCLK_FREQ_HZ);
}