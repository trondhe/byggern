// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include "uart.h"

FILE *mystdio;

void uart_init(unsigned int ubrr) {
	// Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	// Set frame: Register select - 2stop bit - 8data bit
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
	
	// Connect to printf
	mystdio = fdevopen(&UART_Transmit,&UART_Receive);
}

unsigned char UART_Receive(void) {
	// Wait for data to be received
	while (!(UCSR0A & (1<<RXC0)));
	
	// Get and return received data from buffer
	return UDR0;
}

void UART_Transmit(unsigned char data) {
	// Wait for empty transmit buffer
	while (!( UCSR0A & (1<<UDRE0)));
	
	// Put data into buffer, sends the data
	UDR0 = data;
}

// Byte to char* conversion
const char* byte_to_binary(int x)
{
	static char b[9];
	b[0] = '\0';

	int z;
	for (z = 128; z > 0; z >>= 1)
	{
		strcat(b, ((x & z) == z) ? "1" : "0");
	}

	return b;
}