// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

// Dependencies
#include <avr/io.h>
#include <avr/interrupt.h>

// Drivers
#include "can.h"
#include "uart.h"
#include "menu.h"
#include "adc.h"
#include "oled.h"
#include "screen.h"
#include "sram.h"
//#include "buzzer.h"
//#include "music.h"
//#include "drawings.h"


int main(){
	
	// Disable global interrupt while init
	cli();
	
	// Init systems
	SRAM_init();
	UART_Init(MYUBRR);
	interruptInit();		// Placed in adc file
	init_OLED();
	CAN_init();
	
	// Declaration
	volatile CAN_message_t* CAN_message_recieve = CAN_message_pass2main();
	CAN_message_t CAN_message_send;
	CAN_message_send.id = 1;
	CAN_message_send.length = 8;
	
	// Music init
	DDRD |= (1 << PIND5);		//Set D5(OC1A) as output
	BUZZER_init();
	BUZZER_set_tempo(100);
	BUZZER_start(1);
	BUZZER_play_song();
	
	// Menu and screen 
	node_t* node_current = node_menuinit();
	char** buffer = screenbuffer_init();
	int* menuctrl_state = menuctrl_state_pass2main();
	
	// Joystick 
	joy_position position;
	
	
	// Stuff?! 
	int gamestate = 0;
	int shoot = 0;
	int counter = 0;
	
	// Enable global interrupt
	sei();
	
	while(1){
		
		//ADC_debug();
		
		if(PINB & (1<<PB0)){
			gamestate = 1;
		}
		if(PINB & (1<<PB1)){
			gamestate = 0;
		}
		
		if(gamestate == 0){
			menu_nav(&node_current);
			buffer_writemenu(buffer, &node_current, menuctrl_state);
		} else {
			shoot = (PINB & (1<<PB0));
			buffer_writegame(buffer);
			position = readJoystick();
			CAN_message_send.data[0] = position.x;
			CAN_message_send.data[1] = position.y;
			CAN_message_send.data[2] = shoot;
			CAN_byte_send(&CAN_message_send);
		}

		OLED_print_buffer(buffer);
	}
	return 0;	
}
