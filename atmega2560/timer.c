// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t volatile static t=0;

void start_timer(void){
	TCCR5B |= (1 << CS50) | (1 << CS52);			// Prescaler set to 1024
}

void stop_timer(void){
	TCCR5B &= ~(1 << CS50);
	TCCR5B &= ~(1 << CS52);
}

uint16_t acquire_time(){
	return TCNT5L;
}

void reset_timer(void){
	t=0;
}

