// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

#include "adc.h"

typedef struct node_t {
	char node_name[16];
	struct node_t *node_prnt;
	struct node_t **node_chld;
	int node_chld_count;
	int (*action)();
} node_t;

node_t* menu_node_init(int num_childs, char* name, node_t* parent, void* action);
node_t* menu_nodelist_init();
void menu_nav(node_t** node_current, joy_position* joy_pos);
int* menu_state_ctrl_get();
void testfunction_print();
