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
};


pi_control(struct PIcontrol *p, int e);
void pi_control_init(struct PIcontrol *p);

#endif /* PI_H_ */