// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include "spi.h"
#include "mcp.h"


//***************************************************************
//	Initialization of MCP										*
//***************************************************************
void mcp_init(){
	SPI_Init();
	CAN_reset();
}


//***************************************************************
//	Read data from CAN											*
//***************************************************************
uint8_t CAN_read(uint8_t address){
	volatile uint8_t data;
	
	// Select SPI slave
	SPI_select();
	
	// Read from SPI
	SPI_MasterTransmit(MCP_READ);	
	SPI_MasterTransmit(address);
	data = SPI_SlaveReceive();
	
	// Deselect SPI slave
	SPI_deselect();
	
	// Return CAN data
	return data;
}


//***************************************************************
//	Write data on CAN											*
//***************************************************************
void CAN_write(uint8_t address, uint8_t data){
	
	// Select SPI slave
	SPI_select();
	
	// Write through SPI
	SPI_MasterTransmit(MCP_WRITE);		
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(data);
	
	// Deselect SPI slave
	SPI_deselect();
}


//***************************************************************
//	Reset CAN													*
//***************************************************************
void CAN_reset(){
	
	// Select SPI slave
	SPI_select();
	
	// Send command to reset CAN
	SPI_MasterTransmit(MCP_RESET);
	
	// Deselect SPI slave
	SPI_deselect();
}


//***************************************************************
//	Request to send on CAN										*
//***************************************************************
void CAN_requestToSend(int reg){
	
	// Select SPI slave
	SPI_select();
	
	// Send request to send
	SPI_MasterTransmit(reg);
	
	// Deselect SPI slave
	SPI_deselect();
}


//***************************************************************
//	Read status from CAN										*
//***************************************************************
uint8_t CAN_readStatus(){
	
	volatile uint8_t status;
	
	// Select SPI slave
	SPI_select();
	
	// Send command to read status from CAN
	SPI_MasterTransmit(MCP_READ_STATUS);
	status = SPI_SlaveReceive();
	
	// Deselect SPI slave	
	SPI_deselect();
	
	return status;
}


//***************************************************************
//	Modify CAN settings											*
//***************************************************************
void CAN_bitModify(uint8_t address, uint8_t mask, uint8_t bitData){
	
	// Select SPI slave
	SPI_select();
	
	// Send command to modify settings
	SPI_MasterTransmit(MCP_BITMOD);	
	
	// Settings to be modified and corresponding address
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(mask);
	SPI_MasterTransmit(bitData);
	
	// Deselect SPI slave
	SPI_deselect();
}