// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

#include <stdio.h>
#include <avr/io.h>
#include <string.h>

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR 31 //FOSC/16/BAUD-1  // Divided by 16 : Asynchronous Normal mode

extern FILE *mystdio;

void UART_Transmit( unsigned char data );
unsigned char UART_Receive( void );
void uart_init( unsigned int ubrr );
