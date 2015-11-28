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

#define FCVOLTValue (FCVOLTReading *  (47 + 3) / 3 * 3000 / (2^12 - 1)) //compilier computes conversion factor
//47k and 3k voltage divider
//3V reference
//FCVoltReading = FCVOLTValue * 3000mV/(2^12-1) * (47 + 3) / 3
//should result in a voltage in mV

#define CAPVOLTValue (adcvals_1[4] *  (47 + 3) / 3 * 3000 / (2^12 - 1)) //in mV

#define FCCURRValue ((FCCURRReading * (316 + 470) / 470 * 3000 / (2^12 - 1) - 2500) * 1000 / 40) //in mA 
//from ACS756 datasheet
//is this the X50 or X100?
//if its X50:
//Vout = 2500mV + 40mV/A * I
//I = (Vout - 2500mV)/40mV
//because of 31.6k 47k voltage dividers / 12 bit reading
//Vout = FCCURRReading * 3000/(2^12 - 1) * (31.6 + 47) / 47 mV

#define FCPRESValue ((FCPRESReading * 3000 / (2^12-1) * (470+316) / 470) * 50 - 39700) //in mPSI
//pressure transducer is connected in radiometric mode
//voltage is read through a 47k 31.6k voltage divider
//Vsens = FCPRESReading * 3000 / (2^12 - 1) * (470 + 316) / 470 in mV
//http://www.flowcontrolnetwork.com/how-to-calculate-readings-for-compound-pressure-transducers/
//going off example p range is -14.7 to 185 psi  
//0.5-4.5V radiometric output signal
//psi range is 14.7 +185 = 199.7
//4.5 -0.5 = 4V span
//4/199.7 = 0.020V / PSI
//0.5V = -14.7PSI --> 14.7 psi * 0.020V = 0.294
//v = 0.294 + 0.5 when p = -14.7 + 14.7 = 0 psi
//p = 50(v - 0.5) - 14.7 = 50v - 39.7

#define TANKPRESValue ((TANKPRESReading * 3000 / (2^12-1) * (470+316) / 470) * 50 - 39700) //in mPSI //same sensor???

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
//is there a difference between emergency shutdown and normal shutdown?


#endif /* FUELCELL_FUNCTIONS_H_ */