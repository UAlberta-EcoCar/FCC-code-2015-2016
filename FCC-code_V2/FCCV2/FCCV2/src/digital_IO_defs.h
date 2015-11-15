/*
 * IncFile1.h
 *
 * Created: 2015-10-10 1:47:04 PM
 *  Author: Reegan
 */ 


#ifndef FUELCELL_IO_H
#define FUELCELL_IO_H


//digital Inputs
#define START AVR32_PIN_PB03
#define SYSOK AVR32_PIN_PB02
#define FCCON AVR32_PIN_PB23
#define CAPCON AVR32_PIN_PC00
#define RESCON AVR32_PIN_PB22



//LEDs
#define LED0 AVR32_PIN_PB21
#define LED1 AVR32_PIN_PB30
#define LED2 AVR32_PIN_PB19
#define LED3 AVR32_PIN_PB20
#define LED_RUN AVR32_PIN_PB31
#define LED_START AVR32_PIN_PC01
#define LED_ERROR AVR32_PIN_PC02
#define LED_STOP AVR32_PIN_PC03

//Coil driving outputs
#define OP_1 AVR32_PIN_PD30
#define OP_2 AVR32_PIN_PD29
#define OP_3 AVR32_PIN_PD28
#define OP_4 AVR32_PIN_PD27
#define OP_5 AVR32_PIN_PD24
#define OP_6 AVR32_PIN_PD12
#define OP_7 AVR32_PIN_PD11
#define OP_8 AVR32_PIN_PD10
#define OP_9 AVR32_PIN_PD09
#define OP_10 AVR32_PIN_PD08

//digital outputs
#define H2_VALVE OP_10
#define START_RELAY OP_1
#define PURGE_VALVE OP_9
#define RES_RELAY OP_2
#define CAP_RELAY OP_3
#define MOTOR_RELAY OP_4



#endif /* INCFILE1_H_ */