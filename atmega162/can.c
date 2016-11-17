// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

// CAN.c: Driver for CAN communication between nodes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "mcp.h"
#include "can.h"
#include "setup.h"

CAN_message_t CAN_message_recieve;
CAN_message_t CAN_message_send;

void CAN_message_transmitt(int* data){
	for(int i = 0; i < CAN_message_send.length; i++) {
		CAN_message_send.data[i] = data[i];
	}
	CAN_byte_send(&CAN_message_send);
}

void CAN_init(){
	McpInit();
	CAN_bitModify(MCP_RXB0CTRL,0b00000100, 0xFF);			// Rollover disable, mask/filter off			
	CAN_bitModify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);		// Loopback mode
	CAN_bitModify(MCP_CANINTE, 0x01, 1);					// Enable interrupt
	CAN_message_send.id = 2;
	CAN_message_send.length = 8;
}

int CAN_error(void) {
	uint8_t error = CAN_read(MCP_TXB0CTRL);
	
	//Transmission error detected
	if (t_bit_h(error, 4)) return -1;
	
	//Message lost arbitration
	if (t_bit_h(error, 5)) return -2;
	
	return 0;
}

void CAN_byte_send(CAN_message_t* message){
			
	//Set the message id (use standard identifier)
	CAN_write(MCP_TXB0SIDH, (int8_t)(message->id >> 3));
	CAN_write(MCP_TXB0SIDL, (int8_t)(message->id << 5));
		
	//Set data length and use data frame (RTR = 0)
	CAN_write(MCP_TXB0DLC, (0x0F) & (message->length));

	//Set data bytes (max. 8 bytes)
	for (int i = 0; i < message->length; i++) {
		CAN_write(MCP_TXB0D0 + i, message->data[i]);
	}
		
	//CAN_write(MCP_TXB0D0, message->data[0]);
	//Request to send via TX0
	CAN_requestToSend(MCP_RTS_TX0);

}


void CAN_data_receive() {

	
	//Get message id
	CAN_message_recieve.id  = (CAN_read(MCP_RXB0SIDH) << 3) | (CAN_read(MCP_RXB0SIDL) >> 5);
	
	//Get message length
	CAN_message_recieve.length = (0x0F) & (CAN_read(MCP_RXB0DLC));
	
	//Get message data
	for(int i = 0; i < CAN_message_recieve.length; i++) {
		CAN_message_recieve.data[i] = CAN_read(MCP_RXB0D0 + i);
	}
	
	//Clear interrupt flag
	//printf("%d", rx_flag);
	CAN_bitModify(MCP_CANINTF, 0x01, 0);	// Clear interrupt

}


ISR(INT0_vect){
	CAN_data_receive();
}