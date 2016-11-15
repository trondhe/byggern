// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/interrupt.h>
#include "dac.h"
#include "TWI_Master.h"

void DAC_init(){
	TWI_Master_Initialise();
}

void DAC_write(uint8_t val){
		unsigned char msg[3];
		msg[0] = 0x50;
		msg[1] = 0x00;
		msg[2] = val;

	TWI_Start_Transceiver_With_Data(msg, 3);
}

//char read_from_slave(){
	//
	//
	//TWI_Start_Transceiver();
	//
	//
	//TWI_Get_Data_From_Transceiver();
	//
//}



