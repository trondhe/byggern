#pragma once

#define SETT_JOY_GUN 1
#define SETT_SINGLE_AUTO 0

typedef struct sys_val_t {
	int gamemode;		// Menu:0 - Calibration:1 - Game 2
	int settings;	// bit0:Firemode (auto | single) - bit1:Controlmode (Joystick|Gun)
	int calibration_info; // Not calibrated: 0 - Calibrated:1 - left:2 - right:3 - motor:4
	int16_t score_counter;		// Gamepoints based on duration
	int16_t score_top;			// Topscore this session	
	int IR_status;	// Failstate for the game, ok:0 - game over:1
	int playmusic;
} sys_val_t;



void system_logic_vars_init();
sys_val_t* sys_vals_get();
void system_loop();
ISR(TIMER1_OVF_vect);