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
  Serial.begin(115200);
  while (!Serial);
  Serial.setTimeout(500);

  //start CAN-bus
  while(can_init(EXACT_FILTER_MASK,0,0,EXACT_FILTER_MASK,0,0,0,0)) //will ignore all rx messages
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
    digitalWrite(MSG_STATUS_LED,LOW);
    dataString = "";
    dataString = Serial.readStringUntil('\n'); //note this has to be single quotes
    //it is a character -> double " is a string

    if(dataString == "")
    {
      
    }
    else
    {
      digitalWrite(MSG_STATUS_LED,HIGH);
      //run through everything and send over CAN-BUS

      //FC_ERROR
      int16_t error = parse_csv(FC_ERROR_CSV,dataString);
      send_fc_error(error);
    
      //FC_STATE
      uint8_t state = parse_csv(FC_STATE_CSV,dataString);
      send_fc_state(state);

      delay(1);

      //PURGE_COUNT
      uint8_t count = parse_csv(FC_PURGE_COUNT_CSV,dataString);
      send_fc_purge_count(count);
      
      //TIME_BETWEEN_LAST_PURGES
      uint32_t time_var = parse_csv(FC_TIME_BETWEEN_LAST_PURGES_CSV,dataString);
      send_fc_time_between_last_purges(time_var);

      delay(1);

      //ENERGY_SINCE_LAST_PURGE
      uint32_t total_e = parse_csv(FC_ENERGY_SINCE_LAST_PURGE_CSV,dataString);
      //TOTAL_ENERGY
      uint32_t last_e = parse_csv(FC_TOTAL_ENERGY_CSV,dataString);
      send_fc_energy(total_e,last_e);

      delay(1);

      //CHARGE_SINCE_LAST_PURGE
      uint32_t total_q = parse_csv(FC_CHARGE_SINCE_LAST_PURGE_CSV,dataString);
      //TOTAL_CHARGE
      uint32_t last_q = parse_csv(FC_TOTAL_CHARGE_CSV,dataString);
	    send_fc_charge(total_q,last_q);

      delay(1);

      //FCVOLT
      int32_t volt = parse_csv(FCVOLT_CSV,dataString);
  	  send_fc_volt((int32_t) volt);

      //FCCURR
      int32_t curr = parse_csv(FCCURR_CSV,dataString);
  	  send_fc_curr((int32_t) curr);

      delay(1);

      //FCTEMP
      int32_t temp = parse_csv(FCTEMP_CSV,dataString);
  	  send_fc_temp((int32_t) temp);

      //FCPRES
      int32_t pres = parse_csv(FCPRES_CSV,dataString);
  	  send_fc_pres((int32_t) pres);

      delay(1);

      //CAPVOLT
      volt = parse_csv(CAPVOLT_CSV,dataString);
  	  send_fc_capvolt((int32_t) volt);

      //FC_FAN_SPEED
      int32_t f_speed = parse_csv(FC_FAN_SPEED_CSV,dataString);
  	  send_fc_fan_speed((int32_t) f_speed);

      delay(1);

  	  //OUTPUTS
	  
      unsigned char start = parse_csv(FC_START_RELAY_CSV,dataString);
      unsigned char r_relay = parse_csv(FC_RES_RELAY_CSV,dataString);
      unsigned char c_relay = parse_csv(FC_CAP_RELAY_CSV,dataString);
      unsigned char m_relay = parse_csv(FC_MOTOR_RELAY_CSV,dataString);
      unsigned char p_valve = parse_csv(FC_PURGE_VALVE_CSV,dataString);
      unsigned char h_valve = parse_csv(FC_H2_VALVE_CSV,dataString);
      
      if((start | r_relay | c_relay | m_relay | p_valve | h_valve) > 1) //if returned value is not a sign bit there is something wrong
      {
        //set all values to 1 to show there is a problem
        start = 1;
        r_relay = 1;
        c_relay = 1;
        m_relay = 0; //except motor relay, just in case we use that bit to tell motor it can run.
        p_valve = 1;
        h_valve = 1;
      }
  	  send_fc_outputs(start,r_relay,c_relay,m_relay,p_valve,h_valve);

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
