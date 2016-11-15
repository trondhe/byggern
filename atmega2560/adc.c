// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "adc.h"

void adc_init(){
	
	DDRF = ~(1 << PF0);
	
	// Enable ADC conversion complete interrupt
	ADCSRA	|= (1 << ADIE);
	
	// Select ADC channel ADC0
	//ADCSRB	&= ~(1 << MUX5);
	//ADMUX	&= (0b11100000);
	
	// Set internal ref
	ADMUX	&= ~(1 << REFS1);
	ADMUX	|= (1 << REFS0);
	
	// Set ADC in free running mode
	//ADCSRB	&= ~((1 << ADTS0)|(1 << ADTS1)|(1 << ADTS2));
	
	// Set prescaler
	ADCSRA	|= (1 << ADPS0)|(1 << ADPS1)|(1 << ADPS2);
	
	// Enable and start first conversion
	ADCSRA	|= (1 << ADEN);//|(1 << ADSC);

	

}

// Read from ADC register
uint16_t adc_read(uint8_t channel){
	// clear channel
	//ADCSRA	|= (1 << ADEN);
	//ADMUX &= 0b11111000;
	//ADMUX = (ADMUX & 0xE0) | (channel & 0x1F);   //select channel (MUX0-4 bits)
	//ADCSRB = (ADCSRB & 0xDF) | (channel & 0x20);   //select channel (MUX5 bit)
	//ADCSRA |= (1<<ADSC); // Start conversion
	//while(ADCSRA & (1<<ADSC));
	//_delay_us(100);
	//return ADC;
	//ADCSRA &= ~(1<<ADEN); 

	    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	    //single conversion mode
	    ADCSRA |= (1<<ADSC);
	    // wait until ADC conversion is complete
	    while( ADCSRA & (1<<ADSC) );
	    return ADC;
}

// Interrupt vector for ADC
ISR(ADC_vect){
	//ADCSRA &= ~(1 << ADIF);
	//ADCSRA	|= (1 << ADSC);
	

}