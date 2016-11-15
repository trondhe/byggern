#include <avr/io.h>
#include <stdio.h>
#include "i2c.h"



void i2c_init( void )
{
	TWBR = 0x0C; // Baud
}


char i2c_start(char address)
{
	// reset TWI control register
	TWCR = 0;
	// transmit START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	// check if the start condition was successfully transmitted
	if((TWSR & 0xF8) != TW_START){ return 1; }
	
	// load slave address into data register
	TWDR = address;
	
	// start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	// check if the device has acknowledged the READ / WRITE mode
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;
	
	return 0;
}

char i2c_write(char data)
{
	// load data into data register
	TWDR = data;
	// start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	if( (TWSR & 0xF8) != TW_MT_DATA_ACK ){ return 1; }
	
	return 0;
}

char i2c_transmit(char address, char* data, uint16_t length)
{
	// Start
	if (i2c_start(address | I2C_WRITE)) return 1;
	
	//Command byte
	if (i2c_write(0x00)) return 1;
	
	//Send data
	for (uint16_t i = 0; i < length; i++)
	{
		if (i2c_write(data[i])) return 1;
	}
	
	i2c_stop();
	
	return 0;
}

void i2c_stop(void)
{
	// transmit STOP condition
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

unsigned char i2c_read(unsigned char *msg){
	//
	//read from slave
	unsigned char c;
	TWI_Start_Transceiver_With_Data(0x10, 1);
	c = TWI_Get_Data_From_Transceiver(*msg, 1);
	return c;
}