// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t volatile static t=0;

void start_timer(void){
	TCCR5B |=  (1 << CS50) | (1<<CS52);  // Prescaler set to 1024
	TIMSK5 |= (1 << TOIE5);              // Overflow interrupt enable
}

void stop_timer(void){
	TCCR5B &= ~(1 << CS50);
	TCCR5B &= ~(1 << CS52);
	TIMSK5 &= ~(1 << TOIE5);
}

uint16_t acquire_time(){
	return t;
}

void reset_timer(void){
	t=0;
}

ISR(TIMER5_OVF_vect){
	t++;
}