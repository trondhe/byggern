// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

// screen.c: Driver for screen output buffer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/delay.h>
#include "oled.h"
#include "menu.h"
#include "screen.h"
#include "adc.h"
#include "system_logic.h"


//***************************************************************
//	Screen variables init										*
//***************************************************************
static int* menu_ctrl_state_ptr = NULL;
static char** buffer = NULL;
static sys_val_t* sys_vals = NULL;

//***************************************************************
//	Screen external values retrieval							*
//***************************************************************
void screen_vals_init(){
	menu_ctrl_state_ptr = menu_state_ctrl_get();
	sys_vals = sys_vals_get();
}


//***************************************************************
//	Screen buffer memory allocation								*
//***************************************************************
void screen_buffer_init() {
	
	buffer = (char**)malloc(sizeof(char*) * 8);
	if (buffer == NULL) {
		printf("buffer** not allocated\n");
	}
	
	for (int i = 0; i < 8; i++) {
		buffer[i] = malloc(sizeof(char) * 16);
		if (buffer[i] == NULL) {
			printf("buffer[%d] not allocated\n",i);
		}
	}
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 16; j++) {
			buffer[i][j] = 32;
		}
	}
}


//***************************************************************
//	Screen write functions										*
//***************************************************************
void screen_buffer_writemenu(node_t** node_current){

	// Screen clear
	screen_clear_buffer();
	
	// Write current node as header
	screen_textcentering((**node_current).node_name, 0, 61);

	// Write child nodes
	for (int i = 0; i < (**node_current).node_chld_count; i++) {
		for (int j = 0; j < strlen((**node_current).node_chld[i]->node_name); j++) {
			buffer[i+1][j+1] = (**node_current).node_chld[i]->node_name[j];
		}
		for (int j = strlen((**node_current).node_chld[i]->node_name); j < 16; j++) {
			buffer[i+1][j+1] = 32;
		}
	}

	// Write guide arrow
	for (int i = 1; i < 8; i++) {
		if (i == *menu_ctrl_state_ptr + 1)
		{
			buffer[i][0] = 45;
		}
		else {
			buffer[i][0] = 32;
		}
	}

	// Write highscore at bottom of screen
	screen_writehighscore(5);
}

// Remnant of a not needed system, kept for future purposes
void screen_buffer_writecalibrate(){

	// Screen clear
	screen_clear_buffer();
}

// Writes screen with score while in game
void screen_buffer_writegame(){
	screen_clear_buffer();
	if ((sys_vals->settings == 1 | (sys_vals->settings == 0))) {
		screen_textcentering("Joystick mode", 0, 32);
		screen_textcentering("SCORE", 2, 32);
		screen_writescore(4);
	} else if ((sys_vals->settings == 2 | (sys_vals->settings == 3))) {
		screen_textcentering("Gun mode", 0, 32);
		screen_textcentering("SCORE", 2, 32);
		screen_writescore(4);
	}
	
}

// Writes endscreen to buffer
void screen_buffer_writeend (int failtype) {
	screen_clear_buffer();

	if ((sys_vals->score_counter)>(sys_vals->score_top)) {
		sys_vals->score_top = sys_vals->score_counter;
		screen_buffer_writesplash("NEW", "HIGHSCORE!",1,2);
	} else if (failtype == 0) {
		screen_textcentering("Quitter!", 2, 32);
		screen_textcentering("End score", 2, 32);
	} else if (failtype == 1) {
		screen_textcentering("Game over!", 2, 32);
		screen_textcentering("End score", 2, 32);
	}
	screen_writescore(4);
	screen_oled_print_buffer();
	_delay_ms(1500);
}

// Write a splashscreen to buffer with two centered lines of text
void screen_buffer_writesplash(char* string1, char* string2, int line1, int line2) {
	screen_clear_buffer();
	screen_textcentering(string1, line1, 32);
	screen_textcentering(string2, line2, 32);
}

// Write score on line
void screen_writescore(int line) {
	char* score_container_tmp[get_int_len(sys_vals->score_counter)];
	sprintf(score_container_tmp, "%d", sys_vals->score_counter);
	screen_textcentering(score_container_tmp, line, 32);
}

// Write highscore on line+1 with splashtext on top
void screen_writehighscore(int line) {
	screen_textcentering("Highscore:", line, 32);
	char* score_container_tmp[get_int_len(sys_vals->score_top)];
	sprintf(score_container_tmp, "%d", sys_vals->score_top);
	screen_textcentering(score_container_tmp, line+1, 32);
}

// Centers string on line with possible fill
void screen_textcentering(char* string, int row, char fill) {
	int node_name_len = strlen(string);
	int buffer_space_remaining = 16 - node_name_len;
	int buffer_space_remhalf = buffer_space_remaining / 2;
	int buffer_space_modulo = buffer_space_remaining % 2;

	for(int i = 0; i < buffer_space_remhalf; i++){
		buffer[row][i] = fill;
	}
	for(int i = buffer_space_remhalf; i < buffer_space_remhalf+node_name_len; i++){
		buffer[row][i] = string[i-buffer_space_remhalf];
	}
	for(int i = buffer_space_remhalf+node_name_len+buffer_space_modulo; i < 16; i++){
		buffer[row][i] = fill;
	}
}

// Resets the buffer to blank
void screen_clear_buffer() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 16; j++) {
			buffer[i][j] = 32;
		}
	}
}

// Prints the buffer to OLED
void screen_oled_print_buffer() {
	OLED_print_buffer(buffer);
}

// Gets length of int for use in textcentering
int get_int_len (int value){
	int l=1;
	while(value>9){ l++; value/=10; }
	return l;
}