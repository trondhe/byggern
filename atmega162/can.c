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
#include "uart.h"
#include "system_logic.h"

volatile static CAN_message_t CAN_message_recieve;
static CAN_message_t CAN_message_send;
sys_val_t* sys_vals_ptr;

CAN_message_t* CAN_message_recieve_get(){
	return &CAN_message_recieve;
}

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
	CAN_bitModify(MCP_CNF1, 0x00, 0x00);
	CAN_bitModify(MCP_CNF2, 0xF0, 0xF0);
	CAN_bitModify(MCP_CNF3, 0x86, 0x86);
	CAN_message_send.id = 50;
	CAN_message_send.length = 8;
	sys_vals_ptr = sys_vals_get();
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

void CAN_sys_vals_autoupdate(){
	if(CAN_message_recieve.id == 30){
		sys_vals_ptr->IR_status = CAN_message_recieve.data[0];
		sys_vals_ptr->calibration_info = CAN_message_recieve.data[1];
	}
}

ISR(INT0_vect){
	CAN_data_receive();
	CAN_sys_vals_autoupdate();
	//if(CAN_message_recieve.id == 30) {
		//UART_print_char("IR LED: ");
		//UART_print_int(CAN_message_recieve.data[0]);
		//UART_print_char("\n");
		//} else {
		//UART_print_char("unknown id: ");
		//UART_print_int(CAN_message_recieve.id);
		//UART_print_char("\n");
	//}
}