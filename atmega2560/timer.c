// Created by: Lars-Erik Notevarp Bj�rge, Trond H�bertz Emaus, Kristian Husev�g Krohn

#include <avr/io.h>
#include <avr/interrupt.h>


//***************************************************************
//	Start timer													*
//***************************************************************
void start_timer(void){
	
	// Set prescaler to 1024
	TCCR5B |= (1 << CS52) | (1 << CS50);	
}


//***************************************************************
//	Stop timer													*
//***************************************************************
void stop_timer(void){
	
	TCCR5B &= ~(1 << CS50);
	TCCR5B &= ~(1 << CS52);
}


//***************************************************************
//	Get current duration from timer								*
//***************************************************************
uint16_t acquire_time(){
	
	return TCNT5;
}


//***************************************************************
//	Reset timer													*
//***************************************************************
void reset_timer(void){
	TCNT5 = 0;
}

