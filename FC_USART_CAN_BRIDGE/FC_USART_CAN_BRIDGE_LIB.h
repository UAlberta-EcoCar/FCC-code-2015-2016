#ifndef FC_USART_CAN_BRIDGE_H
#define FC_USART_CAN_BRIDGE_H

//#include <arduino.h> //needed for the string definition?

//function for parsing csv
long long parse_csv(char val,String * dataString);

//define all values and the order they are found in csv
//to do: reorder so that levels are sent first

//fc messages
#define FC_ERROR_CSV 2
#define FC_STATE_CSV 3 

//purge data
#define FC_PURGE_COUNT_CSV 4
#define FC_TIME_BETWEEN_LAST_PURGES_CSV 5
#define FC_ENERGY_SINCE_LAST_PURGE_CSV 6
#define FC_TOTAL_ENERGY_CSV 7
#define FC_CHARGE_SINCE_LAST_PURGE_CSV 8
#define FC_TOTAL_CHARGE_CSV 9

//FC levels
#define FCVOLT_CSV 10
#define FCCURR_CSV 11
#define CAPVOLT_CSV 12
#define FCTEMP_CSV 13
#define OPTTEMP_CSV 14
#define FCPRES_CSV 15
#define FC_FAN_SPEED_CSV 16

//output states
#define FC_START_RELAY_CSV 17
#define FC_RES_RELAY_CSV 18
#define FC_CAP_RELAY_CSV 19
#define FC_MOTOR_RELAY_CSV 20
#define FC_PURGE_VALVE_CSV 21
#define FC_H2_VALVE_CSV 22

#define PAST_FC_STATE 23

#endif
