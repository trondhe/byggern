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
	
	// Enable global interrupt
	sei();
	int k; // IR: 1 if obstructed, 0 if nothing, -1 neutral
	int16_t counter = 0;
	uint16_t ping, ir, shoot;

    while(1)
    {
		
		// TESTING AREA			//////////////////////////////
		ping = adc_read(0);
		printf("\nPing = %d", ping);
		_delay_ms(10);	
		ir = adc_read(4);
		printf("        IR = %d", ir);
		//_delay_ms(10);	
		shoot = adc_read(6);
		printf("        Shoot = %d", shoot);
		//_delay_ms(10);
		// END TESTING AREA		//////////////////////////////
		
		
		// Control Position motor
		motor_control(CAN_message_recieve->data[0]);
		
		// Control Servo motor
		servo_set_angle(CAN_message_recieve->data[0]);
		
		// Control solenoid
		solenoid_trigger(CAN_message_recieve->data[2]);
		
		// IR Reciever
		k = ir_obstructed(); 
				
		
		// Unfinished stuff
		enc_value = motor_encoder_read();
		
		//UART_print_char("\n");
		//UART_print_int(enc_value);
    }
	
	return 0;
}