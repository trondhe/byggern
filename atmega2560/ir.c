// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include "adc.h"
#include "ir.h"

static uint16_t filter_limit = 15;


//***************************************************************
//	Set new filter limit										*
//***************************************************************
void ir_set_limit(uint16_t val){
	filter_limit = val;
}


//***************************************************************
//	Check if IR is obstructed									*
//***************************************************************
uint8_t ir_obstructed(void){
	#define FILT_ORD 4
	
	// Check if value is below threshold
	#if FILTER_ORDER == 1
	return adc_read(0) < filter_limit;
	#else
	static uint16_t filter[FILT_ORD-1];

	uint16_t ir_val = adc_read(0);
	uint16_t avg = ir_val;

	// Use average value with higher order filter
	for(uint8_t i = 0; i < FILT_ORD-2; i++){
		avg += filter[i];
		filter[i] = filter[i+1];
	}
	avg += filter[FILT_ORD-2];
	filter[FILT_ORD-2] = ir_val;
	
	// Returns 1 if obstructed
	return avg/FILT_ORD < filter_limit;
	#endif
}
