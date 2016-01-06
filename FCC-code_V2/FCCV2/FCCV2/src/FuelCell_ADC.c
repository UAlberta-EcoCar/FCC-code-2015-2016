/*
 * FuelCell_ADC.c
 *
 * Created: 2015-11-09 6:26:56 PM
 *  Author: Reegan
 */ 

#include "asf.h"
#include "FuelCell_ADC.h"
#include "analog_defs.h"

//create arrays that adc conversion results will be stored in
int16_t adcvals_0[7];
int16_t adcvals_1[6];

void ADCInit(void)
{	
	//Set ADC pins to ADC Function
	gpio_enable_module(ADCIFA_GPIO_MAP, sizeof(ADCIFA_GPIO_MAP) / sizeof(ADCIFA_GPIO_MAP[0]));
	//adc pb clock settings
	
	//Set up ADC
	//adcifa_configure(&AVR32_ADCIFA, &adcifa_opt, 120000); this worked before
	adcifa_configure(&AVR32_ADCIFA, &adcifa_opt, sysclk_get_pbc_hz()); //try this instead 
	
	//configure ADC sequencer 0
	adcifa_configure_sequencer(&AVR32_ADCIFA, 0, &adcifa_sequence_opt, adcifa_sequence_conversion_option_0);
	//sequencer 1
	adcifa_configure_sequencer(&AVR32_ADCIFA, 1, &adcifa_sequence_opt, adcifa_sequence_conversion_option_1);
}

void StartADC_Sequencers(void)
{
	adcifa_start_sequencer(&AVR32_ADCIFA, 0);
	adcifa_start_sequencer(&AVR32_ADCIFA, 1);
}

void ReadADC_Sequencers(void)
{
	while((adcifa_get_values_from_sequencer(&AVR32_ADCIFA,0,&adcifa_sequence_opt,adcvals_0)!=ADCIFA_STATUS_COMPLETED)|(adcifa_get_values_from_sequencer(&AVR32_ADCIFA,1,&adcifa_sequence_opt,adcvals_1)!=ADCIFA_STATUS_COMPLETED));
}


//functions for retrieving values from adcvals array based off of conversion sequence order in analog_defs.h
int convert_temp(int temp_reading)
{
		temp_reading = temp_reading * (-1); //thermistors are on negative input
		//linear curve fit
		temp_reading = -65 * temp_reading + 370650;
		return(temp_reading);
}
int get_FCTEMP1(void)
{
	return(convert_temp(FCTEMP1Reading));
}
int get_FCTEMP2(void)
{
	return(convert_temp(FCTEMP2Reading));
}
//one (or more) of these pressure conversions is wrong
int get_TANKPRES(void)
{
	return(((TANKPRESReading * 3000 / (2048-1) * (470+316) / 470) * 50 - 39700));
}
int get_FCPRES(void)
{
	return(((FCPRESReading * 3000 / (2048-1) * (470+316) / 470) * 9578 - 23670));
}
/*not used
int get_CAPCURR(void)
{
	//from ACS756 datasheet
	//is this the X50 or X100?
	//if its X50:
	//Vout = 2500mV + 40mV/A * I
	//I = (Vout - 2500mV)/40mV
	//because of 31.6k 47k voltage dividers / 12 bit reading
	//Vout = CAPCURRReading * 3000/(2^11 - 1) * (31.6 + 47) / 47 mV
	return(((CAPCURRReading * (316 + 470) / 470 * 3000 / (2048 - 1) - 2500) * 1000 / 40));
}
*/
int FCCURR_intercept = 0;
int get_FCCURR(void)
{
	//from ACS756 datasheet
	//is this the X50 or X100?
	//if its X50:
	//Vout = 2500mV + 40mV/A * I
	//I = (Vout - 2500mV)/40mV
	//because of 31.6k 47k voltage dividers / 12 bit reading
	//Vout = adcreading * 3000/(2^11 - 1) * (31.6 + 47) / 47 mV
	return(FCCURRReading * (316 + 470) / 470 * 3000 / (2048 - 1) * 1000 / 40 - FCCURR_intercept);
}
//zero current reading on startup
void zero_FCCURR(void)
{
	FCCURR_intercept = get_FCCURR();
}
int CAPVOLT_intercept = 0;

int get_CAPVOLT(void)
{
	return(CAPVOLTReading * 3000 * 50 / 3 / (2048-1) - CAPVOLT_intercept);
	//47k and 3k voltage divider
	//3V reference
	//CAPVOLT = ADCreading * 3000mV/(2^11-1) * (47 + 3) / 3
	//should result in a voltage in mV
}

void zero_CAPVOLT(void)
{
	CAPVOLT_intercept = get_CAPVOLT();
}

int FCVOLT_intercept = 0;
int get_FCVOLT(void)
{
	return((FCVOLTReading) * 3000 * 50 / 3 / (2048-1)-FCVOLT_intercept);
	//47k and 3k voltage divider
	//3V reference
	//FCVolt = ADCreading * 3000mV/(2^11-1) * (47 + 3) / 3
	//should result in a voltage in mV
}

void zero_FCVOLT(void)
{
	FCVOLT_intercept = get_FCVOLT();
}
/*
int get_AMBTEMP0(void)
{
	return(convert_temp(AMBTEMP0Reading));
}
int get_AMBTEMP1(void)
{
	return(convert_temp(AMBTEMP1Reading));
}
int get_AMBTEMP2(void)
{
	return(convert_temp(AMBTEMP2Reading));
}
int get_AMBTEMP3(void)
{
	return(convert_temp(AMBTEMP3Reading));
}
*/