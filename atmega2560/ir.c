// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include "adc.h"
#include "ir.h"

static uint16_t obstr_thresh = 15;

// Threshold for IR filter
void ir_obstructThreshold(uint16_t t){
	obstr_thresh = t;
}

// IR sensor obstructed by ball
uint8_t ir_obstructed(void){
	#define FILTER_ORDER 4
	#if FILTER_ORDER == 1
	return adc_read(4) < obstr_thresh;
	#else
	static uint16_t filter[FILTER_ORDER-1];

	uint16_t val = adc_read(4);
	uint16_t avg = val;

	for(uint8_t i = 0; i < FILTER_ORDER-2; i++){
		avg += filter[i];
		filter[i] = filter[i+1];
	}
	avg += filter[FILTER_ORDER-2];
	filter[FILTER_ORDER-2] = val;

	return avg/FILTER_ORDER < obstr_thresh;
	#endif
}
