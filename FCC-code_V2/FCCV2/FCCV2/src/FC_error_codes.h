/*
 * FC_error_codes.h
 *
 * Created: 2015-11-14 5:34:49 PM
 *  Author: Reegan
 */ 


#ifndef FC_ERROR_CODES_H_
#define FC_ERROR_CODES_H_

//each error code sets a bit in "error" value
#define FC_ERR_CAP_DISC (1 << 0)
#define FC_ERR_FC_DISC (1 << 1)
#define FC_ERR_RES_DISC (1 << 2)
//note "DISC" == disconnected
#define FC_ERR_H2OK_LOW (1 << 3)
#define FC_ERR_TEMP_L  (1 << 4) //(-10C)
#define FC_ERR_TEMP_H (1 << 5) //(75C)
#define FC_ERR_PRES_H (1 << 6) //(9.81 PSI)
#define FC_ERR_PRES_L (1 << 7) //only checked in purge, charge run (0.8 PSI)
#define FC_ERR_OVER_CUR (1 << 8) //(78A)
#define FC_ERR_UND_CUR (1 << 9) //(-3A)
#define FC_ERR_OVER_VOLT (1 << 10) // (50.6V)

//define thresholds for over current etc
#define LOW_TEMP_THRES 950 //-10C. 950 is closer to -5C I thought we'd be safe
#define HIGH_TEMP_THRES 1680 // 75C
#define FC_HIGH_PRES_THRES 9810 //9.81 PSI
#define FC_LOW_PRES_THRES 800 //0.8 PSI
#define OVER_CUR_THRES 78000 //78A
#define UNDER_CUR_THRES -3000 //-3A 
#define OVER_VOLT_THRES 50600 //50.6V

#endif /* FC_ERROR_CODES_H_ */