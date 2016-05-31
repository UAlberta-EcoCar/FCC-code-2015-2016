//I quit attempting to can the FCC can bus module to work
//instead, this sketch will link the CAN-BUS to the FCC usart module


#include <mcp2515_lib.h>
#include "fc_can_messages.h"
#include <mcp2515_filters.h>
#include "FC_USART_CAN_BRIDGE_LIB.h"


const int CAN_STATUS_LED = 2;

const int MSG_STATUS_LED  = 3;

void setup() {
  pinMode(CAN_STATUS_LED,OUTPUT);
  pinMode(MSG_STATUS_LED,OUTPUT);
  
  // Start Serial Communication
  Serial.begin(9600);
  while (!Serial);
  Serial.setTimeout(500);

  //start CAN-bus
  while(can_init(0,0,0,0,0,0,0,0))
  {
    Serial.println("mcp2515 init error");
  }
  
  digitalWrite(CAN_STATUS_LED,HIGH);
  
  // Short delay and then begin listening
  delay(2000);
}

String dataString;

void loop() 
{
    delay(450);
    digitalWrite(MSG_STATUS_LED,LOW);
    dataString = "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,1,1,1,1,1,1";
    Serial.println("String Recieved");
//    dataString = Serial.readStringUntil('\n'); //note this has to be single quotes
    //it is a character -> double " is a string

    if(dataString == "")
    {
      
    }
    else
    {
      digitalWrite(MSG_STATUS_LED,HIGH);
      //run through everything and send over CAN-BUS

      //read cvs values
      uint16_t fc_error = parse_csv(FC_ERROR_CSV,&dataString);
      uint8_t fc_state = parse_csv(FC_STATE_CSV,&dataString);
      
      uint8_t fc_purge_count = parse_csv(FC_PURGE_COUNT_CSV,&dataString);
      uint32_t fc_time_between_last_purges = parse_csv(FC_TIME_BETWEEN_LAST_PURGES_CSV,&dataString);
      uint32_t fc_energy_since_last_purge = parse_csv(FC_ENERGY_SINCE_LAST_PURGE_CSV,&dataString);
      uint32_t fc_total_energy = parse_csv(FC_TOTAL_ENERGY_CSV,&dataString);
      uint32_t fc_charge_since_last_purge = parse_csv(FC_CHARGE_SINCE_LAST_PURGE_CSV,&dataString);
      uint32_t fc_total_charge = parse_csv(FC_TOTAL_CHARGE_CSV,&dataString);
      Serial.println(fc_total_charge);

      int32_t fcvolt = parse_csv(FCVOLT_CSV,&dataString);
      int32_t fccurr = parse_csv(FCCURR_CSV,&dataString);
      int32_t capvolt = parse_csv(CAPVOLT_CSV,&dataString);
      uint8_t fctemp = parse_csv(FCTEMP_CSV,&dataString);
      uint8_t opttemp = parse_csv(OPTTEMP_CSV,&dataString);
      int32_t fcpres = parse_csv(FCPRES_CSV,&dataString);
      int32_t fc_fan_speed = parse_csv(FC_FAN_SPEED_CSV,&dataString);

      bool fc_start_relay = parse_csv(FC_START_RELAY_CSV,&dataString);
      bool fc_res_relay = parse_csv(FC_RES_RELAY_CSV,&dataString);
      bool fc_cap_relay = parse_csv(FC_CAP_RELAY_CSV,&dataString);
      bool fc_motor_relay = parse_csv(FC_MOTOR_RELAY_CSV,&dataString);
      bool fc_purge_valve = parse_csv(FC_PURGE_VALVE_CSV,&dataString);
      bool fc_h2_valve = parse_csv(FC_H2_VALVE_CSV,&dataString);
      
      send_fc_error(fc_error);
      send_fc_state(fc_state);
      delay(10);
      
      send_fc_purge_count(fc_purge_count);
      send_fc_time_between_last_purges(fc_time_between_last_purges);
      delay(10);
      send_fc_energy(fc_total_energy,fc_energy_since_last_purge);
      send_fc_charge(fc_total_charge,fc_charge_since_last_purge);
      delay(10);
      
      send_fc_volt(fcvolt);
      send_fc_curr(fccurr);
      delay(10);
      send_fc_capvolt(capvolt);
      delay(10);
      send_fc_temp(fctemp,opttemp);
      send_fc_pres(fcpres);
      delay(10);
      send_fc_fan_speed(fc_fan_speed);
      send_fc_outputs(fc_start_relay,fc_res_relay,fc_cap_relay,fc_motor_relay,fc_purge_valve,fc_h2_valve);
    }

    if(digitalRead(9) == 0)
    {
      //message recieved
      can_get_message();
      digitalWrite(CAN_STATUS_LED,HIGH);
    }
    else
    {
      digitalWrite(CAN_STATUS_LED,LOW);
    }
}
