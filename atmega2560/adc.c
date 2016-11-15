// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

void adc_init(){
	
	DDRF = ~(1 << PF0);
	
	// Enable ADC conversion complete interrupt
	ADCSRA	|= (1 << ADIE);
	
	// Select ADC channel ADC0
	ADCSRB	&= ~(1 << MUX5);
	ADMUX	&= (0b11100000);
	
	// Set internal ref
	ADMUX	&= ~(1 << REFS1);
	ADMUX	|= (1 << REFS0);
	
	// Set ADC in free running mode
	ADCSRB	&= ~((1 << ADTS0)|(1 << ADTS1)|(1 << ADTS2));
	
	// Set prescaler
	ADCSRA	|= (1 << ADPS0)|(1 << ADPS1)|(1 << ADPS2);
	
	// Enable and start first conversion
	ADCSRA	|= (1 << ADEN)|(1 << ADSC);

}

// Read from ADC register
uint16_t adc_read(){
	return ADC; 
}

// Interrupt vector for ADC
ISR(ADC_vect){
	ADCSRA &= ~(1 << ADIF);
	ADCSRA	|= (1 << ADSC);
}