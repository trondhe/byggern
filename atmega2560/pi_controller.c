// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <stdint.h>
#include <stdlib.h>
#include <string.h> 
#include "pi_controller.h"
#include "timer.h"

//dt = (float)TIMER5_OCRB / (float)F_CPU)*1024;
//Should be calculated every time pi() is called

pi_controller_t* pi_init(float Kp, float Ki){
	pi_controller_t* p = malloc(sizeof(pi_controller_t));
	memset(p, 0, sizeof(pi_controller_t));
	p->Kp = Kp;
	p->Ki = Ki;
	start_timer();
	return p;
}

void pi_update(pi_controller_t* p, float ref){		// ref is the position from last encoder read
	p->reference = ref;	
}

float pi_calc(pi_controller_t* p, float y){
	float dt=(float)(acquire_time()*1024/16000000);
	reset_timer();
	float error = p->reference - y;
	
	p->integral += error *dt;
	p->prev_error = error;

	return	p->Kp * error + p->Ki * p->integral;
}