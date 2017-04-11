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
		//configure pins for usart 2
	 	static const gpio_map_t USART_GPIO_MAP =
	 	{
		 	{OPEN_LOG_USART_RX_PIN, OPEN_LOG_USART_RX_FUNCTION},
		 	{OPEN_LOG_USART_TX_PIN, OPEN_LOG_USART_TX_FUNCTION}
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
	 	usart_init_rs232(OPEN_LOG_USART, &USART_OPTIONS, TARGET_PBCCLK_FREQ_HZ);


}