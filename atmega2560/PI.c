/*
 * PI.c
 *
 * Created: 15.11.2016 19.31.29
 *  Author: Kristian
 */ 



 #include <avr/io.h>
 #include "pi_control.h"

 int pi_control(struct PIcontrol *p, int e){
	int int_ok;
	long new_i;
	long u;

	new_i = p->i + e;

	u = (p->kp * (long)e + p->ti * new_i) >> p->shift;

	int_ok = 1;

	if (u > p->max){
		
		u = p->max;

		if (e > 0){
			int_ok = 0;
		}
	}

	else if (u < p->min){
		u = p->min;

		if (e < 0){
			int_ok = 0;
		}
	}

	if (int_ok == 1){
		p->i = new_i;
	}
 }

 void pi_control_init(struct PIcontrol *p){

	p->i = 0L;
 }