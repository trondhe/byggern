// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include <avr/interrupt.h>
#include "servo.h"


void servo_init(void){
	DDRB |= (1<<PB6);
	ICR1 = 40000;
	OCR1B = 3000;

	TCCR1A = (1<<COM1B1)|(1<<WGM11);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS11);
}

void servo_set_angle(int joystick_pos) {
	int16_t servo_angle = (((-joystick_pos+100)*12)+1800);
	//printf("%d\n",servo_angle);
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