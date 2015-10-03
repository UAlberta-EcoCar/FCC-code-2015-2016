/*
 * mySerial.c
 *
 * Created: 2015-05-20 6:56:24 PM
 *  Author: Reegan
 */ 

//serial usart functions 
//mostly for debugging
//I'm only going to worry about transmitting for now

#include "mySerial.h"
#include <avr32/io.h>
#include "ASCII_defs.h"

//FCC board gives access to USART3

//define register bits that are not defined for some reason...
#define AVR32_USART_CR_PAR 9
#define AVR32_USART_CR_MSBF 16
#define AVR32_USART_CR_OVER 19
#define AVR32_USART_CR_CHRL 6
#define AVR32_USART_CR_USCLKS 4 //clock selection

void SerialTransmitInit(void) //page 567 of data sheet
{
  //8 bit synchronous serial
  
  //8 bit character length
  AVR32_USART3.cr |= (3 << AVR32_USART_CR_CHRL);
  
  //MSB first
  AVR32_USART3.cr |= (1 << AVR32_USART_CR_MSBF);
  
  //no parity
  AVR32_USART3.cr |= (4 << AVR32_USART_CR_PAR);
  
  //oversampling set to x16 because why not
  AVR32_USART3.cr &= ~(1 << AVR32_USART_CR_OVER);
  
  //unlock some of the configuration registers
  AVR32_USART3.wpmr = (0x555342 << AVR32_USART_WPMR_WPKEY);
  
  //Baud Rate page 639 of data sheet. //not sure if clk_usart is 120Mhz
  //120MHz / 16 / 9600 baudrate = 781.25
  //120MHz / 16 / 781 = 903.07 = 0.03% error
  AVR32_USART3.brgr = (781 << AVR32_USART_BRGR_CD);
  //CLK_USART divided by a value //not needed
  //Divided Clock Mask -- sets clock rate?
  
  //set tx3 pin to tx mode
  //PC17 = GPIO port 3 pin 17: mode F
  AVR32_GPIO.port[3].pmr0s = (1 << 17);
  AVR32_GPIO.port[3].pmr1c = (1 << 17);
  AVR32_GPIO.port[3].pmr2s = (1 << 17);
  
  //enable transmitter
  AVR32_USART3.cr |= (1 << AVR32_USART_CR_TXEN);
}

void Send(unsigned char SendValue)
{
  while((AVR32_USART3.csr & ( 1 << AVR32_USART_CSR_TXRDY)) == 0) //wait for transmitter to be ready
  {
	  //do nothing while waiting
  }
  AVR32_USART3.thr |= (SendValue & 0xff); //write 8 bits to transmit holding register
}

void SendDigitalStatus (unsigned int Status)
{
	Send(SPACE);
	if (Status)
	{
		Send(49); //ASCII 1
	}
	else
	{
		Send(48); //ASCII 0
	}
	Send(SPACE);
}

unsigned char Digit;
void SendNumAsASCII (unsigned int Num)
{
	//12 bit ADC = want to transmit numbers up to approximately 2^12 = 4096 = 4 digits
	
	Send(SPACE);
	
	if (Num <= 9999)
	{
		Digit = (Num / 1000);
		Num = (Num - (Digit * 1000));
		Send(Digit + 48);
	
		Digit = (Num / 100);
		Num = (Num - (Digit * 100));
		Send(Digit + 48);
	
		Digit = (Num / 10);
		Num = (Num - (Digit * 10));
		Send(Digit + 48);
		
		Send(Num + 48);
		
		Send(SPACE);
	}
}

void STARTMESSAGE (void)
{
	Send(0x48);
	Send(0x49);
	Send(LF);
	Send(0x45);
	Send(0x43);
	Send(0x4F);
	Send(0x43);
	Send(0x41);
	Send(0x52);
	Send(SPACE);
	Send(0x46);
	Send(0x43);
	Send(0x43);
	Send(SPACE);
	Send(0x49);
	Send(0x53);
	Send(SPACE);
	Send(0x53);
	Send(0x54);
	Send(0x41);
	Send(0x52);
	Send(0x54);
	Send(0x49);
	Send(0x4E);
	Send(0x47);
	Send(LF);
}