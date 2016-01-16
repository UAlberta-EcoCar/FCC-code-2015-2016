/*
 * FuelCell_mode_Select.h
 *
 * Created: 2016-01-15 8:54:24 PM
 *  Author: Reegan
 */ 


#ifndef FUELCELL_MODE_SELECT_H_
#define FUELCELL_MODE_SELECT_H_

#define TEST_BENCH_MODE //All conversions adjusted for 0-10V input range
//#define IN_CAR_MODE //FCC will expect a signal from power board
//#define OUT_OF_CAR_MODE //FCC just powered alone no power board


#ifdef TEST_BENCH_MODE
#warning "You are in Test Bench Mode"
#warning "Do not connect Fuel Cell"
#endif

#ifdef OUT_OF_CAR_MODE
#warning "You are in out of car mode"
#warning "If you are using Power Board you might have issues"
#endif

#endif /* FUELCELL_MODE_SELECT_H_ */