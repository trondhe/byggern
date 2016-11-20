/*
 * PI.c
 *
 * Created: 15.11.2016 19.31.29
 *  Author: Kristian
 */ 



 #include <avr/io.h>
 #include "reg_pid.h"
 #include <avr/delay.h>
 
 PID_control* pid_control_init(){
	 PID_control* p = malloc (sizeof(PID_control));
	 p->kp = 0.1;
	 p->ti = 1000;
	 p->h = 0.001;
	 p->max = 127;
	 p->min = -128;
	 p->td = 0;
	 p->yk_1 = 0;
	 p->ek_1 = 0;
	 p->uPIk_1 = 0;
	 
	 start_timer();
	 return p;
 }
 

int8_t pid_control(PID_control *p, int16_t r,  float yk){
	
	float uPIk;
	float uDk;
	float u;
	float ek;
	int8_t uk;

	p->h=((float)acquire_time()/15625.0);

	reset_timer();

	
	//printf("referanse = %d\n", r);
	ek =((float)r+yk);
	
	//UART_print_int(yk);
	//printf("error = %7.6f\n", ek);
	
	//uPIk = p->uPIk_1 + (p->kp *(1 + (p->h/p->ti)) * ek) - p->ek_1; // PI med windup
	//
	//if (uPIk > p->max){
		//uPIk = p->max;
	//}
//
	//else if (uPIk < p->min){
		//uPIk = p->min;
	//}
	////printf("PI pådrag = %.6f\t", uPIk);
	//uDk = p->kp * (p->td/p->h) * (yk - p->yk_1); // Derivator
	////printf("h = %.6f\t", p->h);
	//u = uPIk - uDk;
	////printf("pådrag = %.6f\t", uPIk);
	//if (u > p->max){
		//u = p->max;
	//}
//
	//else if (u < p->min){
		//u = p->min;
	//}
	//
	//p->ek_1 = ek;
	//p->uPIk_1 = uPIk;
	//p->yk_1 = yk;
	//
	//
	////printf("steglengde = %7.6f\n", u);
	u = ek;
	if (u > p->max){
		u = p->max;
		}
		
		else if (u < p->min){
		u = p->min;
		}
	uk = u;
	//uk = (int8_t)uPIk;
	//UART_print_char("uk: ");
	//UART_print_int(uk);
	//UART_print_char("\t");
	//printf("ek = %7.6f\n", ek);
	return uk;
 }

