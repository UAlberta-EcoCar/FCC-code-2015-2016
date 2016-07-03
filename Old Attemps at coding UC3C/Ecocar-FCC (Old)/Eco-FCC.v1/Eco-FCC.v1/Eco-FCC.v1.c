/*
 * Eco_FCC.c
 * Code for U of Alberta EcoCar Fuel Cell Control Board E-P-206-R1.0
 * Created: 2015-05-16 12:18:16 PM
 *  Author: Reegan
 * 
 */ 

//useful webpages
//https://www.kth.se/social/upload/300/Writing%20your%20own%20program%20%28AVR32%20Studio%29_pm_20100910.pdf
//http://www.atmel.com/Images/doc32005.pdf


//to do:
//get ADC code working
//get PWM code working
//start turning pseudo code into code

//AT32Uc3c164c-aut-nd

#include <avr32/io.h>
#include "FuelCell_Inputs.h"
#include "FuelCell_Outputs.h"
#include "SetConfig.h"
#include "mySerial.h"
#include "myTIMER.h"
#include "myASCII.h"
#include "ast.h"

#define ON 1
#define OFF 0

void Delay (unsigned int Delay_Time)
{
	unsigned int Time1 = millis();
	while((millis() - Time1) <= Delay_Time)
	{
		//do something during delay
		//read inputs
	}
}

void TestLoop(unsigned int DelayTime)
{
	//loop through all outputs to ensure they are working
	//I have not yet written the code to run fans and servos
	LED0(ON);
	Delay(DelayTime);
	LED0(OFF);
	LED1(ON);
	Delay(DelayTime);
	LED1(OFF);
	LED2(ON);
	Delay(DelayTime);
	LED2(OFF);
	LED3(ON);
	Delay(DelayTime);
	LED3(OFF);
	OP1(ON);
	Delay(DelayTime);
	OP1(OFF);
	OP2(ON);
	Delay(DelayTime);
	OP2(OFF);
	OP3(ON);
	Delay(DelayTime);
	OP3(OFF);
	OP4(ON);
	Delay(DelayTime);
	OP4(OFF);
	OP5(ON);
	Delay(DelayTime);
	OP5(OFF);
	OP6(ON);
	Delay(DelayTime);
	OP7(OFF);
	OP8(ON);
	Delay(DelayTime);
	OP8(OFF);
	OP9(ON);
	Delay(DelayTime);
	OP9(OFF);
	OP10(ON);
	Delay(DelayTime);
	OP10(OFF);
	//read all inputs and send readings over serial
	//Digital first
	SendDigitalStatus(ReadSTART());
	SendDigitalStatus(ReadSYSOK());
	SendDigitalStatus(ReadTANKPRES());
	SendDigitalStatus(ReadRESCON());
	SendDigitalStatus(ReadCAPCON());
	Send(LF);
	
	//analog (probably won't work)
	//pressure measurements
	SendNumAsASCII(ReadTANKPRES());
	SendNumAsASCII(ReadFCPRES());
	Send(LF);
	//FC current and voltage
	SendNumAsASCII(ReadFCCURR());
	SendNumAsASCII(ReadFCVOLT());
	Send(LF);
	//Cap current and voltage
	SendNumAsASCII(ReadCAPCON());
	SendNumAsASCII(ReadCAPVOLT());
	Send(LF);
}

int main(void)
{
	ConfigureClock();
	OutputsInit();
	InputsInit();
	SerialTransmitInit();
	SetUpTimer();
	
	
 	
	return 0;
}
