// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include "spi.h"

#define SPI_SS PB4
#define SPI_MOSI PB5
#define SPI_MISO PB6
#define SPI_SCK PB7


//***************************************************************
//	Initilization of SPI										*
//***************************************************************
void SPI_Init(void) {
	DDRB |= (1<<SPI_MOSI) | (1<<SPI_SCK) | (1<<SPI_SS);	// Set MOSI and SCK output 
	DDRB &= ~(1<<SPI_MISO);								// Set MISO as input pin
	PORTB |= (1<<DDB6);									// Set internal pullup for MISO
	SPCR |= (1<<SPE)|(1<<MSTR);							// Enable SPI, Master
	SPSR |= (1<<SPI2X);									// Set fosc/2
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
	// Set !SS to 0 to select slave
	PORTB &= ~(1<<SPI_SS);
}


//***************************************************************
//	SPI Slave-Deselect											*
//***************************************************************
void SPI_deselect(void) {
	// Set !SS to 1 to deselect slave
	PORTB |= (1<<SPI_SS);
}
