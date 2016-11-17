/*
 * PI.c
 *
 * Created: 15.11.2016 19.31.29
 *  Author: Kristian
 */ 



 #include <avr/io.h>
 #include "reg_pid.h"
 
 PID_control* pid_control_init(){
	 PID_control* p = malloc (sizeof(PID_control));
	 p->kp = 1;
	 p->ti = 10;
	 p->h = 0.001;
	 p->max = 255;
	 p->min = 0;
	 p->td = 0;
	 p->yk_1 = 0;
	 p->ek_1 = 0;
	 p->uPIk_1 = 0;
	 
	 return p;
 }
 

int8_t pid_control(PID_control *p, int r, int y){
	
	float uPIk;
	float uDk;
	float yk;
	float u;
	float ek;
	
	
	yk = (float)y;
	ek =(float)(r - y);

	
	uPIk = p->uPIk_1 + (p->kp *(1 + (p->h/p->ti)) * ek) - p->ek_1; // PI med windup

	if (uPIk > p->max){
		uPIk = p->max;
	}

	else if (uPIk < p->min){
		uPIk = p->min;
	}

	uDk = p->kp * (p->td/p->h) * ((float)y - p->yk_1); // Derivator
	
	u = uPIk - uDk;
	
	if (u > p->max){
		u = p->max;
	}

	else if (u < p->min){
		u = p->min;
	}
	
	p->ek_1 = ek;
	p->uPIk_1 = uPIk;
	p->yk_1 = yk;
	
	
	return (int8_t)u;
 }

