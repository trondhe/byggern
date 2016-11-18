#pragma once

#define SETT_JOY_GUN 0
#define SETT_SINGLE_AUTO 1

typedef struct sys_val_t {
	int gamemode; // Menu 0  -  Calibration 1  -  Game 2
	int settings; // bit0: Firemode (auto | single), bit1: Controlmode (Joystick|Gun) 
	int is_calibrated;
} sys_val_t;



void system_logic_vars_init();
sys_val_t* sys_vals_get();
void system_loop();
