#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comm.h"
#include "can.h"

	volatile CAN_message_t* CAN_message_recieve = NULL;
	CAN_message_t* CAN_message_send = NULL;

	void CAN_messages_init(){
		CAN_message_recieve = CAN_message_recieve_get();
		CAN_message_send = CAN_message_init(1, 3);
	}


	void CAN_message_transmit(int* data){
		for(int i = 0; i < CAN_message_send->length; i++) {
			CAN_message_send->data[i] = data[i];
			
		}
		//printf("x: %d\ty: %d\tshoot: %d\n",CAN_message_send->data[0],
										 //CAN_message_send->data[1],
										 //CAN_message_send->data[2]);
		CAN_byte_send(CAN_message_send);
	}