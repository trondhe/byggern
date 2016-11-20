// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <stdlib.h>
#include <util/delay.h>
#include <avr/io.h>
#include "motor.h"
#include "TWI_Master.h"
#include "uart.h"


//***************************************************************
//	Initilization of motor and encoder							*
//***************************************************************
void motor_init(){
	// Set data direction as output on motor pins
	DDRH |= (1 << MOTOR_DIRECTION)|			
			(1 << MOTOR_ENABLE);			
	
	// Set data direction as output on encoder pins 
	DDRH |= (1 << ENCODER_ACTIVE_LOW)|		
			(1 << ENCODER_RESET)|			
			(1 << ENCODER_SELECT);			
	
	// Set PORTK as inputs
	DDRK = 0x00;

	// Enable motor
	PORTH |= (1 << MOTOR_ENABLE);
	
	// Reset encoder and disable reset
	PORTH &= ~(1 << ENCODER_RESET);
	_delay_ms(10);
	PORTH |= (1 << ENCODER_RESET);
}


//***************************************************************
//	Read position from encoder									*
//***************************************************************
int16_t motor_encoder_read(){
	
	// Initialize encoder with 0
	uint16_t encodervalue = 0x0000;
		
	// Enable encoder output
	PORTH &= ~(1 << ENCODER_ACTIVE_LOW);

	// Set SEL low, MSB on MJ2
	PORTH &= ~(1 << ENCODER_SELECT);
	_delay_us(20);
	
	// Read MSB
	uint8_t MSB = PINK;

	// Set SEL high, LSB on MJ2
	PORTH |= (1 << ENCODER_SELECT);
	_delay_us(20); 
	
	// Read LSB
	uint8_t LSB = PINK;
	
	// Disable encoder output
	PORTH |= (1 << ENCODER_ACTIVE_LOW);

	// Encodervalue consist of an int16 with MSB and LSB
	encodervalue = ((MSB << 8) | LSB);

	return encodervalue;
}


//***************************************************************
//	Send motor speed command and direction						*
//***************************************************************
void motor_control(int8_t speed){
	
	uint8_t motorInput = abs(speed);

	// Set motor direction
	if (speed > 0) {			
		PORTH |= (1 << PH1);	// Motor goes RIGHT
	} else {					
		PORTH &= ~(1 << PH1);	// Motor goes LEFT
	}
	
	// Send TWI package
	DAC_write(motorInput);	
}


//***************************************************************
//	Encoder debugging											*
//***************************************************************
void motor_encoder_debug(uint8_t MSB, uint8_t LSB){
	
	// Write encoder values as binary
	UART_print_char("\n");
	UART_print_char(byte_to_binary(MSB));
	UART_print_char("\t");
	UART_print_char(byte_to_binary(LSB));
}