# -*- coding: utf-8 -*-
"""
Created on Wed Jun 29 12:22:24 2016

@author: Adnan
"""

import Serial
import datetime


# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
	port='COM3',
	baudrate=57600)
# Previous baudrate (December 2016) = 57600
ser.isOpen()        
filename = datetime.datetime.now().strftime("%Y-%m-%d--%Hh %Mm %Ss %fms")
file_handle = open('%s.csv' %filename, 'w') # opens the csv file
file_handle.write('MILLIS,FC_ERROR,FC_STATE,FC_PURGE_COUNT,FC_TIME_BETWEEN_LAST_PURGES,FC_ENERGY_SINCE_LAST_PURGE,FC_TOTAL_ENERGY,FC_CHARGE_SINCE_LAST_PURGE,FC_TOTAL_CHARGE,FC_VOLT,FC_CURR,FC_CAPVOLT,FC_TEMP,FC_OPTTEMP,FC_PRES,FC_FAN_SPEED,FC_START_RELAY,FC_RES_RELAY,FC_CAP_RELAY,FC_MOTOR_RELAY,FC_PURGE_VALVE,FC_H2_VALVE\n')
data_line = ser.readline()
print(data_line)
file_handle.write(data_line)
while(1):
    data_line = ser.readline()
    print(data_line)
    file_handle.write(data_line)