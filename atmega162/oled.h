// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

#include <stdio.h>
#include <avr/io.h>
#include <string.h>
#include "uart.h"

void init_OLED();
void cmd_OLED(char cmd);
void data_OLED(char data);
int OLED_print_char(char c);
int OLED_print(char *data);
int OLED_home(void);
void OLED_clear_line(uint8_t line);
void OLED_clear(void);
void OLED_goto_line(uint8_t line);
void OLED_ARROW();
void OLED_print_buffer(char** buffer);
