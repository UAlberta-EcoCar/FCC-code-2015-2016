/*
 * IncFile1.h
 *
 * Created: 2015-10-10 1:47:04 PM
 *  Author: Reegan
 */ 


#ifndef FUELCELL_OUTPUTS_H
#define FUELCELL_OUTPUTS_H


#define OP1_FAN 0x00000004



#define OP_GPERS AVR32_GPIO.port[3].gpers
#define OP_ODERS AVR32_GPIO.port[3].oders
#define OP_OVRS AVR32_GPIO.port[3].ovrs
#define OP_OVRC AVR32_GPIO.port[3].ovrc
#define OP_OVRT AVR32_GPIO.port[3].ovrt
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


//Inputs
#define AMBTEMP4 AVR32_ADCIN14_PIN
#define AMBTEMP3 AVR32_ADCIN13_PIN
#define AMBTEMP2 AVR32_ADCIN12_PIN
#define AMBTEMP1 AVR32_ADCIN11_PIN
#define FCTEMP2 AVR32_ADCIN10_PIN
#define FCTEMP1 AVR32_ADCIN9_PIN
#define MFLOW AVR32_ADCIN8_PIN



#define DLED_GPERS AVR32_GPIO.port[1].gpers
#define DLED_ODERS AVR32_GPIO.port[1].oders
#define DLED_OVRS AVR32_GPIO.port[1].ovrs
#define DLED_OVRC AVR32_GPIO.port[1].ovrc
#define DLED_OVRT AVR32_GPIO.port[1].ovrt
#define LED0 (1 << 21)
#define LED1 (1 << 30)
#define LED2 (1 << 19)
#define LED3 (1 << 20)

//status leds //on two different ports
//port c
#define LED_START 1
#define LED_ERROR (1 << 2)
#define LED_STOP (1 << 3)
//port b
#define LED_RUN (1 << 31)

#define LED_START_ON AVR32_GPIO.port[2].ovrs = LED_START;
#define LED_START_OFF AVR32_GPIO.port[2].ovrc = LED_START;
#define LED_ERROR_ON AVR32_GPIO.port[2].ovrs = LED_ERROR;
#define LED_ERROR_OFF AVR32_GPIO.port[2].ovrc = LED_ERROR;
#define LED_STOP_ON AVR32_GPIO.port[2].ovrs = LED_STOP;
#define LED_STOP_OFF AVR32_GPIO.port[2].ovrc = LED_STOP;

#define LED_RUN_ON AVR32_GPIO.port[1].ovrs = LED_RUN;
#define LED_RUN_OFF AVR32_GPIO.port[1].ovrc = LED_RUN;

void EnableLeds(void);






#endif /* INCFILE1_H_ */