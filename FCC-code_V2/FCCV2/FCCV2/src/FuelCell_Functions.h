/*
 * FuelCell_Functions.h
 *
 * Created: 2015-11-09 11:08:25 AM
 *  Author: Reegan
 */ 

#ifndef FUELCELL_FUNCTIONS_H_
#define FUELCELL_FUNCTIONS_H_

void millis_init(void);
unsigned long millis(void);

unsigned int FC_check_alarms(void);

//different fuel cell states
#define FC_STATE_STANDBY 0
#define FC_STATE_SHUTDOWN 1
#define FC_STATE_STARTUP_H2 2
#define FC_STATE_STARTUP_PURGE 3
#define FC_STATE_STARTUP_CHARGE 4
#define FC_STATE_RUN 5

#define ONE_VOLT 1 //define what ADC thinks one volt is


unsigned int FC_standby(void);
unsigned int FC_startup_h2(void);
unsigned int FC_startup_purge(void);
unsigned int FC_startup_charge(void);


#endif /* FUELCELL_FUNCTIONS_H_ */