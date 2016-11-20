// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

// system_logic.c: Contains the logic for the game, menu and screen systems.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system_logic.h"
#include "setup.h"
#include "menu.h"
#include "screen.h"
#include "adc.h"
#include "can.h"


//***************************************************************
//	System logic variables init									*
//***************************************************************
static joy_position joy_pos;
node_t* node_current = NULL;
int CAN_data[8];
CAN_message_t* CAN_message_recieve_ptr;
sys_val_t sys_vals = {.calibration_info = 0, .score_counter = 0, .score_top = 0};
int score_overflow_counter = 0;


//***************************************************************
//	System logic external values retrieval						*
//***************************************************************
void system_logic_vars_init(){
	node_current = menu_nodelist_init();
	screen_buffer_init();
	screen_vals_init();
	menu_vals_init();
	CAN_message_recieve_ptr = CAN_message_recieve_get();
}


//***************************************************************
//	System logic values get										*
//***************************************************************
sys_val_t* sys_vals_get(){
	return &sys_vals;
}


//***************************************************************
//	Main system logic loop function for game					*
//***************************************************************
void system_loop() {

	// Game escape
	if ((sys_vals.gamemode == 2) & (t_bit_l(PINB, PB1))) { // Checks switch sw1 on malfunctionboard
		sys_vals.gamemode = 0;
		screen_buffer_writeend(0);
		sys_vals.score_counter = 0;
	}
	// Game failstate
	if ((sys_vals.gamemode == 2) & (sys_vals.IR_status == 1)) {
		screen_buffer_writeend (1);
		sys_vals.gamemode = 0;
		sys_vals.score_counter = 0;
	}

	// Joystick position update
	joy_pos = readJoystick();


	switch (sys_vals.gamemode) {
		case 0: // Gamemode: Menu
			menu_nav(&node_current, &joy_pos);
			screen_buffer_writemenu(&node_current);
			screen_oled_print_buffer();
		break;
		
		case 1: // Gamemode: Calibration, not needed with current setup, disabled
			settings_guncalibration();
		break;

		case 2: // Gamemode: Game
			screen_buffer_writegame();
			screen_oled_print_buffer();
		break;
	}
	
	// Update of CAN values for node 2
	CAN_data[0] = joy_pos.x;
	CAN_data[1] = joy_pos.y;
	CAN_data[2] = (~(PINB) & (1 << PB0));
	CAN_data[3] = sys_vals.gamemode;
	CAN_data[4] = sys_vals.settings;
	CAN_message_transmitt(CAN_data);
}

// Interrupt based score timer
ISR(TIMER1_OVF_vect) {
	if(sys_vals.gamemode == 2) {
		score_overflow_counter++;
		if (score_overflow_counter > 30) {
			sys_vals.score_counter++;
			score_overflow_counter = 0;
		}
	}
}