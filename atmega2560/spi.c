// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include <avr/io.h>
#include "spi.h"

#define SPI_SS PB0
#define SPI_MOSI PB2
#define SPI_MISO PB3
#define SPI_SCK PB1
#define SS PB7

void SPI_Init(void) {
	DDRB |= (1<<SPI_MOSI) | (1<<SPI_SCK) | (1<<SPI_SS) | (1<<SS);		//Set MOSI and SCK output, all others input 
	DDRB &= ~(1<<SPI_MISO);												//Set MISO as input pin (slave init)
	PORTB |= (1<<DDB3);													//Set internal pullup for MISO
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR1) | (1<<SPR0);				//Enable SPI, Master, set clock rate fck/128 
}

void SPI_MasterTransmit(char cData) {
	SPDR = cData;														// Start transmission
	while(!(SPSR & (1<<SPIF)));											// Wait for transmission
}

char SPI_SlaveReceive(void) {
	SPI_MasterTransmit(0);
	while(!(SPSR & (1<<SPIF)));											// Wait for reception
	return SPDR;														// Return data register
}

void SPI_select(void) {
	PORTB &= ~(1<<SS);													//Set !SS to 0 to select slave
}

void SPI_deselect(void) {
	PORTB |= (1<<SS);													//Set !SS to 1 to deselect slave
}