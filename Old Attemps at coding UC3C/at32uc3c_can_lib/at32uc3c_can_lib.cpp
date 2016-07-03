/*
 * at32uc3c_can_lib.cpp
 *
 * Created: 2015-09-18 7:44:52 PM
 *  Author: Reegan
 */ 

#include <avr32/io.h>
#include "at32uc3c_can_settings.h"
#include "at32uc3c_can_lib.h"

// ************************* //
//To do

//complete can_get_message function

//testing

//CANCFG settings for different bit rates
//match can settings for mcp2515

//support for both channels

//message filtering

//***************************//

//allocate ram
MessageObject myMOb[NUM_BUFFERS];

unsigned char can_init(void)
{
	//Set up the clock for CAN
	AVR32_SCIF.gcctrl[CHANNEL_NUM] |= GCLK_CANIF_Setting;
	
	//Set CAN pin settings in GPIO controller
	//PB4 CANRX   PB5 CANTX
	//GPIO port 2 P4 and P5
	//Mode B -> PMR[2,1,0] = 001
	//do both at same time
	AVR32_GPIO.port[2].pmr2 &= ~((1 << 4)|(1 << 5));
	AVR32_GPIO.port[2].pmr1 &= ~((1 << 4)|(1 << 5));
	AVR32_GPIO.port[2].pmr0 |= (1 << 4)|(1 << 5);

		
	//Set CAN timing settings
	AVR32_CANIF.channel[CHANNEL_NUM].cancfg |= CANCFG_Setting;
	
	//Enable CANIF clock
	AVR32_SCIF.gcctrl[CHANNEL_NUM] |= (1 << AVR32_SCIF_GCCTRL_CEN);	
	
	//save the location of the base MOb address to RANCAMB reg.
	AVR32_CANIF.channel[CHANNEL_NUM].canramb = (unsigned int)&myMOb; 

	//Initialize channel
	AVR32_CANIF.channel[CHANNEL_NUM].canctrl |= (1 << AVR32_CANIF_CANCTRL_INIT);
	//this clears all settings except CANRAMB, CANFCG and CANCTRL.INIT
		
	//set up rx buffers
	
	return(0);
}

//Send function
//should be straight forward
unsigned char can_send_message(CanMessage *p_message)
{
	//search for buffers ready to use
	//first "NUM_RX" buffers will be reserved for RX. last i are TX
	for(unsigned char i = NUM_RX;i < NUM_BUFFERS;i++)
	{
		if((AVR32_CANIF.channel[CHANNEL_NUM].mobesr & (1 << i)) == 0)//check for MOb that is not enabled (ei not sending)
		{
			//clear previous transmission interrupt
			((unsigned volatile long*) &(AVR32_CANIF.channel[CHANNEL_NUM].mobscr))[i*3] |= (1 << AVR32_CANIF_MOBSCR_TXOK);
			

			
			//load pmessage into corresponding MOb
			
			//load ID
			myMOb[i].IDT = p_message -> id;
			
			//set if RTR
			if(p_message -> RTransR)
			{
				myMOb[i].IDT |= (1 << 30);
			}
			
			//load data
			for(unsigned char j = 0;j < p_message -> length;j++)
			{
				myMOb[i].data[j] = p_message -> data[j]; 
			}
			
			//set length and set to tx MOb
			((unsigned volatile long*)&(AVR32_CANIF.channel[CHANNEL_NUM].mobctrl))[i*3] = p_message -> length | (1 << AVR32_CANIF_MOBCTRL_DIR);
			
			//enable MOb to send
			AVR32_CANIF.channel[CHANNEL_NUM].mober |= (1 << i);
			
			return(0); //message sent
		}
	}
	
	//no MObs open 
	return(1);
}



CanMessage can_get_message(void)
{
	//make an empty message
	CanMessage p_message;
	
	//check for RX Mob's no longer 	enabled
	//give priority to lower number
	for(unsigned char i = 0; i < NUM_RX;i++)
	{
		if((AVR32_CANIF.channel[CHANNEL_NUM].mobesr & (1 << i)) == 0)
		{
			//check if message was received (RXOK)
			if(((unsigned volatile long*) &(AVR32_CANIF.channel[CHANNEL_NUM].mobscr))[i*3] & (1 << AVR32_CANIF_MOBSCR_RXOK))
			{
				//clear RXOK 
				((unsigned volatile long*) &(AVR32_CANIF.channel[CHANNEL_NUM].mobscr))[i*3] &= ~(1 << AVR32_CANIF_MOBSCR_RXOK);
				//maybe clear other statuses
				
				//load message
				
			}
			//else
				//some sort of error
				
			//re enable MOb
			AVR32_CANIF.channel[CHANNEL_NUM].mober |= (1 << i); 
			
			return(p_message);
		}
	}
	//return empty message
	return(p_message);
}