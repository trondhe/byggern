// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "adc.h"


//***************************************************************
//	Initialize adc												*
//***************************************************************
void adc_init(){
	
	// Set adc port as input
	DDRF &= ~(1 << PF0);
	
	// Enable ADC conversion complete interrupt
	ADCSRA	|= (1 << ADIE);
	
	// Set internal ref
	ADMUX	&= ~(1 << REFS1);
	ADMUX	|= (1 << REFS0);
	
	// Set prescaler
	ADCSRA	|= (1 << ADPS0)|(1 << ADPS1)|(1 << ADPS2);
	
	// Enable and start first conversion
	ADCSRA	|= (1 << ADEN);
}


//***************************************************************
//	Read from adc channel										*
//***************************************************************
uint16_t adc_read(uint8_t channel){
	
	// Add muxing between adc channels
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	
	// Single conversion mode
	ADCSRA |= (1<<ADSC);
	
	// Wait for ADC conversion to be completed
	while(ADCSRA&(1<<ADSC));
	return ADC;
}


//***************************************************************
//	Interrupt vector for ADC									*
//***************************************************************
ISR(ADC_vect){
	
	//ADCSRA &= ~(1 << ADIF);
	//ADCSRA	|= (1 << ADSC);
}