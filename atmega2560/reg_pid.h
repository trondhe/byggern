/*
 * PI.h
 *
 * Created: 15.11.2016 19.34.04
 *  Author: Kristian
 */ 


#ifndef PI_H_
#define PI_H_

struct PI_control{

	int kp;
	int ti;
	unsigned char shift;
	int max;
	int min;
	long i;
} PI_control;


void pi_control(PI_control *p, int e);
PI_control* pi_control_init();

#endif /* PI_H_ */