// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "mcp.h"

typedef struct CAN_message_t{
	int id;
	uint8_t length;
	int8_t data[8];
} CAN_message_t;


void CAN_message_transmitt(int* data);
void CAN_init();
int CAN_error(void);
void CAN_byte_send(CAN_message_t* message);
void CAN_data_receive();
ISR(INT0_vect);
