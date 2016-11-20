// Created by: Lars-Erik Notevarp Bj�rge, Trond H�bertz Emaus, Kristian Husev�g Krohn

#include <avr/io.h>
#include <util/delay.h>
#include "solenoid.h"

void solenoid_init(){
	DDRL	|= (1 << DDL0);		// Set solenoid as output
	PORTL	|= (1 << PL0);		
}

void solenoid_trigger(int trigger, int gunVal){
	if (trigger == 1 || gunVal == 0) {
		PORTL &=~(1 << PL0);
	} else {
		PORTL |= (1 << PL0);
	}
}

void solenoid_toggle(int trigger, int gunVal){
	if (trigger == 1 || gunVal == 0)
	{
		PORTL &=~(1 << PL0);
		_delay_ms(50);
		PORTL |= (1 << PL0);
		_delay_ms(50);
	}
}