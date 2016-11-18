// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

#include "uart.h"
#include "menu.h"

void screen_vals_init();
void screen_buffer_init();
void screen_buffer_writemenu(node_t** node_current);
void screen_buffer_writegame();
void screen_buffer_writecalibrate(node_t** node_current);
void write_center_text(char* text, int on_line, int if_fill, int fill);