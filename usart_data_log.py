#! /usr/bin/python
#this sketch logs data to a csv file
#it takes care of cleaning out special characters and adding new lines
#however data must be sent over serial in csv format
#also I am having issues with encoding special characters so I am only leting
#CAPITAL letters, numbers, and comma's thru
import serial
import sys
import datetime

now = datetime.datetime.now()

try: 
    file = open('log' + str(now.year) + '_' + str(now.month) + '_' + str(now.day) + '_' + str(now.hour) + '_' + str(now.minute) + '_' + '.csv','w')
    #file.write("New Log File " + str(now.isoformat()) + "\n\r")
    
    ser = serial.Serial('COM6',115200,timeout=1)
    
    accepted_chars = ['1','2','3','4','5','6','7','8','9','0','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',',','-',' ','_']

    print("CTRL+c to quit")

    start_freeze = 1

    while start_freeze == 1:
        line = ser.read(1);
        if line == b'$':
            start_freeze = 0
    
    while 1:
        line2 = ''
        line = str(ser.readline())
        for x in range(0,len(line)):
            if line[x] in accepted_chars:
                line2 = line2 + line[x]
        file.write(str(line2))
        file.write("\n")
        #print(line2)

except:
    file.close()
    ser.close()
