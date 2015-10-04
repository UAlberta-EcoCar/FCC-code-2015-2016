/*
 * myADC.h
 *
 * Created: 2015-05-20 5:15:42 PM
 *  Author: Reegan
 */ 


#ifndef myADC
#define myADC

void ADCInit(void);
void SetAsAnalogIn(unsigned char GPIOPort,unsigned char GPIOPin);
unsigned int AnalogRead(unsigned char SequencerSelect);




#endif 