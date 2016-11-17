/*
 * PI.h
 *
 * Created: 15.11.2016 19.34.04
 *  Author: Kristian
 */ 


#ifndef PI_H_
#define PI_H_

typedef struct PID_control{

	float kp;
	float td;
	float ti;
	float h;
	int max;
	int min;
	float yk_1;
	float uPIk_1;
	float ek_1;

} PID_control;


int8_t pid_control(PID_control *p, int r, int y);
PID_control* pid_control_init();

#endif /* PI_H_ */