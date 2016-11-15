// Created by: Lars-Erik Notevarp Bj�rge, Trond H�bertz Emaus, Kristian Husev�g Krohn
#pragma once

#include <avr/interrupt.h>
#include <avr/io.h>
#include "uart.h"

typedef enum{
	JOYSTICK_Y  = 5,
	JOYSTICK_X  = 4,
	SLIDER_L	= 6,
	SLIDER_R    = 7
} ADC_channel;

typedef struct {
	int x;
	int y;
	int direction;
} joy_position;

void adcRead(ADC_channel ch);
void interruptInit(void);
void ADC_debug(void);
joy_position readJoystick(void);