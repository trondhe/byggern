// Created by: Lars-Erik Notevarp Bj�rge, Trond H�bertz Emaus, Kristian Husev�g Krohn
#pragma once

#include "uart.h"
#include "menu.h"

void screen_buffer_init();
void screen_buffer_writemenu(node_t** node_current);
void screen_buffer_writegame();
void write_center_text(char* text, int on_line, int if_fill, int fill);
void screen_vals_init();