/*
 * IncFile1.h
 *
 * Created: 2015-09-19 3:47:20 PM
 *  Author: Me
 */ 


#ifndef OUTPUT_DEFS_H
#define OUTPUT_DEFS_H


#define OP1_FAN 0x00000004



#define OP_GPERS AVR32_GPIO.port[3].gpers
#define OP_ODERS AVR32_GPIO.port[3].oders
#define OP_OVRS AVR32_GPIO.port[3].ovrs
#define OP_OVRC AVR32_GPIO.port[3].ovrc
#define OP1 (1 << 30)
#define OP2 (1 << 29)
#define OP3 (1 << 28)
#define OP4 (1 << 27)
#define OP5 (1 << 24)
#define OP6 (1 << 12)
#define OP7 (1 << 11)
#define OP8 (1 << 10)
#define OP9 (1 << 9)
#define OP10 (1 << 8)


#define LED_GPERS AVR32_GPIO.port[1].gpers
#define LED_ODERS AVR32_GPIO.port[1].oders
#define LED_OVRS AVR32_GPIO.port[1].ovrs
#define LED_OVRC AVR32_GPIO.port[1].ovc
#define LED0 (1 << 21)
#define LED1 (1 << 30)
#define LED2 (1 << 19)
#define LED3 (1 << 20)







#endif /* INCFILE1_H_ */