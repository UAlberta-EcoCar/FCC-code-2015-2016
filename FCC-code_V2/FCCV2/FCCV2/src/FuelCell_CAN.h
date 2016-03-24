/*
 * FuelCell_CAN.h
 *
 * Created: 2015-11-20 9:27:14 PM
 *  Author: Reegan
 */ 


#ifndef FUELCELL_CAN_H_
#define FUELCELL_CAN_H_


/* non of this code works

//set up can
void CANInit(void);

//Call Back prototype 
void can_out_callback_channel1(U8 handle, U8 event);

//structure for building can messages to send to can_send_message()
typedef struct
{
	unsigned int     id;
	unsigned char    RTransR;
	unsigned char    length;
	Union64          data;
} CanMessage;

//very rought function for sending messages
//sends can message
U8 can_send_message ( CanMessage *p_message );

//run this function once in a while to prevent mob's from filling up
U8 check_if_message_sent(U8 handle);

*/

#endif /* FUELCELL_CAN_H_ */