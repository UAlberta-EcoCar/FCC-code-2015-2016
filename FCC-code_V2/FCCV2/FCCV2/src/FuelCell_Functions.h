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

unsigned int FC_check_alarms(unsigned int fc_state);

//different fuel cell states
#define FC_STATE_STANDBY 0
#define FC_STATE_SHUTDOWN 1
#define FC_STATE_STARTUP_FANS 2
#define FC_STATE_STARTUP_H2 3
#define FC_STATE_STARTUP_PURGE 4
#define FC_STATE_STARTUP_CHARGE 5
#define FC_STATE_RUN 6
#define FC_STATE_ALARM 8

//different substates while running
#define FIRST_PURGE_CYCLE 0
#define PURGE_VALVE_OPEN 2
#define PURGE_VALVE_CLOSED 1

#define FANUPDATE_INTERVAL 50

#define PURGE_INTEGRATION_INTERVAL 10 //10 ms
#define PURGE_THRESHOLD 2300*1000*1000 //from fuel cell documentation. units mA * ms
//this is larger than max 32 bit value
#define PURGE_TIME 200 //get this value from datasheet
#define MAX_PURGE_INTERVAL 0 //max time between purges regardless of current output

//state functions
unsigned int FC_standby(void);
unsigned int FC_startup_fans(void);
unsigned int FC_startup_h2(void);
unsigned int FC_startup_purge(void);
unsigned int FC_startup_charge(void);

unsigned int FC_run(void);

unsigned int FC_shutdown(void);
unsigned int FC_alarm(void);

#endif /* FUELCELL_FUNCTIONS_H_ */