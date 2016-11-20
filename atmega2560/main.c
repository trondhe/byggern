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


//***************************************************************
//	Main program												*
//***************************************************************
int main(void)
{	
	// Disable global interrupt while init
	cli();
	
	// Initializations
	UART_Init(MYUBRR);
	CAN_init();
	CAN_interruptInit();
	servo_init();
	solenoid_init();
	DAC_init();
	adc_init();
	motor_init();
	
	// Typedefs
	typedef struct sys_val_t {
		int mode;
		int settings;
		int calibration_info;
	} sys_val_t;
	
	// Declaration 
	float y;
	uint8_t ir;
	uint16_t ping;
	int8_t servo_angle, solenoid, x_pos, y_pos, gun_value, calibration_stage;
	int16_t ping_pos_left, ping_pos_right, ping_pos_over_motor, r, u;
	CAN_message_t* CAN_message_recieve = CAN_message_pass2main();
	CAN_message_t CAN_message_send;
	PID_control* p = pid_control_init();
	
	// Initialization
	sys_val_t sys_vals = {.mode=0, .settings=0, .calibration_info=0};
	calibration_stage = 0;
	CAN_message_send.id = 30;
	CAN_message_send.length = 8;
	
	// Enable global interrupt
	sei();


	//***************************************************************
	//	Game loop													*
	//***************************************************************
    while(1)
    {
		// Acquire IR value
		ir = ir_obstructed();
		
		// Send IR value and calibration data to node 1
		CAN_message_send.data[0] = 0;
		CAN_message_send.data[1] = sys_vals.calibration_info;
		CAN_byte_send(&CAN_message_send);
						
		// Read CAN messages from node 3 (Arduino UNO)
		if(CAN_message_recieve->id == 1){
			
			//printf("CAN id:%d \n",CAN_message_recieve->id);
			// Get position of hand from ultrasonic sensor
			ping = CAN_message_recieve->data[0] << 8;
			ping |= CAN_message_recieve->data[1];
			
			// Get value when gun is fired (0 if fired, 1 if idle)
			gun_value = CAN_message_recieve->data[2];
		}
		
		// Read CAN data from node 1 (atmega162)
		else if(CAN_message_recieve->id == 50){	
			
			// Receive game mode, settings, joystick position and button from node 1
			sys_vals.mode = CAN_message_recieve->data[3];					
			sys_vals.settings = CAN_message_recieve->data[4];				
			solenoid = CAN_message_recieve->data[2];			
			y_pos = CAN_message_recieve->data[1];
			x_pos = CAN_message_recieve->data[0];
			
		}

		else{
			
			// If unknown node tries to send data
			printf("CAN message with unknown id:%d \n",CAN_message_recieve->id);
		}

		// Modes
		switch(sys_vals.mode){
			
			case 0:		// MENU
				motor_control(0);
				servo_set_angle(0);
			break;
			
			case 1:		// CALIBRATION
				
				// Get position from left side
				if(gun_value == 0 && calibration_stage == 0)
				{
					sys_vals.calibration_info = 3;
					solenoid_trigger(0,gun_value);
					ping_pos_left = ping;
					calibration_stage = 1;
					_delay_ms(1000);
				}
				
				
				else if(calibration_stage == 0)
				{
					sys_vals.calibration_info = 2;
				}
				
				// Get position from right side
				if(gun_value == 0 && calibration_stage == 1)
				{
					sys_vals.calibration_info = 4;
					solenoid_trigger(0,gun_value);
					ping_pos_right = ping;
					calibration_stage = 2;
					_delay_ms(1000);
					
				}
				
				// Get position of motor
				if(gun_value == 0 && calibration_stage == 2)
				{
					sys_vals.calibration_info = 1;
					solenoid_trigger(0,gun_value);
					ping_pos_over_motor = ping;
					calibration_stage = 0;
					_delay_ms(1000);

				}
			break;
			
			case 2:		// GAME MODE
				switch(sys_vals.settings){

					case 0:		// Joystick movements with single-shot
					
						solenoid_trigger(solenoid,gun_value);	
						motor_control(x_pos);	
						servo_set_angle(x_pos);	
						break;
				
					case 1:		// Joystick movements with automatic
					
						solenoid_toggle(solenoid, gun_value);	
						motor_control(x_pos);
						servo_set_angle(x_pos);
						break;
				
					case 2:		// PID movements with single-shot
					
						solenoid_trigger(solenoid,gun_value);
						
						//Adjust reference for offset
						r = ping-650;
						
						// Scale down encoder values to be the same as ping sensor
						y = (motor_encoder_read()/5);
						
						// Calculate control input with PID
						u = pid_control(p, r, y);
						
						// Send control input to motor and servo
						motor_control(u);
						servo_set_angle(u/3);
						break;

					case 3:		// PID movements with automatic
					
						solenoid_toggle(solenoid,gun_value);	// Control solenoid
												
						//Adjust reference for offset	
						r = ping-650;	
						
						// Scale down encoder values to be the same as ping sensor					
						y = (motor_encoder_read()/5);	
						
						// Calculate control input with PID
						u = pid_control(p, r, y);
						
						// Send control input to motor and servo
						motor_control(u);
						servo_set_angle(u/3);
						break;
				}
				break;
		}
    }	
	return 0;
}
