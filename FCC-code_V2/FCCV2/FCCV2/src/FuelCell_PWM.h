/*
 * FuelCell_PWM.h
 *
 * Created: 2015-11-10 10:17:50 AM
 *  Author: Reegan
 */ 


#ifndef FUELCELL_PWM_H_
#define FUELCELL_PWM_H_

#define STARTSPEED 100
#define MAXSPEED 

void PWMInit(void);
int FANUpdate(unsigned int duty_cycle); // X / 4000


#endif /* FUELCELL_PWM_H_ */