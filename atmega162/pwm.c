// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

// PWM.c: Driver for PWM used to play music notes

#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"


//***************************************************************
//	Initialization of PWM for speaker							*
//***************************************************************
void PWM_init(void) {
	//Toggle OC1A on compare match, use fast PWM, OCR1A used as TOP
	TCCR1A |= (1 << COM1A0) | (1 << WGM11) | (1 << WGM10);
	TCCR1A &= ~(1 << COM1A1);
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
}


//***************************************************************
//	PWM music functions											*
//***************************************************************
void PWM_start(uint8_t start) {
	if (start == 1) {
		//Set the prescaler (clk/8)
		TCCR1B |= (1 << CS11);
		TCCR1B &= ~(1 << CS12) & ~(1 << CS10);
		} else {
		//Stop the timer
		TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10);
	}
}

void PWM_play_note(unsigned int count) {
	if (count > 0) {
		//Break between notes
		PWM_start(0);
		_delay_ms(20);
		//Define PWM period (note)
		OCR1A = count;
		PWM_start(1);
		} else {
		PWM_start(0);
	}
}