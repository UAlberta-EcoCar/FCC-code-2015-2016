/*
 * FuelCell_ADC_conversion_consts.h
 *
 * Created: 2016-01-15 8:48:56 PM
 *  Author: Reegan
 */ 


#ifndef FUELCELL_ADC_CONVERSION_CONSTS_H_
#define FUELCELL_ADC_CONVERSION_CONSTS_H_

#include "FuelCell_mode_Select.h"

#ifdef FAKE_INPUT_MODE
#define FCCURRCoefficient ((316 + 470) / 470 * 3000 / (2048 - 1) * 1000 / 40) //+-5V from zero point ~= +-125A
#define CAPCURRCoefficient ((316 + 470) / 470 * 3000 / (2048 - 1) * 1000 / 40)
#define CAPVOLTCoefficient (3000 * 50 / 3 / (2048-1) * 12) //FCC will think 5V is 60 Volts
#define FCVOLTCoefficient (3000 * 50 / 3 / (2048-1) * 12) //FCC will think 5V is 60 Volts
#define FCPRESCoefficient 0 //I don't know pressure sensor conversion
#define FCPRESConst -4000 //will always read as 4 PSI
#define TANKPRESCoefficient 0 // I don't know pressure sensor conversion
#define TANKPRESConst 0 //tank isn't connected anyway
#define TEMPCoefficient 0
#define TEMPConst 313000 //Unable to connect Thermistors to test bench. Will always read 40degC
#endif


#ifdef IN_CAR_MODE
#define FCCURRCoefficient ((316 + 470) / 470 * 3000 / (2048 - 1) / 16) //16 mV per Amp
#define CAPCURRCoefficient ((316 + 470) / 470 * 3000 / (2048 - 1) / 16) //16 mV per Amp
#define CAPVOLTCoefficient (3000 * 50 / 3 / (2048-1))
#define FCVOLTCoefficient (3000 * 50 / 3 / (2048-1))
#define FCPRESCoefficient (3000 / (2048-1) * (470+316) / 470) * 9578) //P = (v - 2.47119999) / 0.1044
#define FCPRESConst 23670
#define TANKPRESCoefficient (3000 / (2048-1) * (470+316) / 470) * 50)
#define TANKPRESConst 39700
#define TEMPCoefficient 0
#define TEMPConst 323150
#endif


#ifdef TEST_BENCH_MODE
#define FCCURRCoefficient ((316 + 470) / 470 * 3000 / (2048 - 1) / 16) //16mV per Amp
#define CAPCURRCoefficient ((316 + 470) / 470 * 3000 / (2048 - 1) / 16)
#define CAPVOLTCoefficient (3000 * 50 / 3 / (2048-1))
#define FCVOLTCoefficient (3000 * 50 / 3 / (2048-1))
#define FCPRESCoefficient (3000 / (2048-1) * (470+316) / 470 * 9578)
#define FCPRESConst 23670 //P = (v - 2.47119999) / 0.1044
#define TANKPRESCoefficient 0
#define TANKPRESConst 0
#define TEMPCoefficient -62
#define TEMPConst 367572
#endif



#endif /* FUELCELL_ADC_CONVERSION_CONSTS_H_ */