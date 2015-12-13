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
		temp_reading = temp_reading * (-1);
		//thermistor curve has a linear range and non linear range
		//see excel thermistor curve fit
		/*if ((temp_reading <= 251)&(temp_reading > 88)) //reading is in the linear range (293 to 363K)
		{
			//temp in mK is
			temp_reading = ( -4088 * temp_reading ) / 100 + 399090;
		}
		else if (temp_reading > 251) //lower non linear range (253 to 293K)
		{
			temp_reading = -494*temp_reading*temp_reading/10 + 25109*temp_reading-2900700;
		}
		else //temp is in upper linearish range (363K +)
		{
			temp_reading = -1361*temp_reading + 577290;
		}*/
		temp_reading = -428 * temp_reading + 404830;
		
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
int get_FCCURR(void)
{
	//from ACS756 datasheet
	//is this the X50 or X100?
	//if its X50:
	//Vout = 2500mV + 40mV/A * I
	//I = (Vout - 2500mV)/40mV
	//because of 31.6k 47k voltage dividers / 12 bit reading
	//Vout = adcreading * 3000/(2^11 - 1) * (31.6 + 47) / 47 mV
	return(((FCCURRReading * (316 + 470) / 470 * 3000 / (2048 - 1) - 2500) * 1000 / 40));
}
int get_CAPVOLT(void)
{
	return(CAPVOLTReading * 3000 * 50 / 3 / (2048-1));
	//47k and 3k voltage divider
	//3V reference
	//CAPVOLT = ADCreading * 3000mV/(2^11-1) * (47 + 3) / 3
	//should result in a voltage in mV
}
int get_FCVOLT(void)
{
	return((FCVOLTReading) * 3000 * 50 / 3 / (2048-1));
	//47k and 3k voltage divider
	//3V reference
	//FCVolt = ADCreading * 3000mV/(2^11-1) * (47 + 3) / 3
	//should result in a voltage in mV
}
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
