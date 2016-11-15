// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include "spi.h"

#define SPI_SS PB4
#define SPI_MOSI PB5
#define SPI_MISO PB6
#define SPI_SCK PB7

void SPI_Init(void) {
	DDRB |= (1<<SPI_MOSI) | (1<<SPI_SCK) | (1<<SPI_SS);			//Set MOSI and SCK output, all others input 
	DDRB &= ~(1<<SPI_MISO);										//Set MISO as input pin (slave init)
	PORTB |= (1<<DDB6);											//Set internal pullup for MISO
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);						//Enable SPI, Master, set clock rate fck/16 
}

void SPI_MasterTransmit(char cData) {
	// Start transmission
	SPDR = cData;
	
	// Wait for transmission					
	while(!(SPSR & (1<<SPIF)));		
}


char SPI_SlaveReceive(void) {
	// Send dummy bit for initialization purposes
	SPI_MasterTransmit(0);
	
	// Wait for reception
	while(!(SPSR & (1<<SPIF)));	
		
	return SPDR;					
}

void SPI_select(void) {
	// Set !SS to 0 to select slave
	PORTB &= ~(1<<SPI_SS);		
}

void SPI_deselect(void) {
	// Set !SS to 1 to deselect slave
	PORTB |= (1<<SPI_SS);		
}