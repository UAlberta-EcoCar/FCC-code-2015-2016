#ifndef FC_CAN_MESSAGES_H
#define FC_CAN_MESSAGES_H

void send_fc_error(uint16_t error);

void send_fc_state(uint8_t state);

void send_fc_purge_count(uint8_t count);

void send_fc_time_between_last_purges(uint32_t time_var);

void send_fc_energy(uint32_t total, uint32_t last);

void send_fc_charge(uint32_t total, uint32_t last);

void send_fc_volt(int32_t volt);

void send_fc_curr(int32_t curr);

void send_fc_temp(int32_t temp);

void send_fc_pres(int32_t pres);

void send_fc_capvolt(int32_t volt);

void send_fc_fan_speed(int32_t speed);

void send_fc_outputs(int8_t start, int8_t res, int8_t cap, int8_t motor, int8_t purge, int8_t h2);

#endif