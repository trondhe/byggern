// Created by: Lars-Erik Notevarp Bj�rge, Trond H�bertz Emaus, Kristian Husev�g Krohn
#pragma once

#include <avr/io.h>

void adc_init(void);
uint16_t adc_read(uint8_t channel);

