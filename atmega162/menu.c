// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

// menu.c: Driver for menu system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "adc.h"

static int menu_ctrl_state = 0;
static int menu_ctrl_hold = 0;

// TESTING IN PROGRESS	///////////////////////////////////////////////

void testfunction_print(){
	printf("\n\nTESTPRINT\n\n");
}

//		END TESTING		///////////////////////////////////////////////




int* menu_state_ctrl_get(){
	return &menu_ctrl_state;
}

node_t* menu_node_init(int num_childs, char* name, node_t* parent, void* action)
{
	node_t* node = (node_t*)malloc(sizeof(node_t));
	if (node == NULL) {
		for (int i = 0; i < strlen(name); i++){
			printf("%c",name[i]);
		}
		printf("\n");
		return NULL;
	}
	
	node->node_chld_count = 0;
	strcpy(node->node_name, name);
	node->node_prnt = parent;
	node->node_chld = (node_t**)malloc(sizeof(node_t*) * num_childs);
	for (int i = 0; i < num_childs; i++) {
		node->node_chld[i] = NULL;
	}
	if (parent != NULL) {
		parent->node_chld[parent->node_chld_count] = node;
		parent->node_chld_count++;
	}
	node->action = action;
	return node;
}

node_t* menu_nodelist_init() {
	//NODE TEMPLATE
	// 	node_t* node_ = node_init(7, "", &node_, functionptr);
	
	// MOTHER NODE
	node_t* node_mthr = menu_node_init(2, "Home", NULL, NULL);

	// CHILD NODES,	CHILD OF CHILD, ...
	node_t* node_play = menu_node_init(2, "Play", node_mthr, NULL);
	node_t* node_norm = menu_node_init(0, "Normal", node_play, &testfunction_print);
	node_t* node_hard = menu_node_init(0, "Hard", node_play, NULL);

	node_t* node_sett = menu_node_init(2, "Settings", node_mthr, NULL);
	node_t* node_tuni = menu_node_init(0, "Tuning", node_sett, NULL);
	node_t* node_dead = menu_node_init(0, "Deadzone", node_sett, NULL);
	
	return node_mthr;
}

void menu_nav(node_t** node_current, joy_position* joy_pos) {	
	// Scrolling restriction reset
	if (joy_pos->direction == 5) {
		menu_ctrl_hold = 0;
	}
	
	// Menu control
	//menuctrl = readJoystick();
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