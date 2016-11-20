// Created by: Lars-Erik Notevarp Bj�rge, Trond H�bertz Emaus, Kristian Husev�g Krohn

// Dependencies
#include <avr/io.h>
#include <avr/interrupt.h>

// Drivers
#include "sram.h"
#include "setup.h"
#include "fn_init.h"
#include "can.h"
#include "uart.h"
#include "menu.h"
#include "adc.h"
#include "oled.h"
#include "screen.h"
#include "system_logic.h"

//#include "buzzer.h"
//#include "music.h"
//#include "drawings.h"


int main()
{
	// Disable global interrupt while init
	cli();
	
	// Init systems
	sram_init();
	uart_init(MYUBRR);
	interrupt_init();
	oled_init();
	CAN_init();
	mfcard_io_init();
	system_logic_vars_init();
	scoretimer_init();
	
	// Enable global interrupt
	sei();

	while(1){

		//ADC_debug();
		system_loop();
	}
	return 0;	
}
