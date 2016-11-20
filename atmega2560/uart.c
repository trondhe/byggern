// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

#include "uart.h"

FILE *mystdio;


//***************************************************************
//	Initilization for UART										*
//***************************************************************
void UART_Init(unsigned int ubrr) {
	
	// Set baud rate 
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	//Enable TX1 and RX1
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
	
	//Set frame format: 8data, 2stop bit
	UCSR0C |= (1<<USBS0)| (3<<UCSZ00);
	
	// Connect to printf
	mystdio = fdevopen(&UART_Transmit,&UART_Receive);
}


//***************************************************************
//	Receive data from UART										*
//***************************************************************
unsigned char UART_Receive(void) {
	
	// Wait for data to be received 
	while (!(UCSR0A & (1<<RXC0)));
	
	// Get and return received data from buffer 
	return UDR0;
}


//***************************************************************
//	Transmit data via UART										*
//***************************************************************
void UART_Transmit(unsigned char data) {
	
	// Wait for empty transmit buffer 
	while (!( UCSR0A & (1<<UDRE0)));
	
	// Put data into buffer, sends the data 
	UDR0 = data;
}


//***************************************************************
//	Print strings via UART												*
//***************************************************************
void UART_print_char(char* data) {
	for (int i = 0; i < strlen(data); i++) {
			while (!( UCSR0A & (1<<UDRE0)));
			UDR0 = data[i];
	}
}


//***************************************************************
//	Print integers via UART													*
//***************************************************************
void UART_print_int(uint16_t data) {
	char str[15];
	sprintf(str, "%d", data);
	for (int i = 0; i < strlen(str); i++) {
		while (!( UCSR0A & (1<<UDRE0)));
		UDR0 = str[i];
	}
}