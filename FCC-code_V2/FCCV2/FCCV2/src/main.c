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
#include "FuelCell_IO.h"

#define FALSE 0
#define TRUE 1

uint32_t a;
char b;
unsigned int g;
unsigned long counta;

adcifa_opt_t adcifa_opt = {
	.frequency                = 10000,  // ADC frequency (Hz)
	.reference_source         = ADCIFA_REF1V, // Reference Source
	.sample_and_hold_disable  = FALSE,    // Disable Sample and Hold Time
	.single_sequencer_mode    = FALSE,    // Single Sequencer Mode
	.free_running_mode_enable = FALSE,    // Free Running Mode
	.sleep_mode_enable        = FALSE     // Sleep Mode
};

adcifa_sequencer_opt_t adcifa_sequence_opt = {
	.convnb               = 7, // Number of sequence
	.resolution           = ADCIFA_SRES_12B,         // Resolution selection
	.trigger_selection    = ADCIFA_TRGSEL_SOFT,      // Trigger selection
	.start_of_conversion  = ADCIFA_SOCB_ALLSEQ,      // Conversion Management
	.half_word_adjustment = ADCIFA_HWLA_NOADJ,       // Half word Adjustment
	.software_acknowledge = ADCIFA_SA_NO_EOS_SOFTACK // Software Acknowledge
};

 adcifa_sequencer_conversion_opt_t adcifa_sequence_conversion_option[7] =
  {
	  {
		  .channel_p = AVR32_ADCIFA_INP_GNDANA,   // Positive Channel
		  .channel_n = AVR32_ADCIFA_INN_ADCIN8,   // Negative Channel
		  .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	  },
	  {
		  .channel_p = AVR32_ADCIFA_INP_GNDANA,             // Positive Channel
		  .channel_n = AVR32_ADCIFA_INN_ADCIN9,             // Negative Channel
		  .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	  },
	  {
		  .channel_p = AVR32_ADCIFA_INP_GNDANA,             // Positive Channel
		  .channel_n = AVR32_ADCIFA_INN_ADCIN10,             // Negative Channel
		  .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	  },
	  {
		  .channel_p = AVR32_ADCIFA_INP_GNDANA,             // Positive Channel
		  .channel_n = AVR32_ADCIFA_INN_ADCIN11,             // Negative Channel
		  .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	  },
	  {
		  .channel_p = AVR32_ADCIFA_INP_GNDANA,             // Positive Channel
		  .channel_n = AVR32_ADCIFA_INN_ADCIN12,             // Negative Channel
		  .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	  },
	  {
		  .channel_p = AVR32_ADCIFA_INP_GNDANA,             // Positive Channel
		  .channel_n = AVR32_ADCIFA_INN_ADCIN13,             // Negative Channel
		  .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	  },
	  {
		  .channel_p = AVR32_ADCIFA_INP_GNDANA,             // Positive Channel
		  .channel_n = AVR32_ADCIFA_INN_ADCIN14,             // Negative Channel
		  .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	  },
  };

 adcifa_sequencer_conversion_opt_t adcifa_sequence_conversion_option_1[8] =
 {
	 {
		 .channel_p = AVR32_ADCIFA_INP_ADCIN0,   // Positive Channel
		 .channel_n = AVR32_ADCIFA_INN_GNDANA,   // Negative Channel
		 .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	 },
	 {
		 .channel_p = AVR32_ADCIFA_INP_ADCIN1,             // Positive Channel
		 .channel_n = AVR32_ADCIFA_INN_GNDANA,             // Negative Channel
		 .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	 },
	 {
		 .channel_p = AVR32_ADCIFA_INP_ADCIN2,             // Positive Channel
		 .channel_n = AVR32_ADCIFA_INN_GNDANA,             // Negative Channel
		 .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	 },
	 {
		 .channel_p = AVR32_ADCIFA_INP_ADCIN3,             // Positive Channel
		 .channel_n = AVR32_ADCIFA_INN_GNDANA,             // Negative Channel
		 .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	 },
	 {
		 .channel_p = AVR32_ADCIFA_INP_ADCIN4,             // Positive Channel
		 .channel_n = AVR32_ADCIFA_INN_GNDANA,             // Negative Channel
		 .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	 },
	 {
		 .channel_p = AVR32_ADCIFA_INP_ADCIN5,             // Positive Channel
		 .channel_n = AVR32_ADCIFA_INN_GNDANA,             // Negative Channel
		 .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	 },
	 {
		 .channel_p = AVR32_ADCIFA_INP_ADCIN6,             // Positive Channel
		 .channel_n = AVR32_ADCIFA_INN_GNDANA,             // Negative Channel
		 .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	 },
	 {
		 .channel_p = AVR32_ADCIFA_INP_ADCIN7,             // Positive Channel
		 .channel_n = AVR32_ADCIFA_INN_GNDANA,             // Negative Channel
		 .gain      = ADCIFA_SHG_1                     // Gain of the conversion
	 },
 };


int16_t adcvals[7];
int16_t adcvals_1[8];


int main (void)

{
	board_init();
	gpio_enable_module(ADCIFA_GPIO_MAP, sizeof(ADCIFA_GPIO_MAP) / sizeof(ADCIFA_GPIO_MAP[0]));
	adcifa_configure(&AVR32_ADCIFA, &adcifa_opt, 120000);
	
	adcifa_configure_sequencer(&AVR32_ADCIFA, 0, &adcifa_sequence_opt, adcifa_sequence_conversion_option);
	
	DLED_GPERS=(LED0 | LED1 | LED2 | LED3);
	DLED_ODERS=(LED0 | LED1 | LED2 | LED3);
	
	g=0;
	a=0;
	/* Insert system clock initialization code here (sysclk_init()). */
	
	
	
	counta=0;
	b=ast_init_counter(&AVR32_AST,AST_OSC_RC,AST_PSEL_32KHZ_1HZ,counta);
	ast_enable(&AVR32_AST);
	while(1){
		adcifa_start_sequencer(&AVR32_ADCIFA, 0);
		while(adcifa_get_values_from_sequencer(&AVR32_ADCIFA,
		0,
		&adcifa_sequence_opt,
		adcvals)!=ADCIFA_STATUS_COMPLETED);
		DLED_OVRT |= LED0;
	}
	/*while(1){
		a=ast_get_counter_value(&AVR32_AST);
		if((ast_get_counter_value(&AVR32_AST)%7)==0){
			DLED_OVRS |= LED0;
		}
		if((ast_get_counter_value(&AVR32_AST)%13)==0){
			DLED_OVRC |= LED0;
		}
	}*/


	
	/* Insert application code here, after the board has been initialized. */
}