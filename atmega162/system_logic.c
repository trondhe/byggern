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

static joy_position joy_pos;
node_t* node_current = NULL;
int CAN_data[8];
sys_val_t sys_vals;

void system_logic_vars_init(){
	node_current = menu_nodelist_init();
	screen_buffer_init();
	screen_vals_init();
	menu_vals_init();
	sys_vals.is_calibrated = 0;
}

sys_val_t* sys_vals_get(){
	return &sys_vals;
}


void system_loop() {
	
	// Joystick position update
	joy_pos = readJoystick();

	switch (sys_vals.gamemode) {
		case 0: // Gamemode: Menu
			menu_nav(&node_current, &joy_pos);
			screen_buffer_writemenu(&node_current);
			break;
		
		case 1: // Gamemode: Calibration
			screen_buffer_writecalibrate(&node_current);
			break;
		case 2: // Gamemode: Game
			screen_buffer_writegame(&node_current);
			break;
	}
	
	CAN_data[0] = joy_pos.x;
	CAN_data[1] = joy_pos.y;
	CAN_data[2] = PINB;
	CAN_data[3] = sys_vals.gamemode;
	CAN_data[4] = sys_vals.settings;
	CAN_message_transmitt(CAN_data);
		


}