#include <stdio.h>
#include "can_message.h"
#include "can_message_def.h"
#include "fc_can_messages.h"
#include <mcp2515_lib.h>
#include <arduino.h>

/*
* Create CAN Message Descriptors
*/
//can_msg::MsgEncode MSG_NAME( can_msg::TYPE, can_msg::DEVICE, can_msg::MSG, can_msg::PRIOR, LEN)

//FC_ERROR
can_msg::MsgEncode fc_error_msg( can_msg::UINT16, can_msg::FUEL_CELL, can_msg::FC_ERROR, can_msg::CRITICAL, 1 );

//FC_STATE
can_msg::MsgEncode fc_state_msg( can_msg::UINT8, can_msg::FUEL_CELL, can_msg::FC_STATE, can_msg::IMPORTANT, 1 );

//FC_PURGE_COUNT
can_msg::MsgEncode fc_purge_count_msg( can_msg::UINT8, can_msg::FUEL_CELL, can_msg::FC_PURGE_COUNT, can_msg::LOGGING, 1 );

//FC_TIME_BETWEEN_LAST_PURGES
can_msg::MsgEncode fc_time_between_last_purges_msg( can_msg::UINT32, can_msg::FUEL_CELL, can_msg::FC_TIME_BETWEEN_LAST_PURGES, can_msg::LOGGING, 1 );

//FC_ENERGY
can_msg::MsgEncode fc_energy_msg( can_msg::UINT32, can_msg::FUEL_CELL, can_msg::FC_ENERGY, can_msg::LOGGING, 2 );

//FC_CHARGE
can_msg::MsgEncode fc_charge_msg( can_msg::UINT32, can_msg::FUEL_CELL, can_msg::FC_CHARGE, can_msg::LOGGING, 2);

//FCVOLT
can_msg::MsgEncode fc_volt_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_VOLT, can_msg::INFORMATION, 1);

//FCCURR
can_msg::MsgEncode fc_curr_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_CURR, can_msg::INFORMATION, 1);

//FCTEMP
can_msg::MsgEncode fc_temp_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_TEMP, can_msg::INFORMATION, 1);

//FCPRES
can_msg::MsgEncode fc_pres_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_PRES, can_msg::INFORMATION, 1);

//CAPVOLT
can_msg::MsgEncode fc_capvolt_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_CAPVOLT, can_msg::INFORMATION, 1);

//FC_FAN_SPEED
can_msg::MsgEncode fc_fan_speed_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_FAN_SPEED, can_msg::INFORMATION, 1);

//OUTPUTS
can_msg::MsgEncode fc_outputs_msg( can_msg::BOOL, can_msg::FUEL_CELL, can_msg::FC_OUTPUTS, can_msg::INFORMATION, 6);


/*
* Create functions for sending the messages
*/


//FC_ERROR
void send_fc_error(uint16_t error)
{
	//make a messages
	CanMessage msg;
	msg.id = fc_error_msg.id();
	msg.length = fc_error_msg.len();
	fc_error_msg.buf(msg.data, error);
	while(can_send_message(&msg));
}

//FC_STATE
void send_fc_state(uint8_t state)
{
	//make a messages
	CanMessage msg;
	msg.id = fc_state_msg.id();
	msg.length = fc_state_msg.len();
	fc_state_msg.buf(msg.data, state);
	while(can_send_message(&msg));
}

//FC_PURGE_COUNT
void send_fc_purge_count(uint8_t count)
{
	//make a messages
	CanMessage msg;
	msg.id = fc_purge_count_msg.id();
	msg.length = fc_purge_count_msg.len();
	fc_purge_count_msg.buf(msg.data, count);
	while(can_send_message(&msg));
}

//FC_TIME_BETWEEN_LAST_PURGES
void send_fc_time_between_last_purges(uint32_t time_var)
{
	//make a messages
	CanMessage msg;
	msg.id = fc_time_between_last_purges_msg.id();
	msg.length = fc_time_between_last_purges_msg.len();
	fc_time_between_last_purges_msg.buf(msg.data, time_var);
	while(can_send_message(&msg));
}

//FC_ENERGY TOTAL / SINCE_LAST_PURGE
void send_fc_energy(uint32_t total, uint32_t last)
{
	//make a messages
	CanMessage msg;
	msg.id = fc_energy_msg.id();
	msg.length = fc_energy_msg.len();
	fc_energy_msg.buf(msg.data, total << can_msg::FC_TOTAL_ENERGY | last << can_msg::FC_ENERGY_SINCE_LAST_PURGE);
	while(can_send_message(&msg));
}



//FC_CHARGE TOTAL/SINCE_LAST_PURGE
void send_fc_charge(uint32_t total, uint32_t last)
{
	//make a messages
	CanMessage msg;
	msg.id = fc_charge_msg.id();
	msg.length = fc_charge_msg.len();
	fc_charge_msg.buf(msg.data, (total << can_msg::FC_TOTAL_CHARGE | last << can_msg::FC_CHARGE_SINCE_LAST_PURGE));
	while(can_send_message(&msg));
}


//FCVOLT
void send_fc_volt(int32_t volt)
{
	//make a messages
	CanMessage msg;
	msg.id = fc_volt_msg.id();
	msg.length = fc_volt_msg.len();
	fc_volt_msg.buf(msg.data, volt);
	while(can_send_message(&msg));
}

//FCCURR
void send_fc_curr(int32_t curr)
{
	//make a messages
	CanMessage msg;
	msg.id = fc_curr_msg.id();
	msg.length = fc_curr_msg.len();
	fc_curr_msg.buf(msg.data, curr);
	while(can_send_message(&msg));
}

//FCTEMP
void send_fc_temp(int32_t temp)
{
	//make a messages
	CanMessage msg;
	msg.id = fc_temp_msg.id();
	msg.length = fc_temp_msg.len();
	fc_temp_msg.buf(msg.data, temp);
	while(can_send_message(&msg));
}

//FCPRES
void send_fc_pres(int32_t pres)
{
	//make a messages
	CanMessage msg;
	msg.id = fc_pres_msg.id();
	msg.length = fc_pres_msg.len();
	fc_pres_msg.buf(msg.data, pres);
	while(can_send_message(&msg));
}

//FC_CAPVOLT
void send_fc_capvolt(int32_t volt)
{
	//make a messages
	CanMessage msg;
	msg.id = fc_capvolt_msg.id();
	msg.length = fc_capvolt_msg.len();
	fc_capvolt_msg.buf(msg.data, volt);
	while(can_send_message(&msg));
}


//FC_FAN_SPEED
void send_fc_fan_speed(int32_t speed)
{
	//make a messages
	CanMessage msg;
	msg.id = fc_fan_speed_msg.id();
	msg.length = fc_fan_speed_msg.len();
	fc_fan_speed_msg.buf(msg.data, speed);
	while(can_send_message(&msg));
}

//FC_OUTPUTS
void send_fc_outputs(int8_t start, int8_t res, int8_t cap,int8_t motor,int8_t purge,int8_t h2)
{
	CanMessage msg;
	msg.id = fc_outputs_msg.id();
	msg.length = fc_outputs_msg.len();
	fc_outputs_msg.buf(msg.data, start << can_msg::FC_START_RELAY | res << can_msg::FC_RES_RELAY | cap << can_msg::FC_CAP_RELAY | motor << can_msg::FC_MOTOR_RELAY | purge << can_msg::FC_PURGE_VALVE | h2 << can_msg::FC_PURGE_VALVE);
	while(can_send_message(&msg));
}