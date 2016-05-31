#ifndef FC_USART_CAN_BRIDGE_H
#define FC_USART_CAN_BRIDGE_H

//#include <arduino.h> //needed for the string definition?

//function for parsing csv
long long parse_csv(char val,String * dataString);

//define all values and the order they are found in csv
//to do: reorder so that levels are sent first

//fc messages
#define FC_ERROR_CSV 1
#define FC_STATE_CSV 2 

//purge data
#define FC_PURGE_COUNT_CSV 3
#define FC_TIME_BETWEEN_LAST_PURGES_CSV 4
#define FC_ENERGY_SINCE_LAST_PURGE_CSV 5
#define FC_TOTAL_ENERGY_CSV 6 
#define FC_CHARGE_SINCE_LAST_PURGE_CSV 7
#define FC_TOTAL_CHARGE_CSV 8

//FC levels
#define FCVOLT_CSV 9 
#define FCCURR_CSV 10
#define CAPVOLT_CSV 11
#define FCTEMP_CSV 12
#define OPTTEMP_CSV 13
#define FCPRES_CSV 14
#define FC_FAN_SPEED_CSV 15

//output states
#define FC_START_RELAY_CSV 16
#define FC_RES_RELAY_CSV 17
#define FC_CAP_RELAY_CSV 18
#define FC_MOTOR_RELAY_CSV 19
#define FC_PURGE_VALVE_CSV 20
#define FC_H2_VALVE_CSV 21

#endif
