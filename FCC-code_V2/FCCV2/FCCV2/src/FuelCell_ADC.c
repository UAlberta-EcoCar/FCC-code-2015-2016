/*
 * FuelCell_ADC.c
 *
 * Created: 2015-11-09 6:26:56 PM
 *  Author: Reegan
 */ 

#include "asf.h"
#include "FuelCell_ADC.h"
#include "analog_defs.h"
#include "FuelCell_ADC_conversion_consts.h"

//create arrays that adc conversion results will be stored in
int16_t adcvals_0[7];
int16_t adcvals_1[6];

int16_t readvals_0a[7];
int16_t readvals_0b[7];
int16_t readvals_0c[7];
int16_t readvals_0d[7];

int16_t readvals_1a[6];
int16_t readvals_1b[6];
int16_t readvals_1c[6];
int16_t readvals_1d[6];

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

uint8_t readstagger = 0;
void ReadADC_Sequencers(void)
{
	while((adcifa_get_values_from_sequencer(&AVR32_ADCIFA,0,&adcifa_sequence_opt,adcvals_0)!=ADCIFA_STATUS_COMPLETED)|(adcifa_get_values_from_sequencer(&AVR32_ADCIFA,1,&adcifa_sequence_opt,adcvals_1)!=ADCIFA_STATUS_COMPLETED));
	switch(readstagger)
	{
		case 0:
		for(int x = 1;x <= 7;x++)
		{
			readvals_0a[x] = adcvals_0[x];	
		}
		for(int x = 1;x <= 6;x++)
		{
			readvals_1a[x] = adcvals_1[x];	
		}
		readstagger = 1;
		break;
		
		case 1:
		for(int x = 1;x <= 7;x++)
		{
			readvals_0b[x] = adcvals_0[x];	
		}
		for(int x = 1;x <= 6;x++)
		{
			readvals_1b[x] = adcvals_1[x];	
		}
		readstagger = 2;
		break;
		
		case 2:
		for(int x = 1;x <= 7;x++)
		{
			readvals_0c[x] = adcvals_0[x];	
		}
		for(int x = 1;x <= 6;x++)
		{
			readvals_1c[x] = adcvals_1[x];	
		}
		readstagger = 3;
		break;
		
		case 3:
		for(int x = 1;x <= 7;x++)
		{
			readvals_0d[x] = adcvals_0[x];	
		}
		for(int x = 1;x <= 6;x++)
		{
			readvals_1d[x] = adcvals_1[x];	
		}
		readstagger = 0;
		break;
	}
}


//functions for retrieving values from adcvals array based off of conversion sequence order in analog_defs.h
int convert_temp(int temp_reading)
{
		temp_reading = temp_reading * (-1); //thermistors are on negative input
		//linear curve fit
		temp_reading = TEMPCoefficient * temp_reading + TEMPConst;
		return(temp_reading);
}
int get_FCTEMP(void)
{
	return(convert_temp((FCTEMP1Reading + FCTEMP2Reading)/2));
}
//one (or more) of these pressure conversions is wrong
int get_TANKPRES(void)
{
	return(TANKPRESReading * TANKPRESCoefficient - TANKPRESConst);
}
int get_FCPRES(void)
{
	return(FCPRESReading * FCPRESCoefficient - FCPRESConst);
}
/*not used
int get_CAPCURR(void)
{
	return(((CAPCURRReading * (316 + 470) / 470 * 3000 / (2048 - 1) - 2500) * 1000 / 40));
}
*/

int FCCURR_intercept = FCCURRConst;
int get_FCCURR(void)
{
	int val = FCCURRReading * FCCURRCoefficient - FCCURR_intercept;
	//val = val *(-1); //I have sensor connected backwards 
	if(val < 0) //filter out negative numbers b/c they mess with the current integration algorithm
	{
		return(0);
	}
	else
	{
		return(val);
	}
}
//zero current reading on startup
void zero_FCCURR(void)
{
	FCCURR_intercept = get_FCCURR();
}

int CAPVOLT_intercept = 0;
int get_CAPVOLT(void)
{
	return(CAPVOLTReading * CAPVOLTCoefficient - CAPVOLT_intercept);
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
	return((FCVOLTReading) * FCVOLTCoefficient - FCVOLT_intercept);
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