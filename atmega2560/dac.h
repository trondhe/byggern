// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

#include <stdint.h>

#define MAX520_TWI_ADDR 0
#define MAX520_TWI_ADDR_BASE 0b01010000
#define MAX520_CHANNEL_MOTOR 0

void DAC_init();
void DAC_write(uint8_t val);