/*
 * FuelCell_ADC.h
 *
 * Created: 2015-11-09 6:26:42 PM
 *  Author: Reegan
 */ 


#ifndef FUELCELL_ADC_H_
#define FUELCELL_ADC_H_



void ADCInit(void);
void StartADC_Sequencers(void);
void ReadADC_Sequencers(void);

int convert_temp(int temp_reading);
int get_FCTEMP(void);
int get_TANKPRES(void);

int get_FCPRES(void);
//do we want to zero this?

/*
int get_CAPCURR(void);
void zero_CAPCURR(void);
*/

int get_CAPVOLT(void);
void zero_CAPVOLT(void);

int get_FCVOLT(void);
void zero_FCVOLT(void);

int get_FCCURR(void);
void zero_FCCURR(void);
/*
int get_AMBTEMP0(void);
int get_AMBTEMP1(void);
int get_AMBTEMP2(void);
int get_AMBTEMP3(void);
*/


#endif /* FUELCELL_ADC_H_ */