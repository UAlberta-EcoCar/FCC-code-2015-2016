/*
 * mySerial.h
 *
 * Created: 2015-05-20 6:56:41 PM
 *  Author: Reegan
 */ 


#ifndef MYSERIAL_H_
#define MYSERIAL_H_

void SerialTransmitInit(void);
void Send(unsigned char SendValue);
void SendNumAsASCII(unsigned int Num);
void STARTMESSAGE(void);
void SendDigitalStatus(unsigned int Status);

#endif /* MYSERIAL_H_ */
