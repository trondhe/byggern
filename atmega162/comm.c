#include "comm.h"
#include "can.h"

	volatile CAN_message_t* CAN_message_recieve = NULL; CAN_message_recieve_get();
	CAN_message_t* CAN_message_send;
	CAN_message_send.id = 1;
	CAN_message_send.length = 8;

	void CAN_message_send_init(int id, int length) {
		CAN_message_t* CAN_message_send
	}

	void CAN_message_recieve_init(){
		CAN_message_recieve = CAN_message_recieve_get();
	}

	void CAN_message_transmit(int* data){
		for(int i = 0; i < CAN_message_send.length; i++) {
			CAN_message_send.data[i] = data
		}
		CAN_byte_send(&CAN_message_send);
	}