// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "mcp.h"

typedef struct CAN_message_t{
	int id;
	uint8_t length;
	int8_t* data;
} CAN_message_t;

CAN_message_t* CAN_message_recieve_get(void);
CAN_message_t* CAN_message_init(int id, uint8_t length);
void can_init(void);
void CAN_byte_send(CAN_message_t* message);
int CAN_error(void);
void CAN_data_receive(void);
ISR(INT0_vect);
