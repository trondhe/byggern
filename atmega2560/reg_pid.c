/*
 * PI.c
 *
 * Created: 15.11.2016 19.31.29
 *  Author: Kristian
 */ 



 #include <avr/io.h>
 #include "reg_pid.h"
 
 PI_control* pi_control_init(){
	 PI_control* p = malloc (sizeof(PI_control));
	 p->kp = 1;
	 p->ti = 10;
	 p->shift = 1;
	 p->max = 255;
	 p->min = 0;
	 p->i = 0L;
	 
	 return p;
 }
 

long pi_control(struct PI_control *p, int e){
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
	
	return (int8_t)u;
 }

