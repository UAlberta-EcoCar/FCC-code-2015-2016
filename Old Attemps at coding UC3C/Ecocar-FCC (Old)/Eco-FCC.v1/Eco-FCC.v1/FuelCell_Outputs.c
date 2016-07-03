#include "FuelCell_Outputs.h"
#include <avr32/io.h>
#include "myPWM.h"

//I just noticed that the documentation includes zero information on outputs that actually control the fuel cell


void SetDigitalOut(unsigned int GPIONUM, unsigned int GPIOPin);
void OutputsInit(void);

void SetDigitalOut(unsigned int GPIOPort, unsigned int GPIOPin)
{
	AVR32_GPIO.port[GPIOPort].gpers = 1 << GPIOPin; //Enable GPIO control
	AVR32_GPIO.port[GPIOPort].oders = 1 << GPIOPin; //enable output driver
}

void OutputsInit(void)
{
	//Setup digital io's
	
	//LED0
	//PB21 = GPIO 53. 53 / 32 = GPIO port 2. 53 - 32 = P21
	SetDigitalOut(2,21);
	
	//LED1
	//PB30 = GPIO 62. 62 / 32 = GPIO port 2. 62 - 32 = P30
	SetDigitalOut(2,30);
	
	//LED2
	//PB19 = GPIO port 2, P19
	SetDigitalOut(2,19);
	
	//LED3
	//PB20 = GPIO port 2, P20
	SetDigitalOut(2,20);
	
	//OP1
	//PD30 = GPIO port 4 pin 30
	SetDigitalOut(4,30);
	
	//OP2
	//PD29
	SetDigitalOut(4,29);
	
	//OP3
	//PD28
	SetDigitalOut(4,29);
	
	//OP4
	//PD27
	SetDigitalOut(4,27);
	
	//OP5
	//PD24
	SetDigitalOut(4,24);
	
	//OP6
	//PD12
	SetDigitalOut(4,12);
	
	//OP7
	//PD11
	SetDigitalOut(4,11);
	
	//OP8
	//PD10
	SetDigitalOut(4,10);
	
	//OP9
	//PD9
	SetDigitalOut(4,9);
	
	//OP10
	//PD8
	SetDigitalOut(4,8);
}



void SetFAN_OP(unsigned int FanSetting)
{
	//PWM output to the fan.
	//PC13	
}

void SetSRV_OP(unsigned int ServoSetting)
{
	//PWM output to the enclosure fan servo	
	//PC11
}

void LED0(unsigned int LEDInput)
{
	//PB21 = GPIO port 2, P21
	if(LEDInput)
	{
		AVR32_GPIO.port[2].ovrs = 1 << 21; //set output value register
	}
	else
	{
		AVR32_GPIO.port[2].ovrc = 1 << 21; //clear it
	}
}

void LED1(unsigned int LEDInput)
{
	//PB30 = GPIO port 2, P30
	if (LEDInput)
	{
		AVR32_GPIO.port[2].ovrs = 1 << 30;
	}
	else
	{
		AVR32_GPIO.port[2].ovrc = 1 << 30;
	}
}

void LED2(unsigned int LEDInput)
{
	//PB19 = GPIO port 2, P19
	if (LEDInput)
	{
		AVR32_GPIO.port[2].ovrs = 1 << 19;
	}
	else
	{
		AVR32_GPIO.port[2].ovrc = 1 << 19;
	}
}

void LED3(unsigned int LEDInput)
{
	//PB20 = GPIO port 2, P20
	if (LEDInput)
	{
		AVR32_GPIO.port[2].ovrs = 1 << 20;
	}
	else
	{
		AVR32_GPIO.port[2].ovrc = 1 << 20;
	}
}

//General 12V outputs (for driving coils/relays/valves)
void OP1(unsigned int Input)
{
	//PD30 = GPIO port 4 pin 30
	if (Input)
	{
		AVR32_GPIO.port[4].ovrs = 1 << 30;
	}
	else
	{
		AVR32_GPIO.port[4].ovrc = 1 << 30;
	}
}

void OP2 (unsigned int Input)
{
	//PD29 = GPIO port 4 pin 29
	if (Input)
	{
		AVR32_GPIO.port[4].ovrs = 1 << 29;
	}
	else
	{
		AVR32_GPIO.port[4].ovrc = 1 << 29;
	}
}

void OP3 (unsigned int Input)
{
	//PD28 = gpio port 4 pin 28
	if (Input)
	{
		AVR32_GPIO.port[4].ovrs = 1 << 28;
	}
	else
	{
		AVR32_GPIO.port[4].ovrc = 1 << 28;
	}
}

void OP4 (unsigned int Input)
{
	//PD27 = GPIO port 4 pin 27
	if (Input)
	{
		AVR32_GPIO.port[4].ovrs = 1 << 27;
	}
	else
	{
		AVR32_GPIO.port[4].ovrc = 1 << 27;
	}
}

void OP5 (unsigned int Input)
{
	//PD24
	if (Input)
	{
		AVR32_GPIO.port[4].ovrs = 1 << 24;
	}
	else
	{
		AVR32_GPIO.port[4].ovrc = 1 << 24;
	}
}

void OP6 (unsigned int Input)
{
	//PD12
	if(Input)
	{
		AVR32_GPIO.port[4].ovrs = 1 << 12;
	}
	else
	{
		AVR32_GPIO.port[4].ovrc = 1 << 12;
	}
}

void OP7 (unsigned int Input)
{
	//PD11
	if(Input)
	{
		AVR32_GPIO.port[4].ovrs = 1 << 11;
	}
	else
	{
		AVR32_GPIO.port[4].ovrc = 1 << 11;
	}
}

void OP8 (unsigned int Input)
{
	//PD10
	if (Input)
	{
		AVR32_GPIO.port[4].ovrs = 1 << 10;
	}
	else
	{
		AVR32_GPIO.port[4].ovrc = 1 << 10;
	}
}

void OP9 (unsigned int Input)
{
	//PD9
	if (Input)
	{
		AVR32_GPIO.port[4].ovrs = 1 << 9;
	}
	else
	{
		AVR32_GPIO.port[4].ovrc = 1 << 9;
	}
}

void OP10 (unsigned int Input)
{
	//PD8
	if (Input)
	{
		AVR32_GPIO.port[4].ovrs = 1 << 8;
	}
	else
	{
		AVR32_GPIO.port[4].ovrc = 1 << 8;
	}
}