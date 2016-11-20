// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include "spi.h"

// Define SPI pins
#define SPI_SS PB0
#define SPI_MOSI PB2
#define SPI_MISO PB3
#define SPI_SCK PB1
#define SS PB7


//***************************************************************
//	Initilization of SPI										*
//***************************************************************
void SPI_Init(void) {
	// Set data direction for SPI pins
	DDRB |= (1<<SPI_MOSI) | (1<<SPI_SCK) | (1<<SPI_SS) | (1<<SS);		
	DDRB &= ~(1<<SPI_MISO);			
	
	// Set pullup for MISO									
	PORTB |= (1<<DDB3);			
	
	// Enable SPI, Master and set clock rate to fck/16										
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);				
}


//***************************************************************
//	Transmit via SPI											*
//***************************************************************
void SPI_MasterTransmit(char cData) {
	
	SPDR = cData;					
	
	// Wait for transmission									
	while(!(SPSR & (1<<SPIF)));											
}


//***************************************************************
//	Receive from SPI											*
//***************************************************************
char SPI_SlaveReceive(void) {
	
	// Send dummy bit
	SPI_MasterTransmit(0);
	
	// Wait for reception
	while(!(SPSR & (1<<SPIF)));											
	return SPDR;														
}


//***************************************************************
//	SPI Slave-Select											*
//***************************************************************
void SPI_select(void) {
	PORTB &= ~(1<<SS);													
}


//***************************************************************
//	SPI Slave-Deselect											*
//***************************************************************
void SPI_deselect(void) {
	PORTB |= (1<<SS);													
}