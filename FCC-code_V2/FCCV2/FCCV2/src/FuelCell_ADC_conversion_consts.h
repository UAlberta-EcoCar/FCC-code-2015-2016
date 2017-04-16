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
#define FCCURRConst 0
#define CAPCURRCoefficient ((316 + 470) / 470 * 3000 / (2048 - 1) * 1000 / 40)
#define CAPVOLTCoefficient (3000 * 50 / 3 / (2048-1) * 12) //FCC will think 5V is 60 Volts
#define FCVOLTCoefficient (3000 * 50 / 3 / (2048-1) * 12) //FCC will think 5V is 60 Volts
#define FCPRESCoefficient 0 //I don't know pressure sensor conversion
#define FCPRESConst -4000 //will always read as 4 PSI
#define TEMPCoefficient 0
#define TEMPConst 313000 //Unable to connect Thermistors to test bench. Will always read 40degC
#endif




#ifdef TEST_BENCH_MODE
#define FCCURRCoefficient ((3000 * 10) / (2048-1)) //100 gain, Shunt resistance is 0.001 ohms
#define CAPCURRCoefficient ((3000 * 10.60603) / (2048-1)) //94.286 gain, Shunt resistance is 0.001 ohms
#define CAPVOLTCoefficient 19474
#define CAPVOLTConst 1144.6
#define FCVOLTCoefficient 20075
#define FCVOLTConst 1294.9
#define FCPRESCoefficient 24
#define FCPRESConst 24648
#define TEMPCoefficient -62
#define TEMPConst 367572
#endif



#endif /* FUELCELL_ADC_CONVERSION_CONSTS_H_ */