// Created by: Lars-Erik Notevarp Bj�rge, Trond H�bertz Emaus, Kristian Husev�g Krohn
#pragma once

#include "uart.h"
#include "menu.h"

char** screenbuffer_init();
void buffer_writemenu(char** buffer, node_t** node_current);
void buffer_writegame(char** buffer);
void menuctrl_state_init();