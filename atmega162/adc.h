// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

#include <avr/interrupt.h>
#include <avr/io.h>
#include "uart.h"

#define JOYSTICK_DIGITAL_TRESH 95

typedef enum{
	JOYSTICK_Y  = 5,
	JOYSTICK_X  = 4,
	SLIDER_L	= 6,
	SLIDER_R    = 7
} ADC_channel;

typedef struct joy_position{
	int x;
	int y;
	int direction;
} joy_position;

void adcRead(ADC_channel ch);
void ADC_debug(void);
joy_position readJoystick(void);
