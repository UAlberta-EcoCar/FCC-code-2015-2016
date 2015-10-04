#include "FuelCell_Inputs.h"
#include <avr32/io.h>
#include "myADC.h"

void InputsInit (void)
{
	//set DDR for all inputs
	//page 11 of datasheet needed
	
	//pins default as digital inputs
	
	//set analog inputs
	SetAsAnalogIn(1,5); //set PA5 as ADCIN1
	SetAsAnalogIn(1,7); //set PA7 as ADCIN3
	SetAsAnalogIn(1,8); //set PA8 as ADCIN4
	SetAsAnalogIn(1,9); //set PA9 as ADCIN5
	SetAsAnalogIn(1,10); //set PA10 as ADCIN6
	SetAsAnalogIn(1,11); //set PA11 as ADCIN7
	//maybe set up interrupts
	
	//set up ADC
	ADCInit();
}

unsigned int ReadSTART(void)
{
	//The START signal tiggers the Fuel Cell Controller
	//to go into its startup mode
	//Connected to PB3 = GPIO port 2 pin 3
	return(AVR32_GPIO.port[2].pvr & (1 << 3)); //returns zero if clear and !=0 if set
}

unsigned int ReadSYSOK(void)
{
	//The SYSOK signal indicates that the system 
	//is in proper operating condition. 
	//It should be shorted to ground in case of an 
	//emergency stop event such as an emergency 
	//stop button press, or unsafe hydrogen level.
	//Connected to PB2
	//Should Probably be an interrupt
	return(0); //to remove compiler warning
}

unsigned int ReadTANKPRES(void)
{
	//The TANKPRES signal indicates the pressure 
	//in the line from the hydrogen tank.	
	//S- PA4 unused. Analog input
	//PA5 Attached to S- with 10k res. Analog input
	
	//ADCIN1
	//using sequencer 0
	AVR32_ADCIFA.inpsel00 = 1; //conversion 1, adcin1, seq 0. see page 1146 of datasheet	
	return(AnalogRead(0));
}

unsigned int ReadFCPRES(void)
{
	//The FCPRES signal indicates the hydrogen pressure 
	//at the input to the fuel cell.
	//PA6 unused
	//PA7 Analog input
	AVR32_ADCIFA.inpsel00 = 3; //conversion 1, adcin3, seq 0. see page 1146 of datasheet
	return(AnalogRead(0));
}

unsigned int ReadFAN_TAC(void)
{
	//PD2. Fan Tachometer
	return(0); //to remove compiler warning
}

unsigned int ReadFCCURR(void)
{
	//The FCCUR current sensor reports the current flowing from the fuel cell	
	//PA9 Analog input
	AVR32_ADCIFA.inpsel00 = 5; //conversion 1, adcin5, seq 0. see page 1146 of datasheet
	return(AnalogRead(0));
}

unsigned int ReadFCVOLT(void)
{
	//Reads fuel cell voltage. voltage divider
	//PA11
	AVR32_ADCIFA.inpsel00 = 7; //conversion 1, adcin 7, seq 0. see page 1146 of data sheet
	return(AnalogRead(0));
}

unsigned int ReadFCCON(void)
{
	//determines if the fuel cell is connected
	//PB23 = GPIO port 2 pin 23
	return(AVR32_GPIO.port[2].pvr & (1 << 23));
}

unsigned int ReadCAPCURR(void)
{
	//The CAPCUR current sensor reports the current flowing unsigned into or out of the ultra capacitors.
	//PA8 Analog input
	AVR32_ADCIFA.inpsel00 = 4; //conversion 1, adcin 4, seq 0. see page 1146 of data sheet
	return(AnalogRead(0));
}

unsigned int ReadCAPCON(void)
{
	//determines if the ultra capacitors are connected
	//PC0 = GPIO port 3 pin 0
	return(AVR32_GPIO.port[3].pvr & 1);
}

unsigned int ReadCAPVOLT(void)
{
	//read the ultra capacitor voltage
	//PA10
	AVR32_ADCIFA.inpsel00 = 6;
	return(AnalogRead(0));
}

unsigned int ReadRESCON(void)
{
	//determines if the start-up power resistors are connected
	//PB22 = GPIO port 2 pin 22
	return(AVR32_GPIO.port[2].pvr & (1 << 22));
}

unsigned int ReadAMBTEMP1(void)
{
	//PA22
	return(0); //to remove warning
}

unsigned int ReadAMBTEMP2(void)
{
	//PA23
	return(0); //to remove warning
}

unsigned int ReadAMBTEMP3(void)
{
	//PA24
	return(0); //to remove warning
}

unsigned int ReadAMBTEMP4(void)
{
	//PA25
	return(0); //to remove warning
}

unsigned int ReadFCTEMP1(void)
{
	//PA20
	return(0); //to remove warning
}

unsigned int ReadFCTEMP2(void)
{
	//PA21
	return(0); //to remove warning
}

unsigned int ReadMFLOW(void)
{
	//PA19
	return(0); //to remove warning
}

