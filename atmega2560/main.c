// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

// Dependencies
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

// Drivers
#include "can.h"
#include "uart.h"
#include "spi.h"
#include "mcp.h"
#include "dac.h"
#include "motor.h"
#include "adc.h"
#include "ir.h"
#include "solenoid.h"
#include "reg_pid.h"



// Custom UART print functions
//UART_print_char("text");
//UART_print_int(int);

const char *byte_to_binary(int x)
{
	static char b[9];
	b[0] = '\0';

	int z;
	for (z = 128; z > 0; z >>= 1)
	{
		strcat(b, ((x & z) == z) ? "1" : "0");
	}

	return b;
}

int main(void)
{	
	// Disable global interrupt while init
	cli();
	
	// Init systems
	UART_Init(MYUBRR);
	CAN_init();
	CAN_interruptInit();
	servo_init();
	solenoid_init();
	DAC_init();
	adc_init();
	motor_init();
	//motor_encoder_calibrate();
	
	// Init values
	int16_t encoder_maxvalue;
	int16_t enc_value;
	int servo_angle = 2000;
	CAN_message_t* CAN_message_recieve = CAN_message_pass2main();
	CAN_message_t CAN_message_send;
	CAN_message_send.id = 2;
	CAN_message_send.length = 3;
	PID_control* p = pid_control_init();
	
	
	// Enable global interrupt
	sei();
	// testing variables
	int k; // IR: 1 if obstructed, 0 if nothing, -1 neutral
	int16_t counter = 0;
	uint16_t ping, ir, shoot;
	int8_t mode;
	int16_t r; // Referanse
	int16_t y;
	//uint16_t e; // Reguleringsavvik;
	int8_t u; // Pådrag
	CAN_message_recieve->data[0] = -1;
	
    while(1)
    {
		//printf("%d\n",CAN_message_recieve->data[0]);
		//UART_print_char("penis");
		//printf("Penis\n");
		// TESTING AREA			//////////////////////////////
		ping = adc_read(0);
		//printf("\nPing = %d", ping);
		ir = adc_read(4);
		//printf("        IR = %d", ir);
		shoot = adc_read(6);
		//printf("        Shoot = %d", shoot);
		// END TESTING AREA		//////////////////////////////
		
		
		
		mode = CAN_message_recieve->data[3];		// Receive mode from CAN
		mode = 2;
		//printf("HER:::: %d\n",CAN_message_recieve->data[1]);
		
		
		switch(mode){
			case 0:		// Menu active
				
				printf("Menu is active\n");		
				break;
			
			case 1:		// Joystick
				
				motor_control(CAN_message_recieve->data[0]);	// Control Position motor
				servo_set_angle(CAN_message_recieve->data[0]);	// Control Servo motor
				solenoid_trigger(CAN_message_recieve->data[2]);	// Control solenoid		
				break;
			
			case 2:		// Guns N' Roses mode with automatic
				r = adc_read(0);
				y = (motor_encoder_read()>> 3);	//Scale down from 32768 to 255

				u = pid_control(p, r, y);
				//printf("ref = %d  Målt: %d\t", r,y);
				motor_control (u);
				//printf("Pådrag = %d\n", u);
				if(adc_read(6) < 860){
					//printf("SKYT!");
					solenoid_toggle();			// Machine gun
				}
			
				//enc_value = motor_encoder_read();
			
			
			break;
			
			case 3:		// Guns N' Roses mode with semi-automatic
			
			if(adc_read(6) < 860)
			{
				//printf("SKYT!");
				solenoid_trigger(1);		// Single shot
			}
			else
			{
				solenoid_trigger(0);
			}
			
			break;
			
			case 4: // Guns N' Roses mode with burst
			
			break;
			
			
		}
		
		
		
		// IR Reciever
		k = ir_obstructed(); 
				
		
		// Unfinished stuff
		enc_value = motor_encoder_read();
		
		//UART_print_char("\n");
		//UART_print_int(enc_value);
    }
	
	return 0;
}