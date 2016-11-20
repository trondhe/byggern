// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <stdlib.h>
#include <util/delay.h>
#include <avr/io.h>
#include "motor.h"
#include "TWI_Master.h"
#include "uart.h"



int16_t prev_error = 0;

void motor_init(){
	// Sets DDR on motor pins
	DDRH |= (1 << MOTOR_DIRECTION)|			// DIR  Motor direction
			(1 << MOTOR_ENABLE);			// EN   Motor enable
	
	// Sets DDR on encoder pins  
	DDRH |= (1 << ENCODER_ACTIVE_LOW)|		// !OE  Encoder active low
			(1 << ENCODER_RESET)|			// !RST Encoder reset active low
			(1 << ENCODER_SELECT);			// SEL  Encoder select
	
	//Set PORTK to input
	DDRK = 0x00;

	// Enables motor
	PORTH |= (1 << MOTOR_ENABLE);
	
	// Disable reset
	PORTH |= (1 << ENCODER_RESET);
}

int16_t motor_encoder_read(){
	uint16_t encodervalue = 0x0000;
		
	// Enable encoder output
	PORTH &= ~(1 << ENCODER_ACTIVE_LOW);

	// Set SEL low, MSB on MJ2
	PORTH &= ~(1 << ENCODER_SELECT);
	_delay_us(20); // 20µs pause
	
	// Read MSB
	uint8_t MSB = PINK;

	// Set SEL high, LSB on MJ2
	PORTH |= (1 << ENCODER_SELECT);
	_delay_us(20); // 20µs pause
	
	// Read LSB
	uint8_t LSB = PINK;
	
	// Disable encoder output
	PORTH |= (1 << ENCODER_ACTIVE_LOW);

	// Encodervalue = MSB LSB
	encodervalue = ((MSB << 8) | LSB);
	
	//Binary encoder value print for debugging
	//motor_encoder_debug(MSB, LSB);
	
	return encodervalue;
}

void motor_encoder_calibrate() {
	// ITS MAGIC
	UART_print_char("\n1");
	_delay_ms(3000);
	UART_print_char("\n2");
	motor_control(-100);
	_delay_ms(3000);
	UART_print_char("\n3");
	motor_control(10);
	_delay_ms(600);
	UART_print_char("\n4");
	PORTH &= ~(1 << ENCODER_RESET);
	_delay_ms(10);
	PORTH |= (1 << ENCODER_RESET);
}

void motor_control(int8_t speed){
	
	uint8_t motorInput = abs(speed);
	//printf("Motor input : %d", speed);
	// Set motor direction
	if (speed > 0) {			
		PORTH |= (1 << PH1);	// Motor goes RIGHT
	} else {					
		PORTH &= ~(1 << PH1);	// Motor goes LEFT
	}
	
	// Send TWI package
	DAC_write(motorInput);	
}

void motor_encoder_debug(uint8_t MSB, uint8_t LSB){
	UART_print_char("\n");
	UART_print_char(byte_to_binary(MSB));
	UART_print_char("\t");
	UART_print_char(byte_to_binary(LSB));
}