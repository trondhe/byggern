// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

 #include <avr/io.h>
 #include <avr/delay.h>
 #include "reg_pid.h"
 
 
//***************************************************************
//	Initilization of PID controller								*
//***************************************************************
 PID_control* pid_control_init(){
	 
	 // Allocate memory for PID variables
	 PID_control* p = malloc (sizeof(PID_control));
	 
	 // Initialize gains and saturation limits
	 p->kp = 0.1;
	 p->ti = 1000;
	 p->h = 0.001;
	 p->max = 127;
	 p->min = -128;
	 p->td = 0;
	 p->yk_1 = 0;
	 p->ek_1 = 0;
	 p->uPIk_1 = 0;
	 
	 // Start timer to retrieve the step length
	 start_timer();
	 
	 return p;
 }
 
 
//***************************************************************
//	PID controller calculation							*
//***************************************************************
int8_t pid_control(PID_control *p, int16_t r,  float yk){
	
	// Declarations
	float uPIk;
	float uDk;
	float u;
	float ek;
	int8_t uk;

	// Get step length
	p->h=((float)acquire_time()/15625.0);
	
	// Reset timer to make next step length accurate
	reset_timer();
	
	// Error dynamics
	ek =((float)r+yk);
	
	// Control input for PI 
	uPIk = p->uPIk_1 + (p->kp *(1 + (p->h/p->ti)) * ek) - p->ek_1;
	
	
	// Anti wind-up saturation to prevent accumulation of integral term
	if (uPIk > p->max){
		uPIk = p->max;
	}
	else if (uPIk < p->min){
		uPIk = p->min;
	}

	// Control input for D
	uDk = p->kp * (p->td/p->h) * (yk - p->yk_1);

	// Control input for PID
	u = uPIk - uDk;

	// Saturation for control input between min and max
	if (u > p->max){
		u = p->max;
	}
	else if (u < p->min){
		u = p->min;
	}
	
	// Update error, control input and measurement
	p->ek_1 = ek;
	p->uPIk_1 = uPIk;
	p->yk_1 = yk;
	uk = u;

	return uk;
 }

