// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include <util/delay.h>
#include "solenoid.h"

void solenoid_init(){
	DDRL	|= (1 << DDL0);		// Set solenoid as output
	PORTL	|= (1 << PL0);		
}

void solenoid_trigger(int trigger){
	if (trigger == 0 || adc_read(6) < 860) {
		PORTL &=~(1 << PL0);
	} else {
		PORTL |= (1 << PL0);
	}
}

void solenoid_toggle(int trigger){
	if (trigger == 0 || adc_read(6) < 860)
	{
		PORTL &=~(1 << PL0);
		_delay_ms(50);
		PORTL |= (1 << PL0);
		_delay_ms(50);
	}
}