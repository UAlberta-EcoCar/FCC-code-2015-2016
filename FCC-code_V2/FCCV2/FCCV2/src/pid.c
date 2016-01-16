#include <stdio.h>
#include <stdlib.h>
#include "pid.h"
#include "asf.h"
#include "FuelCell_Functions.h"
#include "millis_function.h"

#define P 500
#define I 0
#define D 0

int accumulated_error = 0;
unsigned int past_temperature = 0;
unsigned int time_passed = 0;

unsigned int PID(int currentTemp, int setPoint) {
  //don't need mK resolution
  currentTemp = currentTemp >> 10; //fast way to divide by 1000 (2^10 = 1024 ~ 1000 with 2.4% error) (saves a couple microseconds yay!)
  setPoint = setPoint >> 10;  //example 22222/1000 = 22 22222/1024 = 21
  
  //proportional part
  int p_value = 0;
  p_value = (currentTemp - setPoint) * P;

  time_passed = millis() - time_passed; //calculate time difference since last update

  // integral part
  accumulated_error = (currentTemp - setPoint)*(time_passed) + accumulated_error;
  int i_value = accumulated_error * I;
  
  int d_value = 0;
  d_value = (currentTemp - past_temperature) / (time_passed);
  d_value = d_value * D;

  time_passed = millis(); //record past run time
  past_temperature = currentTemp;
  //printf("p_value = %f, i_value = %f, d_value = %f\n", p_value, i_value, d_value);
  setPoint = ((p_value + i_value + d_value) / 10); //scale value down
  
  //precaution against possible negative numbers
  if(setPoint > 0)
  {
	  setPoint = 0;
  }
  return(setPoint);
}

void initialize_pid() {
  // initialize variables
  time_passed = millis();
}


/* int main(int argc, char argv[]) { */
  
/*   int i = 0; */
/*   float setPoint = 75.00; */
/*   time_passed = time(NULL); */
/*   for (i = 0; i < 15; i++) { */
/*     sleep(2); */
/*     printf("currentTemp = %f, setPoint = %f, fanSpeed = %f \n", (float)(i + 60), setPoint, PID((float)(i + 60), setPoint)); */
/*   } */

/*   for (i = 0; i < 15; i++) { */
/*     sleep(2); */
/*     printf("currentTemp = %f, setPoint = %f, fanSpeed = %f \n", (float)(74 - i), setPoint, PID((float)(74 - i), setPoint)); */
/*   }   */


/*   return 0; */
/* } */
