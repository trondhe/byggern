// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

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
	
	
	// Music init
	//DDRD |= (1 << PIND5);		//Set D5(OC1A) as output
	//BUZZER_init();
	//BUZZER_set_tempo(100);
	//BUZZER_start(1);
	//BUZZER_play_song();
	
	// Enable global interrupt
	sei();

	while(1){

		//ADC_debug();
		system_loop();
	}
	return 0;	
}
