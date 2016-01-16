/*
 * FuelCell_ADC_conversion_consts.h
 *
 * Created: 2016-01-15 8:48:56 PM
 *  Author: Reegan
 */ 


#ifndef FUELCELL_ADC_CONVERSION_CONSTS_H_
#define FUELCELL_ADC_CONVERSION_CONSTS_H_

#include "FuelCell_mode_Select.h"

#ifdef TEST_BENCH_MODE

#warning "You are in Test Bench Mode"
#warning "Do not connect Fuel Cell"

#define FCCURRCoefficient ((316 + 470) / 470 * 3000 / (2048 - 1) * 1000 / 40) //+-5V from zero point = +-125A
#define CAPVOLTCoefficient (3000 * 50 / 3 / (2048-1) * 60) //FCC will think 10V is 60 Volts
#define FCVOLTCoefficient (3000 * 50 / 3 / (2048-1) * 60) //FCC will think 10V is 60 Volts
#define FCPRESCoefficient (3000 / (2048-1) * (470+316) / 470) * 9578)
#define FCPRESConst 23670
#define TANKPRESCoefficient (3000 / (2048-1) * (470+316) / 470) * 50)
#define TANKPRESConst 39700
#define TEMPCoefficient 0
#define TEMPConst 370650 //Unable to connect Thermistors to testbench. Will always read 50degC

#endif


#ifdef IN_CAR_MODE

#define FCCURRCoefficient ((316 + 470) / 470 * 3000 / (2048 - 1) * 1000 / 40)
#define CAPVOLTCoefficient (3000 * 50 / 3 / (2048-1))
#define FCVOLTCoefficient (3000 * 50 / 3 / (2048-1))
#define FCPRESCoefficient (3000 / (2048-1) * (470+316) / 470) * 9578)
#define FCPRESConst 23670
#define TANKPRESCoefficient (3000 / (2048-1) * (470+316) / 470) * 50)
#define TANKPRESConst 39700
#define TEMPCoefficient 0
#define TEMPConst 323150

#endif


#ifdef OUT_OF_CAR_MODE

#warning "You are in out of car mode"
#warning "if you are using Power Board you will have issues"

#define FCCURRCoefficient ((316 + 470) / 470 * 3000 / (2048 - 1) * 1000 / 40)
#define CAPVOLTCoefficient (3000 * 50 / 3 / (2048-1))
#define FCVOLTCoefficient (3000 * 50 / 3 / (2048-1))
#define FCPRESCoefficient (3000 / (2048-1) * (470+316) / 470) * 9578)
#define FCPRESConst 23670
#define TANKPRESCoefficient (3000 / (2048-1) * (470+316) / 470) * 50)
#define TANKPRESConst 39700
#define TEMPCoefficient -65
#define TEMPConst 370650

#endif



#endif /* FUELCELL_ADC_CONVERSION_CONSTS_H_ */