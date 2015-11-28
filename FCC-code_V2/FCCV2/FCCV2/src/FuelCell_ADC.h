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
int * ReadADC_Sequencer0();

int get_FCTEMP1(void);
int get_FCTEMP2(void);
int get_TANKPRES(void);
int get_FCPRES(void);
int get_CAPCURR(void);
int get_CAPVOLT(void);
int get_FCVOLT(void);
int get_FCCURR(void);

#endif /* FUELCELL_ADC_H_ */