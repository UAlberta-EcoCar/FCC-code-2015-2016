/*
 * FuelCell_PWM.h
 *
 * Created: 2015-11-10 10:17:50 AM
 *  Author: Reegan
 */ 


#ifndef FUELCELL_PWM_H_
#define FUELCELL_PWM_H_


void PWMInit(void);
int FANUpdate(int duty_cycle); 
unsigned int get_FANSpeed(void);

#endif /* FUELCELL_PWM_H_ */