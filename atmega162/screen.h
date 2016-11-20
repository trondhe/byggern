// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

#include "uart.h"
#include "menu.h"

void screen_buffer_init();
void screen_buffer_writemenu(node_t** node_current);
void screen_buffer_writecalibrate();
void screen_buffer_writegame();
void screen_buffer_writeend (int failtype);
void screen_buffer_writesplash(char* string1, char* string2, int line1, int line2);
void screen_writescore(int line);
void screen_writehighscore(int line);
void screen_textcentering(char* string, int row, char fill);
void screen_clear_buffer();
void screen_oled_print_buffer();
int get_int_len (int value);
void screen_vals_init();