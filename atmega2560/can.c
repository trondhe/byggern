// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcp.h"
#include "can.h"

//***************************************************************
//	CAN variables init											*
//***************************************************************
volatile CAN_message_t CAN_message_recieve;
CAN_message_t* CAN_message_pass2main(){
	return &CAN_message_recieve;
}


//***************************************************************
//	Initialization of CANbus									*
//***************************************************************
void CAN_init(){
	
	// Initialize MCP to allow CAN transfer
	mcp_init();
	
	// Rollover disable and normal mode
	CAN_bitModify(MCP_RXB0CTRL,0b00000100, 0xFF);					
	CAN_bitModify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);		
	
	// Set baudrate equal to 500KBs
	CAN_bitModify(MCP_CNF1, 0x00, 0x00);	
	CAN_bitModify(MCP_CNF2, 0xF0, 0xF0);
	CAN_bitModify(MCP_CNF3, 0x86, 0x86);
	
	// Enable interrupt on CAN receive
	CAN_bitModify(MCP_CANINTE, 0x01, 1);
}


//***************************************************************
//	Initialization of interrupt on CAN							*
//***************************************************************
void CAN_interruptInit(void){
	// Set pin PE4 as input and interrupt pin
	PORTE |= (1<<PE4);
	DDRE &= ~(1<<PE4);
	
	// Enable INT4 and trigger at falling edge
	EIMSK = (1<<INT4);		
	EICRB = 1<<ISC41 | 0<<ISC40;
}


//***************************************************************
//	Check for errors over CAN									*
//***************************************************************
int CAN_error(void) {
	
	// Get data from CAN
	uint8_t error = CAN_read(MCP_TXB0CTRL);
	
	// Transmission error detected	
	if (test_bit(error, 4)) return -1;
	
	// Message lost arbitration
	if (test_bit(error, 5)) return -2;
	
	// No error
	return 0;
}


//***************************************************************
//	Send data over CAN											*
//***************************************************************
void CAN_byte_send(CAN_message_t* message){
	
	// Set the message id 
	CAN_write(MCP_TXB0SIDH, (int8_t)(message->id >> 3));
	CAN_write(MCP_TXB0SIDL, (int8_t)(message->id << 5));
		
	// Set data length and use data frame
	CAN_write(MCP_TXB0DLC, (0x0F) & (message->length));

	// Set data bytes
	for (int i = 0; i < message->length; i++) {
		CAN_write(MCP_TXB0D0 + i, message->data[i]);
	}
	
	// Ask for permission to send
	CAN_requestToSend(MCP_RTS_TX0);
}


//***************************************************************
//	Receive data over CAN										*
//***************************************************************
void CAN_data_receive() {

	// Get message id
	CAN_message_recieve.id  = (CAN_read(MCP_RXB0SIDH) << 3) | (CAN_read(MCP_RXB0SIDL) >> 5);
	
	// Get message length
	CAN_message_recieve.length = (0x0F) & (CAN_read(MCP_RXB0DLC));
	
	// Get message data
	for(int i = 0; i < CAN_message_recieve.length; i++) {
		CAN_message_recieve.data[i] = CAN_read(MCP_RXB0D0 + i);
	}
	
	//Clear interrupt flag
	CAN_bitModify(MCP_CANINTF, 0x01, 0);	// Clear interrupt

}


//***************************************************************
//	Interrupt vector for CAN									*
//***************************************************************
ISR(INT4_vect){
	CAN_data_receive();
}