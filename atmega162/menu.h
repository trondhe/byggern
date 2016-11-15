// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

typedef struct node_t {
	char node_name[16];
	struct node_t *node_prnt;
	struct node_t **node_chld;
	int node_chld_count;
	int (*action)();
} node_t;

node_t* node_init(int num_childs, char* name, node_t* parent, void* action);
node_t* node_menuinit();
void menu_nav(node_t** node_current);
int* menuctrl_state_pass2c();
void testfunction_print();