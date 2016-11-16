// Created by: Lars-Erik Notevarp Bj�rge, Trond H�bertz Emaus, Kristian Husev�g Krohn

#pragma once

#include <stdint.h>

typedef struct pi_controller_t pi_controller_t;
struct pi_controller_t {
	float reference;
	
	float Kp;
	float Ki;
	
	float prev_error;
	float integral;
};

pi_controller_t* pi_init(float Kp, float Ki);
void pi_update(pi_controller_t* p, float y);
float pi_calc(pi_controller_t* p, float y);
