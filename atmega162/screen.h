// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

#include "uart.h"
#include "menu.h"

char** screen_buffer_init();
void screen_buffer_writemenu(char** buffer, node_t** node_current);
void screen_buffer_writegame(char** buffer);
void screen_vals_init();