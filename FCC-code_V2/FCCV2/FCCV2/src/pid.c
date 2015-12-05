#include <stdio.h>
#include <stdlib.h>
#include "pid.h"
#include "asf.h"
#include "FuelCell_Functions.h"

#define P 1
#define I 0.1
#define D 10

float accumulated_error = 0;
float time_called_previously = 0;
float past_temperature = 0;
U32 seconds = 0;
U32 time_passed = 0;


float PID(float currentTemp, float setPoint) {
  seconds = millis() / 1000;

  // proportional part
  float p_value = 0;
  p_value = (currentTemp - setPoint) * P;
  

  // integral part
  time_passed = seconds - time_passed;
  accumulated_error = (currentTemp - past_temperature)*(time_passed) + accumulated_error;
  float i_value = accumulated_error * I;
  
  float d_value = 0;
  d_value = (currentTemp - past_temperature) / (time_passed);
  d_value = d_value * D;

  time_passed = seconds;
  past_temperature = currentTemp;
  //printf("p_value = %f, i_value = %f, d_value = %f\n", p_value, i_value, d_value);
  return p_value + i_value + d_value;
}

void initialize_pid() {
  // initialize variables
  time_passed = millis() / 1000;
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
