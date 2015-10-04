/*
 * myADC.c
 *
 * Created: 2015-05-20 5:15:10 PM
 *  Author: Reegan
 */ 

//look at this http://asf.atmel.com/docs/latest/uc3c/html/group__group__avr32__drivers__adcifa.html#ga2be32ef5904a905f4911558fd38b5300
//and datasheet

#include "myADC.h"
#include <avr32/io.h>

void ADCInit(void)
{
	AVR32_ADCIFA.cfg  &= ~(1 << AVR32_ADCIFA_CFG_ADCEN); //disable ADC
	AVR32_ADCIFA.cfg |= (1 << AVR32_ADCIFA_CFG_SHD); //Sample and Hold disabled
	AVR32_ADCIFA.cfg &= ~(3 << 5); //0.6*VDDANA reference
	AVR32_ADCIFA.cfg &= ~(1 << 3); //single sequencer mode disabled
	AVR32_ADCIFA.cfg &= ~(1 << 2); //power saving mode disabled

	//ADC clock settings
	//need 1.5MHz - 125KHz
	//CKDIV register sets prescaler
	//120MHz clock / 500KHz = 240 divider (I think)
	AVR32_ADCIFA.ckdiv = 240;
	
	
	AVR32_ADCIFA.cr |= (1 << AVR32_ADCIFA_CR_TSTART); //start timer
	
	
	//sequencer 0
	AVR32_ADCIFA.seqcfg0 &= ~(3 << AVR32_ADCIFA_SEQCFG0_SRES); //12 bit resolution
	AVR32_ADCIFA.seqcfg0 &= ~(3 << AVR32_ADCIFA_SEQCFG0_TRGSEL); //software trigger select
	AVR32_ADCIFA.seqcfg0 &= ~(1 << AVR32_ADCIFA_SEQCFG0_SOCB); //single conversion
	AVR32_ADCIFA.seqcfg0 |= (1 << AVR32_ADCIFA_SEQCFG0_SA);//set Software Acknowledge bit
	
	//sequencer 1 //not using
	//AVR32_ADCIFA.seqcfg1 &= ~(3 << AVR32_ADCIFA_SEQCFG0_SRES); //12 bit resolution
	//AVR32_ADCIFA.seqcfg1 &= ~(3 << AVR32_ADCIFA_SEQCFG0_TRGSEL); //software trigger select
	//AVR32_ADCIFA.seqcfg1 &= ~(1 << AVR32_ADCIFA_SEQCFG1_SOCB); //single conversion
	//AVR32_ADCIFA.seqcfg1 |= (1 << AVR32_ADCIFA_SEQCFG1_SA);//set Software Acknowledge bit
	
	AVR32_ADCIFA.cfg  |= (1 << AVR32_ADCIFA_CFG_ADCEN); //enable ADC
}


void SetAsAnalogIn(unsigned int GPIOPort,unsigned int GPIOPin)
{
	//set pin to function A
	AVR32_GPIO.port[GPIOPort].pmr0c = 1 << GPIOPin;
	AVR32_GPIO.port[GPIOPort].pmr1c = 1 << GPIOPin;
	AVR32_GPIO.port[GPIOPort].pmr2c = 1 << GPIOPin;
}

unsigned int AnalogRead(unsigned int SequencerSelect)
{
	//starts conversion and returns value
	//analog multiplexer must be set separately
	//see usage examples
	if (SequencerSelect == 0)
	{	
		//start conversion
		AVR32_ADCIFA.cr |= (1 << AVR32_ADCIFA_CR_SOC0);
		//wait for end of conversion
		while(~AVR32_ADCIFA.sr & (1 << AVR32_ADCIFA_SR_SEOC0))
		{
			//do nothing
		}
		//read value
		//send read acknowledgment :write a 1 in the SEOSx bit of SCR register
		AVR32_ADCIFA.scr |= (1 << AVR32_ADCIFA_SCR_SEOS0);
		//return value
		return(AVR32_ADCIFA.resx[0]);
	}
	else
	{
		return(0);
	}
}
