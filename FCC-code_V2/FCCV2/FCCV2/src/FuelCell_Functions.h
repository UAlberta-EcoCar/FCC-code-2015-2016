/*
 * FuelCell_Functions.h
 *
 * Created: 2015-11-09 11:08:25 AM
 *  Author: Reegan
 *
*/
#ifndef FUELCELL_FUNCTIONS_H_


//different fuel cell states
#define FC_STATE_STANDBY 0
#define FC_STATE_SHUTDOWN 1
#define FC_STATE_STARTUP_FANS 2
#define FC_STATE_STARTUP_H2 3
#define FC_STATE_STARTUP_PURGE 4
#define FC_STATE_STARTUP_CHARGE 5
#define FC_STATE_RUN 6
#define FC_STATE_ALARM 8
#define FC_STATE_REPRESSURIZE 9
#define FC_STATE_MANUAL_DEPRESSURIZE 10
#define FC_STATE_AIR_STARVE 11

//different sub states while running
#define FIRST_PURGE_CYCLE 0
#define PURGE_VALVE_OPEN 2
#define PURGE_VALVE_CLOSED 1

#define FANUPDATE_INTERVAL 100

#define PURGE_INTEGRATION_INTERVAL 5
#define PURGE_THRESHOLD 2300000000 //2300 C from fuel cell documentation. units mA * ms
//this is larger than max 32 bit value (nice)
#define PURGE_TIME 200 //get this value from datasheet
//purge on low voltage
//#define PURGE_VOLT_THRES 30000

#define TOTAL_CHARGE_ENERGY_INTEGRATION_INTERVAL 5


//data logging / data retrieval functions
unsigned int get_time_between_last_purges(void);
unsigned int get_number_of_purges(void);
U64 get_total_charge_extracted(void);
U64 get_coulumbs_since_last_purge(void);
U64 get_total_E(void);
U64 get_J_since_last_purge(void);

int calc_opt_temp(void);
int calc_min_temp(void);
int calc_max_temp(void);

//state functions
unsigned int FC_standby(int);
unsigned int FC_startup_fans(void);
unsigned int FC_startup_h2(void);
unsigned int FC_startup_purge(void);
unsigned int FC_startup_charge(int);
unsigned int FC_repressurize(void);
unsigned int FC_manual_depressurize(void);

unsigned int FC_run(void);
unsigned int FC_air_starve(void);

unsigned int FC_shutdown(void);
unsigned int FC_alarm(void);

unsigned int pid_temp_control(void);

#endif /* FUELCELL_FUNCTIONS_H_ */