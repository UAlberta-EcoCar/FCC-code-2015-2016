#ifndef CAN_MESSAGE_DEFS_H
#define CAN_MESSAGE_DEFS_H
//Defines for all messages that will go over our CAN-BUS
//All messages will be defined here to avoid conflicts
//11 bit identifier = valid id's are 0 to 2047
//messages are sent according to priority, 0 is highest priority
//keep message priorities spreadout for future additions


//when brake pedal is hit it is a high priority message
#define MESSAGE_BRAKE_ID 15
#define MESSAGE_BRAKE_LENGTH 1
#define MESSAGE_BRAKE_DATA data.u8[0]
#define BRAKE_BIT 0


//FC ERROR message
#define MESSAGE_FC_ID_ERROR 50
#define MESSAGE_FC_LENGTH_ERROR 4
#define MESSAGE_FC_DATA_ERROR data.u32[0]


//just reading the gas pedal isn't that high of priority
#define MESSAGE_GAS_PEDAL_ID 150
#define MESSAGE_GAS_PEDAL_LENGTH 1
#define GAS_PEDAL_MSB 0
#define GAS_PEDAL_LSB 1


//FC Data Logging: low priority

//Fuelcell temperature mK
#define MESSAGE_FC_ID_FCTEMP 1500
#define MESSAGE_FC_LENGTH_FCTEMP 8
#define MESSAGE_FC_DATA_FCTEMP1 data.u32[0]
#define MESSAGE_FC_DATA_FCTEMP2 data.u32[1]

//Fuelcell current and voltage mA and mV
#define MESSAGE_FC_ID_FCVOLT_FCCURR 1510
#define MESSAGE_FC_LENGTH_FCVOLT_FCCURR 8
#define MESSAGE_FC_DATA_FCVOLT data.u32[0]
#define MESSAGE_FC_DATA_FCVOLT data.u32[1]


//Super capacitor volt and current mV mA
#define MESSAGE_FC_ID_CAPVOLT_CAPCURR 1515
#define MESSAGE_FC_LENGTH_CAPVOLT_CAPCURR 8
#define MESSAGE_FC_DATA_CAPVOLT data.u32[0]


#define MESSAGE_FC_ID_MFLOW 1520
#define MESSAGE_FC_LENGTH_MFLOW 4
#define MESSAGE_FC_DATA_MFLOW data.u32[0]

#define MESSAGE_FC_ID_FCPRES_TANKPRES 1525
#define MESSAGE_FC_LENGTH_FCPRES_TANKPRES 8
#define MESSAGE_FC_DATA_FCPRES data.u32[0]
#define MESSAGE_FC_DATA_TANKPRES data.u32[1]

#define MESSAGE_FC_ID_AMBTEMP12 1530
#define MESSAGE_FC_LENGTH_AMBTEMP12 8
#define MESSAGE_FC_DATA_AMBTEMP1 data.u32[0]
#define MESSAGE_FC_DATA_AMBTEMP2 data.u32[1]

#define MESSAGE_FC_ID_AMBTEMP34 1535
#define MESSAGE_FC_LENGTH_AMBTEMP34 8
#define MESSAGE_FC_DATA_AMBTEMP3 data.u32[0]
#define MESSAGE_FC_DATA_AMBTEMP4 data.u32[1]

//Fuelcell state
#define MESSAGE_FC_ID_FCSTATE 1540
#define MESSAGE_FC_LENGTH_FCSTATE 4
#define MESSAGE_DATA_FCSTATE data.u32[0]

#endif