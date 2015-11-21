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
#define FC_STATE_RUN_PURGE 6

#define ONE_VOLT 1 //define what ADC thinks one volt is for FCVOLT/CAPVOLT
#define FCCUR (FCCURRReading - 2500)/40
//from ACS756 datasheet
//is this the X50 or X100?
//if its X50:
//Vout = 2500mV + 40mV/A * I
//I = (Vout - 2500mV)/40mV

//because of voltage dividers / 12 bit reading
//Vout = FCCURRReading * (31.6 + 47)/47 / (2^12 - 1) = FCCURRReading * 393/(235*4095)

#define PURGE_INTEGRATION_INTERVAL 10
#define PURGE_THRESHOLD 0 * 1000 //get this value from fuel cell documentation. units AMP * mSEC
#define PURGE_TIME 0 //get this value from datasheet
#define MAX_PURGE_INTERVAL 0 //max time between purges regardless of current output

//state functions
unsigned int FC_standby(void);
unsigned int FC_startup_h2(void);
unsigned int FC_startup_purge(void);
unsigned int FC_startup_charge(void);

unsigned int FC_run(void);
unsigned int FC_run_purge(void);

unsigned int FC_shutdown(void);
//is there a difference between emergency shutdown and normal shutdown?


#endif /* FUELCELL_FUNCTIONS_H_ */