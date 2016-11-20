// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

// menu.c: Driver for menu system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/delay.h>
#include "menu.h"
#include "adc.h"
#include "system_logic.h"
#include "setup.h"
#include "pwm.h"
#include "buzzer.h"
#include "music.h"


//***************************************************************
//	Menu variables init											*
//***************************************************************
static int menu_ctrl_state = 0;
static int menu_ctrl_hold = 0;
sys_val_t* sys_val = NULL;


//***************************************************************
//	Menu external values retrieval								*
//***************************************************************
void menu_vals_init(){
	sys_val = sys_vals_get();
}


//***************************************************************
//	Menu values get												*
//***************************************************************
int* menu_state_ctrl_get(){
	return &menu_ctrl_state;
}


//***************************************************************
//	Menu actions												*
//***************************************************************

// PID based position control gamemode
void play_gunmode() {
	sys_val->gamemode = 2;					// Set game mode
	s_bit(sys_val->settings, SETT_JOY_GUN); // Set joystick mode

	// Calibration code, seemingly not needed
		//if (sys_val->calibration_info == 0) {		// Checks if system is calibrated
		//settings_guncalibration();				// Set calibration mode
		//} else {
		//sys_val->gamemode = 2;					// Set game mode
		//s_bit(sys_val->settings, SETT_JOY_GUN); // Set joystick mode
		//}

}

// Speed based feed-forward control gamemode
void play_joymode() {
	sys_val->gamemode = 2;						// Set game mode	
	c_bit(sys_val->settings, SETT_JOY_GUN);		// Set gun mode
}

// Calibration function, not needed, kept as backup
void settings_guncalibration() {
	sys_val->gamemode = 1;						// Set calibration mode
	
	screen_buffer_writesplash("Calibrate", "left side",2,4);
	screen_oled_print_buffer();
	while (!(sys_val->calibration_info==3)) {}
	screen_buffer_writesplash("Calibrate", "right side",2,4);
	screen_oled_print_buffer();
	while (!(sys_val->calibration_info==4)) {}
	screen_buffer_writesplash("Calibrate", "motor position",2,4);
	screen_oled_print_buffer();
	while (!(sys_val->calibration_info==1)) {}
	screen_buffer_writesplash("Gun", "calibrated!",2,4);
	screen_oled_print_buffer();
	_delay_ms(700);
}

// Sets the firemode of the solenoid, singlefire or automatic
void settings_firemode(){
	if(t_bit_h(sys_val->settings, SETT_SINGLE_AUTO)) {
		c_bit(sys_val->settings, SETT_SINGLE_AUTO); // Set single fire mode
		screen_buffer_writesplash("SINGLE FIRE", "ACTIVATED!",2,4);
		screen_oled_print_buffer();
		_delay_ms(550);
	} else {
		s_bit(sys_val->settings, SETT_SINGLE_AUTO); // Set automatic fire mode
		screen_buffer_writesplash("AUTOMATIC FIRE", "ACTIVATED!",2,4);
		screen_oled_print_buffer();
		_delay_ms(550);
	}
}

// Play some dank tunes
void settings_playmusic() {
	 ////Music init
	//DDRD |= (1 << PIND5);		//Set D5(OC1A) as output
	//BUZZER_init();
	//BUZZER_set_tempo(100);
	//BUZZER_start(1);
	//BUZZER_play_song();
}


//***************************************************************
//	Menu node initiation functions								*
//***************************************************************

// Create node, adds the necessary pointer for childs and parent nodes. 
node_t* menu_node_init(int num_childs, char* name, node_t* parent, void* action)
{
	// Allocate space on heap
	node_t* node = (node_t*)malloc(sizeof(node_t));
	if (node == NULL) {
		for (int i = 0; i < strlen(name); i++){
			printf("%c",name[i]);
		}
		printf("\n");
		return NULL;
	}
	
	// Initalize child count, parent node, and child nodes
	node->node_chld_count = 0;
	strcpy(node->node_name, name);
	node->node_prnt = parent;
	node->node_chld = (node_t**)malloc(sizeof(node_t*) * num_childs);
	for (int i = 0; i < num_childs; i++) {
		node->node_chld[i] = NULL;
	}

	// Adds node to child of parent
	if (parent != NULL) {
		parent->node_chld[parent->node_chld_count] = node;
		parent->node_chld_count++;
	}

	// Add function pointer for menu action 
	node->action = action;
	return node;
}

// Node structure
node_t* menu_nodelist_init() {
	//NODE TEMPLATE
	// 	node_t* node_ = node_init(7, "", &node_, functionptr);
	
	// MOTHER NODE
	node_t* node_mthr = menu_node_init(2, "Home", NULL, NULL);

	// CHILD NODES,	CHILD OF CHILD, ...
	node_t* node_play = menu_node_init(2, "Play", node_mthr, NULL);
	node_t* node_gun = menu_node_init(0, "Gun", node_play, &play_gunmode);
	node_t* node_joy = menu_node_init(0, "Joystick", node_play, &play_joymode);

	node_t* node_settings = menu_node_init(2, "Settings", node_mthr, NULL);
	//node_t* node_calibrate = menu_node_init(0, "Calibrate gun", node_settings, &settings_guncalibration);
	node_t* node_firemode = menu_node_init(0, "Firemode", node_settings, &settings_firemode);
	node_t* node_music = menu_node_init(0, "Play music", node_settings, &settings_playmusic);
	
	return node_mthr;
}



//***************************************************************
//	Menu navigation logic										*
//***************************************************************
void menu_nav(node_t** node_current, joy_position* joy_pos) {	
	// Scrolling restriction reset
	if (joy_pos->direction == 5) {
		menu_ctrl_hold = 0;
	}
	
	// Menu control
	if (menu_ctrl_hold == 0) {
		switch (joy_pos->direction) {
			case 8:	// Menu control direction: UP
				if ((**node_current).node_chld_count == 0){
					break;
				}
				else if (menu_ctrl_state == 0) {
					menu_ctrl_state = ((**node_current).node_chld_count - 1);
					menu_ctrl_hold = 1;
					break;
				}
				else {
					menu_ctrl_state -= 1;
					menu_ctrl_hold = 1;
				}
				break;
				
			case 2:	// Menu control direction: DOWN
				if ((**node_current).node_chld_count == 0){
					break;
				}
				 else if (menu_ctrl_state == ((**node_current).node_chld_count - 1)) {
					menu_ctrl_state = 0;
					menu_ctrl_hold = 1;
					break;
				}
				else {
					menu_ctrl_state += 1;
					menu_ctrl_hold = 1;
				}
				break;
				
			case 6:	// Menu control direction: RIGHT
				if ((**node_current).node_chld[menu_ctrl_state]->node_chld_count == 0) {
					(**node_current).node_chld[menu_ctrl_state]->action();
					menu_ctrl_hold = 1;
					break;
				}
				*node_current = (**node_current).node_chld[menu_ctrl_state];
				menu_ctrl_hold = 1;
				menu_ctrl_state = 0;
				break;
				
			case 4:	// Menu control direction: LEFT
				if ((**node_current).node_prnt == NULL) {
					menu_ctrl_hold = 1;
					break;
				}
				*node_current = (**node_current).node_prnt;
				menu_ctrl_hold = 1;
				menu_ctrl_state = 0;
				break;
		}
	}
}