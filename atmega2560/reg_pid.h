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
	int16_t max;
	int16_t min;
	float yk_1;
	float uPIk_1;
	float ek_1;

} PID_control;


int8_t pid_control(PID_control *p, int16_t r, float y);
PID_control* pid_control_init();

#endif /* PI_H_ */