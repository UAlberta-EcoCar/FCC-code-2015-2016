/*
 * FuelCell_DataLogging.h
 *
 * Created: 2016-01-16 9:07:35 AM
 *  Author: Reegan
 */ 


#ifndef FUELCELL_DATALOGGING_H_
#define FUELCELL_DATALOGGING_H_

#define USART_DATA_DISPLAY_INTERVAL 250

void usart_data_display(unsigned int fc_state, unsigned int error_msg);


#endif /* FUELCELL_DATALOGGING_H_ */