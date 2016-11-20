// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/interrupt.h>
#include "dac.h"
#include "TWI_Master.h"


//***************************************************************
//	Initialization of DAC										*
//***************************************************************
void DAC_init(){
	TWI_Master_Initialise();
}


//***************************************************************
//	Write data through DAC										*
//***************************************************************
void DAC_write(uint8_t val){
	
	// Data packet structure for TWI
	unsigned char msg[3];
	msg[0] = 0x50;
	msg[1] = 0x00;
	msg[2] = val;

	// Send over I2C bus
	TWI_Start_Transceiver_With_Data(msg, 3);
}




