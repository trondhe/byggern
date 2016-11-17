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
	CAN_message_send.length = 8;
	PID_control* p = pid_control_init();
	
	
	// Enable global interrupt
	sei();
	
	// testing variables
	int k; // IR: 1 if obstructed, 0 if nothing, -1 neutral
	int16_t counter = 0;
	uint16_t ir, shoot;
	int8_t mode;
	int8_t options;
	int8_t sol;
	int8_t x_pos;
	int16_t r; // Referanse
	int16_t y;
	int16_t ping_pos_left;
	int16_t ping_pos_right;
	int16_t ping_pos_over_motor;
	int calibration_stage = 0;

	int8_t u; // Pådrag
	long ping;

	
    while(1)
    {
		//debug();
		// TESTING AREA			//////////////////////////////

		// END TESTING AREA		//////////////////////////////
		if(CAN_message_recieve->id == 1)
		{
			printf("Message with id 1 received\n");
			ping |= (CAN_message_recieve->data[0] << 24);
			ping |= (CAN_message_recieve->data[1] << 16);
			ping |= (CAN_message_recieve->data[2] << 8);
			ping |= CAN_message_recieve->data[3];
			printf("%ld\n",ping);
		}
		
		else if(CAN_message_recieve->id == 2)
		{	
			printf("Message with id 2 received\n");
			mode = CAN_message_recieve->data[3];				// Receive mode byte from CAN
			options = CAN_message_recieve->data[4];				// Receive options byte from CAN
			sol = CAN_message_recieve->data[2];
			x_pos = CAN_message_recieve->data[0];
		}

		else
		{
			printf("CAN message with unknown id:%d \n",CAN_message_recieve->id);	
		}


		_delay_ms(50);
		
		switch(mode){
			case 0:		// MENU
				printf("Menu is active\n");
			break;
			
			case 1:		// CALIBRATION
			if(adc_read(6) < 860 && calibration_stage == 0)
			{
				solenoid_trigger(1);
				ping_pos_left = adc_read(0);
				calibration_stage = 1;
				_delay_ms(1000);
			}
			
			if(adc_read(6) < 860 && calibration_stage == 1)
			{
				solenoid_trigger(1);
				ping_pos_right = adc_read(0);
				calibration_stage = 2;
				_delay_ms(1000);
			}
			
			if(adc_read(6) < 860 && calibration_stage == 2)
			{
				solenoid_trigger(1);
				ping_pos_over_motor = adc_read(0);
				calibration_stage = 0;
				_delay_ms(1000);
			}
			
			printf("Min: %d\t",ping_pos_left);
			printf("Max: %d\t",ping_pos_right);
			printf("Motor: %d\n",ping_pos_over_motor);
			
			break;
			
			case 2:		// GAME MODE
			
				switch(options){
// 
					case 0:		// Joystick movements with single-shot
						solenoid_trigger(sol);	// Control solenoid
						motor_control(x_pos);	// Control Position motor
						servo_set_angle(x_pos);	// Control Servo motor
						break;
				
					case 1:		// Joystick movements with automatic
						solenoid_toggle(sol);	// Control solenoid
						motor_control(x_pos);	// Control Position motor
						servo_set_angle(x_pos);	// Control Servo motor
						break;
				
					case 2:		// PID movements with single-shot
						solenoid_trigger(sol);	// Control solenoid
						r = adc_read(0);
						y = (motor_encoder_read()>> 3);	//Scale down from 32768 to 255
						u = pid_control(p, r, y);
						motor_control(u);
						servo_set_angle(u);
						break;
				
					case 3:		// PID movements with automatic
						solenoid_toggle(sol);	// Control solenoid
						r = adc_read(0);
						y = (motor_encoder_read()>> 3);	//Scale down from 32768 to 255
						u = pid_control(p, r, y);
						motor_control(u);
						servo_set_angle(u);
						break;
				}
				break;
		}
		
		
		// IR Reciever
		k = ir_obstructed(); 
				
		
		// Unfinished stuff
		enc_value = motor_encoder_read();

    }
	
	return 0;
}

void debug(){
		printf("\nPing = %d", adc_read(0));
		printf("\nIR = %d", adc_read(4));
		printf("\nShoot = %d", adc_read(6));
}