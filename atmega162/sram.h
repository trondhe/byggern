// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"

void SRAM_init(void);
void SRAM_write(unsigned int address, unsigned int data);
unsigned int SRAM_read(unsigned int address);
void SRAM_test(void);
