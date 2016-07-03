#include "at32uc3c_can_lib.h"
#include <avr32/io.h>

int main (void)
{

	//set up CAN
	can_init(); 
	
	//make new message
	CanMessage message;
	//set message id
	message.id = 7;
	//set message length
	message.length = 1;
	//set first data byte
	message.data[0] = 12;
	//send message
	can_send_message(&message);
	
	return(0);	
}