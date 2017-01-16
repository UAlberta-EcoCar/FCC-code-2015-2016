/*
 * FuelCell_DataLogging.h
 *
 * Created: 2016-01-16 9:07:35 AM
 *  Author: Reegan
 */ 


#ifndef FUELCELL_DATALOGGING_H_
#define FUELCELL_DATALOGGING_H_

#define USART_BRIDGE_INTERVAL 25

void usart_data_logging(unsigned int fc_state, unsigned int error_msg);

#define USART_BRIDGE LOG_USART
void usart_can_bridge(unsigned int fc_state, unsigned int error_msg,unsigned int past_fc_state);

#endif /* FUELCELL_DATALOGGING_H_ */
