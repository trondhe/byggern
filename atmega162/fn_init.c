// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include <avr/interrupt.h>
#include "fn_init.h"

void interrupt_init(void){
	DDRD &= ~(1<<PD3);						// ADC interrupt input
	DDRD &= ~(1<<PD2);						// CAN interrupt input
	GICR |= (1<<INT0) | (1<<INT1);			// Enable INT1 and INT0
	
	EMCUCR |= 1<<ISC11 | 1<<ISC01;			// Both trigger inputs at falling edge
	EMCUCR &= ~(1<<ISC10);
	EMCUCR &= ~(1<<ISC00);
}

void mfcard_io_init() {
	DDRB  &= ~((1<<DDB0)|(1<<DDB1));	// Enable digital inputs from malfunctionboard, active low
	PORTB |= (1<<PB0)|(1<<PB1);		// Enable pull up resistor, active low on digital input
}
