// Created by: Lars-Erik Notevarp Bj�rge, Trond H�bertz Emaus, Kristian Husev�g Krohn
#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "mcp.h"

typedef struct CAN_message_t{
	int id;
	uint8_t length;
	uint8_t data[8];
} CAN_message_t;


CAN_message_t* CAN_message_recieve_get();
void CAN_message_transmitt(int* data);
void CAN_init();
int CAN_error(void);
void CAN_byte_send(CAN_message_t* message);
void CAN_data_receive();
void CAN_sys_vals_autoupdate();
ISR(INT0_vect);