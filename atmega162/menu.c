// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

// MENU.c: Driver for menu system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "adc.h"

// TESTING IN PROGRESS	///////////////////////////////////////////////

void testfunction_print(){
	printf("\n\nPENIS ON THE SCREEN\n\n");
}

//		END TESTING		///////////////////////////////////////////////

static int menuctrl_state = 0;
static int menuctrl_hold = 0;
static joy_position menuctrl;


int* menuctrl_state_pass2main(){
	return &menuctrl_state;
}

node_t* node_init(int num_childs, char* name, node_t* parent, void* action)
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

node_t* node_menuinit() {
	//NODE TEMPLATE
	// 	node_t* node_ = node_init(7, "", &node_, functionptr);
	
	// MOTHER NODE
	node_t* node_mthr = node_init(2, "Home", NULL, NULL);

	// CHILD NODES,	CHILD OF CHILD, ...
	node_t* node_play = node_init(2, "Play", node_mthr, NULL);
	node_t* node_norm = node_init(0, "Normal", node_play, &testfunction_print);
	node_t* node_hard = node_init(0, "Hard", node_play, NULL);

	node_t* node_sett = node_init(2, "Settings", node_mthr, NULL);
	node_t* node_tuni = node_init(0, "Tuning", node_sett, NULL);
	node_t* node_dead = node_init(0, "Deadzone", node_sett, NULL);
	
	return node_mthr;
}

void menu_nav(node_t** node_current) {	
	// Scrolling restriction reset
	if (menuctrl.direction == 5) {
		menuctrl_hold = 0;
	}
	
	// Menu control
	menuctrl = readJoystick();
	if (menuctrl_hold == 0) {
		switch (menuctrl.direction) {
			case 8:	// Menu control direction: UP
				if ((**node_current).node_chld_count == 0){
					break;
				}
				else if (menuctrl_state == 0) {
					menuctrl_state = ((**node_current).node_chld_count - 1);
					menuctrl_hold = 1;
					break;
				}
				else {
					menuctrl_state -= 1;
					menuctrl_hold = 1;
				}
				break;
				
			case 2:	// Menu control direction: DOWN
				if ((**node_current).node_chld_count == 0){
					break;
				}
				 else if (menuctrl_state == ((**node_current).node_chld_count - 1)) {
					menuctrl_state = 0;
					menuctrl_hold = 1;
					break;
				}
				else {
					menuctrl_state += 1;
					menuctrl_hold = 1;
				}
				break;
				
			case 6:	// Menu control direction: RIGHT
				if ((**node_current).node_chld[menuctrl_state]->node_chld_count == 0) {
					(**node_current).node_chld[menuctrl_state]->action();
					menuctrl_hold = 1;
					break;
				}
				*node_current = (**node_current).node_chld[menuctrl_state];
				menuctrl_hold = 1;
				menuctrl_state = 0;
				break;
				
			case 4:	// Menu control direction: LEFT
				if ((**node_current).node_prnt == NULL) {
					menuctrl_hold = 1;
					break;
				}
				*node_current = (**node_current).node_prnt;
				menuctrl_hold = 1;
				menuctrl_state = 0;
				break;
		}
	}
}