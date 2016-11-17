// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include <util/delay.h>
#include "solenoid.h"

void solenoid_init(){
	//initialize pin 0 on port e for solenoid usage
	DDRL	|= (1 << DDL0);
	PORTL	|= (1 << PL0);
}

void solenoid_trigger(int trigger){
	if (trigger == 1) {
		PORTL &=~(1 << PL0);
	} else {
		PORTL |= (1 << PL0);
	}
}

void solenoid_toggle(){
	PORTL &=~(1 << PL0);
	_delay_ms(30);
	PORTL |= (1 << PL0);
	_delay_ms(30);
}

void solenoid_burst(){
	PORTL &=~(1 << PL0);
	_delay_ms(50);
	PORTL |= (1 << PL0);
	_delay_ms(50);
	PORTL &=~(1 << PL0);
	_delay_ms(50);
	PORTL |= (1 << PL0);
	_delay_ms(50);
	PORTL &=~(1 << PL0);
	_delay_ms(50);
	PORTL |= (1 << PL0);
	_delay_ms(50);
	
}