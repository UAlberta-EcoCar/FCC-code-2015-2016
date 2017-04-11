/*
 * FuelCell_USART.h
 *
 * Created: 2015-12-05 6:00:22 PM
 *  Author: Reegan
 */


#ifndef FUELCELL_USART_H_
#define FUELCELL_USART_H_

void USARTInit(void);


#define TARGET_PBCCLK_FREQ_HZ 7500000  // PBA clock target frequency, in Hz see init.c

//OPEN LOG PC4 and PC5
//USART2-TXD and USART2-RXD
#define OPEN_LOG_USART (&AVR32_USART2)
#define OPEN_LOG_USART_RX_PIN       AVR32_USART2_RXD_1_PIN //defined as gpio pin 69 (page 14 of datasheet)
#define OPEN_LOG_USART_RX_FUNCTION  AVR32_USART2_RXD_FUNCTION //defined as 2 for gpio function C (0,1,2 => a,b,c)
#define OPEN_LOG_USART_TX_PIN       AVR32_USART2_TXD_1_PIN //defined as gpio pin 68 (page 14 of data sheet)
#define OPEN_LOG_USART_TX_FUNCTION  AVR32_USART2_TXD_FUNCTION //gpio function C
#define OPEN_LOG_USART_CLOCK_MASK   AVR32_USART2_CLK_PBA //usart 0,2,3 are on clock PBA (page 64 of datasheet)
//The next two lines don't seem to be used anywhere so I commented them out
//#define OPEN_LOG_PDCA_CLOCK_HSB		AVR32_PDCA_CLK_HSB //i forget what this setting does
//#define OPEN_LOG_PDCA_CLOCK_PB		AVR32_PDCA_CLK_PBC //i forget this one as well


#endif /* FUELCELL_USART_H_ */
