/*
 * myADC.h
 *
 * Created: 2015-05-20 5:15:42 PM
 *  Author: Reegan
 */ 


#ifndef myADC
#define myADC

void ADCInit(void);
void SetAsAnalogIn(unsigned int GPIOPort,unsigned int GPIOPin);
unsigned int AnalogRead(unsigned int SequencerSelect);




#endif 