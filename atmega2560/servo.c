// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include <avr/interrupt.h>
#include "servo.h"


//***************************************************************
//	Initilization of servo motor								*
//***************************************************************
void servo_init(void){
	
	// Set data direction to output
	DDRB |= (1<<PB6);
	
	// Set top value for Fast-PWM counter
	ICR1 = 40000;
	
	// Set trigger value for Fast-PWM counter
	OCR1B = 3000;
	
	// Set timer for Fast-PWM
	TCCR1A = (1<<COM1B1)|(1<<WGM11);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS11);
}


//***************************************************************
//	Set servo angle 											*
//***************************************************************
void servo_set_angle(int pos) {
	
	// Set offset such that 0 is middle position
	int16_t servo_angle = (((-pos+100)*12)+1800);
	
	// To ensure dead-zone around mid point (To make servo quiet when idle!)
	if (servo_angle <= 1800) {
		OCR1B = 1800;
		} else if (servo_angle >= 4200) {
		OCR1B = 4200;
		} else if ((servo_angle >= 2850) && (servo_angle <= 3400)) {
		OCR1B = 3000;
		} else {
		OCR1B = servo_angle;
	}
}