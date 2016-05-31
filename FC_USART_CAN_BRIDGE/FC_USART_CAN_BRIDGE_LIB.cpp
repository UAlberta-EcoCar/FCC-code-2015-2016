#include <arduino.h>
#include "FC_USART_CAN_BRIDGE_LIB.h"


long long parse_csv(char val,String * dataString)
{  
    String subString = "";
    unsigned char index = 0;
    for(char x = 1; x < val; x++) //find xth csv value
    { 
      index = dataString->indexOf(',',index+1); //find next comma in csv
      if(index == 255) //check for acceptable index (255 means it ran to end of string and quit)
      {
          //exit function return max value
          return(0x7fffffff);
      }
    }

    unsigned char index2 = dataString->indexOf(',',index+1);
    if (index2 - index == 0) //if index is same something goofy happened
    {
        //exit function return max value
        return(0x7fffffff);
    }
    
    subString = dataString->substring(index+1,index2);
    return(subString.toInt());
}
