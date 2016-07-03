//Library for using canif module on atmel at32uc3c microcontroller
//written for ualberta ecocar fuel cell controller
//commands almost identical to my mcp2515 library for atmega328p

#ifndef AT32UC3C_CAN_LIB_H
#define AT32UC3C_CAN_LIB_H
//at32uc3c can lib should be almost same as mcp2515 lib at the high level
//inside workings will be very different


//structure for building can messages to send to can_send_message()
typedef struct
{
    unsigned int id;
    unsigned char RTransR; //defaults to not a remote transmit request
    unsigned char length;
	unsigned char data[8];
} CanMessage;

// message object structure
//this is how controller stores messages to send/receive
//the goal -> never see this in main program
struct MessageObject {
	unsigned int IDT;
	unsigned int IDM; //defaults to no receive filter mask ei: anything goes
	unsigned char data[7];
};

//starts can bus
unsigned char can_init(void);

//sends message
unsigned char can_send_message(CanMessage *p_message);

//recieve messages
CanMessage can_get_message(void);

#endif