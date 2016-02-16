/*
 * FuelCell_mode_Select.h
 *
 * Created: 2016-01-15 8:54:24 PM
 *  Author: Reegan
 */ 


#ifndef FUELCELL_MODE_SELECT_H_
#define FUELCELL_MODE_SELECT_H_

#define FAKE_INPUT_MODE //FCC just powered alone no power board
//#define IN_CAR_MODE //FCC will expect a signal from power board
//#define FAKE_INPUT_MODE //All conversions adjusted for 0-10V input range


#ifdef TEST_BENCH_MODE
#warning "You are in Test Bench Mode"
#warning "If you use the power board you might have issues"
#endif

#ifdef FAKE_INPUT_MODE
#warning "You are in fake input mode"
#warning "Do not connect fuel cell"
#endif

#endif /* FUELCELL_MODE_SELECT_H_ */