// Created by: Lars-Erik Notevarp Bj�rge, Trond H�bertz Emaus, Kristian Husev�g Krohn
#pragma once

#include <stdio.h>
#include <avr/io.h>
#include <string.h>
#include "uart.h"

char** screenbuffer_init();
void buffer_writemenu(char** buffer, node_t** node_current, int* menuctrl_state);
void buffer_writegame(char** buffer);
