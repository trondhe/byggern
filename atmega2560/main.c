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
	
	typedef struct sys_val_t {
		int mode;		// Menu:0 - Calibration:1 - Game 2
		int settings;	// bit0:Firemode (auto | single) - bit1:Controlmode (Joystick|Gun)
		int calibration_info; // Not calibrated: 0 - Calibrated:1 - left:2 - right:3 - motor:4
	} sys_val_t;
	
	int16_t encoder_maxvalue;
	int16_t enc_value;
	int servo_angle = 2000;
	CAN_message_t* CAN_message_recieve = CAN_message_pass2main();
	CAN_message_t CAN_message_send;
	CAN_message_send.id = 3;
	CAN_message_send.length = 8;
	PID_control* p = pid_control_init();
	sys_val_t sys_vals = {.mode=0, .settings=0, .calibration_info=0};
	int triggerVal = 970; 
	int shootVal;
	
	
	// Enable global interrupt
	sei();
	
	// testing variables
	int k; // IR: 1 if obstructed, 0 if nothing, -1 neutral
	int16_t counter = 0;
	uint16_t ir, shoot;
	int8_t solenoid;
	int8_t x_pos;
	int8_t y_pos;
	int16_t r; // Referanse
	float y;
	int16_t ping_pos_left;
	int16_t ping_pos_right;
	int16_t ping_pos_over_motor;
	int calibration_stage = 0;
	

	int16_t u; // Pådrag
	uint16_t ping;
	int gunVal = 1;

	PORTH &= ~(1 << ENCODER_RESET);
	_delay_ms(10);
	// Disable reset
	PORTH |= (1 << ENCODER_RESET);
	
    while(1)
    {
		shootVal = adc_read(6);
		ir = adc_read(0);

		//CAN_message_send.data[1] = sys_vals.calibration_info;
		//CAN_byte_send(&CAN_message_send);
						
		
		if(CAN_message_recieve->id == 1)
		{
			
			//ping = CAN_message_recieve->data[0];

			//ping = CAN_message_recieve->data[1];
			//printf(byte_to_binary(ping));
			
			
			ping = CAN_message_recieve->data[0] << 8;
			ping |= CAN_message_recieve->data[1];
			printf("%d\n",ping);
			
			gunVal = CAN_message_recieve->data[2];
			printf("CAN: %d\n",CAN_message_recieve->id);
			
			
		}
		
		else if(CAN_message_recieve->id == 50)
		{	
			sys_vals.mode = CAN_message_recieve->data[3];					// Receive mode byte from CAN
			sys_vals.settings = CAN_message_recieve->data[4];				// Receive options byte from CAN
			solenoid = CAN_message_recieve->data[2];			
			y_pos = CAN_message_recieve->data[1];
			x_pos = CAN_message_recieve->data[0];
			printf("CAN: %d Mode: %d Settings: %d x_pos: %d\n",CAN_message_recieve->id,sys_vals.mode,sys_vals.settings,x_pos);
		}

		else
		{
			//printf("CAN message with unknown id:%d \n",CAN_message_recieve->id);
		}
		//sys_vals.settings = 1;
		//printf("%d\n",shootVal);
		//printf("CAN: %d Mode: %d Settings: %d x_pos: %d\n",CAN_message_recieve->id,sys_vals.mode,sys_vals.settings,x_pos);
		sys_vals.mode = 2;
		sys_vals.settings = 2;
		switch(sys_vals.mode){
			printf("penis\n");
			case 0:		// MENU
			motor_control(0);
			servo_set_angle(0);
				//printf("Menu is active\n");
			break;
			
			case 1:		// CALIBRATION
				printf("Calibration mode\t");
			
				if(gunVal == 0 && calibration_stage == 0)
				{
					sys_vals.calibration_info = 3;
					solenoid_trigger(0,gunVal);
					ping_pos_left = ping;
					calibration_stage = 1;
					_delay_ms(1000);
				}
				
				else if(calibration_stage == 0)
				{
					sys_vals.calibration_info = 2;
				}
				
				
				if(gunVal == 0 && calibration_stage == 1)
				{
					sys_vals.calibration_info = 4;
					solenoid_trigger(0,gunVal);
					ping_pos_right = ping;
					calibration_stage = 2;
					_delay_ms(1000);
					
				}
				
				if(gunVal == 0 && calibration_stage == 2)
				{
					sys_vals.calibration_info = 1;
					solenoid_trigger(0,gunVal);
					ping_pos_over_motor = ping;
					calibration_stage = 0;
					_delay_ms(1000);

				}
				

				
				printf("Min: %d\t",ping_pos_left);
				printf("Max: %d\t",ping_pos_right);
				printf("Motor: %d\n",ping_pos_over_motor);
			break;
			
			
			case 2:		// GAME MODE
				switch(sys_vals.settings){
					printf("In game mode\n");
					case 0:		// Joystick movements with single-shot
						//printf("Joystick with single-shot\t");
						solenoid_trigger(solenoid,gunVal);	// Control solenoid
						motor_control(x_pos);	// Control Position motor
						servo_set_angle(x_pos);	// Control Servo motor
						break;
				
					case 1:		// Joystick movements with automatic
						//printf("Joystick with automatic\t");
						solenoid_toggle(solenoid, gunVal);	// Control solenoid
						motor_control(x_pos);	// Control Position motor
						servo_set_angle(x_pos);	// Control Servo motor
						break;
				
					case 2:		// PID movements with single-shot
						//printf("PID with single-shot\t");
						solenoid_trigger(solenoid,gunVal);	// Control solenoid
						r = ping-650;						// Low limit 690, high limit 2450
						//UART_print_int(r);
						//UART_print_char("\n");
						y = (motor_encoder_read()/5);	//Scale down from 32768 to 255
						u = pid_control(p, r, y);
						//UART_print_int(u);
						//UART_print_int(r);
						//UART_print_char("\n");
						motor_control(u);
						servo_set_angle(u/3);
						break;
				
					case 3:		// PID movements with automatic
						//printf("PID with automatic\t");
						solenoid_toggle(solenoid,gunVal);	// Control solenoid
						r = ping-650;						// Low limit 690, high limit 2450
						//UART_print_int(r);
						//UART_print_char("\n");
						y = (motor_encoder_read()/5);	//Scale down from 32768 to 255
						u = pid_control(p, r, y);
						//UART_print_int(u);
						//UART_print_int(y);
						UART_print_char("\n");
						motor_control(u);
						servo_set_angle(u/3);
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