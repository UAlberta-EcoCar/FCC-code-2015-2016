/*
 * IncFile1.h
 *
 * Created: 2015-10-10 1:47:04 PM
 *  Author: Reegan
 */ 


#ifndef FUELCELL_IO_H
#define FUELCELL_IO_H


#define OP1_FAN 0x00000004



//Analog Inputs
#define AMBTEMP4 AVR32_ADCIN14_PIN
#define AMBTEMP3 AVR32_ADCIN13_PIN
#define AMBTEMP2 AVR32_ADCIN12_PIN
#define AMBTEMP1 AVR32_ADCIN11_PIN
#define FCTEMP2 AVR32_ADCIN10_PIN
#define FCTEMP1 AVR32_ADCIN9_PIN
#define MFLOW AVR32_ADCIN8_PIN
#define FCVOLT AVR32_ADCIN7_PIN
#define CAPVOLT AVR32_ADCIN6_PIN
#define FCCURR AVR32_ADCIN5_PIN
#define CAPCURR AVR32_ADCIN4_PIN
#define FCPRES AVR32_ADCIN3_PIN
//adcin2 unused
#define TANKPRES AVR32_ADCIN1_PIN
//adcin0 unused

//Analog GPIO MAP
static const gpio_map_t ADCIFA_GPIO_MAP =
{
	{AMBTEMP4,AVR32_ADCIN14_FUNCTION},
	{AMBTEMP3,AVR32_ADCIN13_FUNCTION},
	{AMBTEMP2,AVR32_ADCIN12_FUNCTION},
	{AMBTEMP1,AVR32_ADCIN11_FUNCTION},
	{FCTEMP2,AVR32_ADCIN10_FUNCTION},
	{FCTEMP1,AVR32_ADCIN9_FUNCTION},
	{MFLOW,AVR32_ADCIN8_FUNCTION},
	{FCVOLT,AVR32_ADCIN7_FUNCTION),
	{CAPVOLT,AVR32_ADCIN6_FUNCTION};
	{FCCURR,AVR32_ADCIN5_FUNCTION};
	{CAPCURR,AVR32_ADCIN4_FUNCTION};
	{FCPRES,AVR32_ADCIN3_FUNCTION};
	{TANKPRES,AVR32_ADCIN1_FUNCTION};
};

//ADC settings
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

adcifa_sequencer_conversion_opt_t adcifa_sequence_conversion_option_0[7] =
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

//these are based off of the above sequence order
#define MFLOWValue adcvals_0[0] //sequencer 0 adcin8
#define FCTEMP1Value adcvals_0[1] //seq 0 adcin9
#define FCTEMP2Value adcvals_0[2] //seq 0 adcin10
#define AMBTEMP1Value adcvals_0[3] //seq 0 adcin11
#define AMBTEMP2Value adcvals_0[4] //seq 0 adcin12
#define AMBTEMP3Value adcvals_0[5] //seq 0 adcin13
#define AMBTEMP0Value adcvals_0[6] //seq 0 adcin 14

adcifa_sequencer_conversion_opt_t adcifa_sequence_conversion_option_1[6] =
{
	{
		.channel_p = AVR32_ADCIFA_INP_ADCIN1,             // Positive Channel
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

//these are based off the above sequencer order
#define TANKPRESValue adcvals_1[0] //seq 1 adcin1
#define FCPRESValue adcvals_1[1] //seq 1 adcin3
#define CAPCURRValue adcvals_1[2] //seq 1 adcin4
#define FCCURRValue adcvals_1[3] //seq 1 adcin5
#define CAPVOLTValue adcvals_1[4] //seq 1 adcin6
#define FCVOLTValue adcvals_1[5] //seq 1 adcin7


//digital Inputs
#define START AVR32_PIN_PB03
#define SYSOK AVR32_PIN_PB02
#define FCCON AVR32_PIN_PB23
#define CAPCON AVR32_PIN_PC00
#define RESCON AVR32_PIN_PB22

//LEDs
#define LED0 AVR32_PIN_PB21
#define LED1 AVR32_PIN_PB30
#define LED2 AVR32_PIN_PB19
#define LED3 AVR32_PIN_PB20
#define LED_RUN AVR32_PIN_PB31
#define LED_START AVR32_PIN_PC01
#define LED_ERROR AVR32_PIN_PC02
#define LED_STOP AVR32_PIN_PC03

//Coil driving outputs
#define OP_1 AVR32_PIN_PD30
#define OP_2 AVR32_PIN_PD29
#define OP_3 AVR32_PIN_PD28
#define OP_4 AVR32_PIN_PD27
#define OP_5 AVR32_PIN_PD24
#define OP_6 AVR32_PIN_PD12
#define OP_7 AVR32_PIN_PD11
#define OP_8 AVR32_PIN_PD10
#define OP_9 AVR32_PIN_PD09
#define OP_10 AVR32_PIN_PD08


#endif /* INCFILE1_H_ */