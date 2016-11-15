// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

// adc.c: Driver for ADC used for joystick signal acquisition 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "setup.h"

volatile int adcTemp = 0;
volatile char* adc = 0x1404;

typedef enum{
	JOYSTICK_Y  = 5,
	JOYSTICK_X  = 4,
	SLIDER_L	= 6,
	SLIDER_R    = 7
} ADC_channel;

typedef struct {
	int x;
	int y;
	int* direction;
} joy_position;


void adcRead(ADC_channel ch){
	*adc = ch;
}

ISR(INT1_vect)
{
	adcTemp = *adc;
}

uint8_t JoystickX(){
	adcRead(JOYSTICK_X);
	uint8_t x = adcTemp;
	return x;
}

uint8_t JoystickY(){
	adcRead(JOYSTICK_Y);
	uint8_t y = adcTemp;
	return y;
}

joy_position readJoystick(){
	joy_position position;
	position.x = (int)(200*((((float)JoystickX()) / 255.0) - 0.5025));
	//printf("X = %d\t ", position.x);
	position.y = (int)(200*((((float)JoystickY()) / 255.0) - 0.5025));
	//printf("Y = %d\t\t\n\n", position.y);
	
	
	if (abs(position.x) > 90 || abs(position.y) > 90){
		
		if(position.x > 90){
			position.direction = 6;
		}
		
		else if(position.x < -90){
			position.direction = 4;
		}
		
		else if(position.y > 90){
			position.direction = 8;
		}
		else if(position.y < 90){
			position.direction = 2;
		}
	}
	else{
		position.direction = 5;
		//backToStandby = 1;
	}
	//printf("%d\n",position.direction);
	return position;
}

void ADC_debug(void){
	adcRead(JOYSTICK_X);
	printf("\t\t");
	printf("JOY_X: %3.0f %%",  (float)adcTemp*0.3921);
	adcRead(JOYSTICK_Y);
	printf("\t\t");
	printf("JOY_Y: %3.0f %%",  (float)adcTemp*0.3921);
	adcRead(SLIDER_L);
	printf("\t\t");
	printf("SLIDER_LEFT: %3.0f %%", (float)adcTemp*0.3921);
	adcRead(SLIDER_R);
	printf("\t\t");
	printf("SLIDER_RIGHT: %3.0f %%",  (float)adcTemp*0.3921);

	if(t_bit_l(PINB, PB0)){
		printf("\t");
		printf("PB0 AKTIV");
	} else {
		printf("\t");
		printf("PB0 -----");
	}

	if(t_bit_l(PINB, PB1)){
		printf("\t");
		printf("PB1 AKTIV");
	} else {
		printf("\t");
		printf("PB1 -----");
	}


	printf("\r\n");
}