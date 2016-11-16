// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

// screen.c: Driver for screen output buffer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "oled.h"
#include "menu.h"
#include "screen.h"
#include "adc.h"


static int* menu_ctrl_state;

void menu_ctrl_state_init(){
	menu_ctrl_state = menu_state_ctrl_get();
}

char** screen_buffer_init() {
	
	char** buffer = (char**)malloc(sizeof(char*) * 8);
	if (buffer == NULL) {
		printf("buffer** not allocated\n");
		return NULL;
	}
	
	for (int i = 0; i < 8; i++) {
		buffer[i] = malloc(sizeof(char) * 16);
		if (buffer[i] == NULL) {
			printf("buffer[%d] not allocated\n",i);
			return NULL;
		}
	}
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 16; j++) {
			buffer[i][j] = 32;
		}
	}
	return buffer;
}


void screen_buffer_writemenu(char** buffer, node_t** node_current){

	// Screen clear
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 16; j++) {
			buffer[i][j] = 32;
		}
	}
	
	// Write current node on top
	int node_name_len = strlen((**node_current).node_name);
	int buffer_space_remaining = 16 - node_name_len;
	int buffer_space_remhalf = buffer_space_remaining / 2;
	int buffer_space_modulo = buffer_space_remaining % 2;

	for(int i = 0; i < buffer_space_remhalf; i++){
		buffer[0][i] = 61;
	}
	for(int i = buffer_space_remhalf; i < buffer_space_remhalf+node_name_len; i++){
		buffer[0][i] = (**node_current).node_name[i-buffer_space_remhalf];
	}
	for(int i = buffer_space_remhalf+node_name_len+buffer_space_modulo; i < 16; i++){
		buffer[0][i] = 61;
	}

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
		if (i == *menu_ctrl_state + 1)
		{
			buffer[i][0] = 45;
		}
		else {
			buffer[i][0] = 32;
		}

	OLED_print_buffer(buffer);
	}
}

void screen_buffer_writegame(char** buffer){
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 16; j++) {
			buffer[i][j] = 35;
		}
	}

	OLED_print_buffer(buffer);
}