// Created by: Lars-Erik Notevarp Bj�rge, Trond H�bertz Emaus, Kristian Husev�g Krohn

// adc.c: Driver for ADC used for joystick signal acquisition 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "setup.h"


//***************************************************************
//	adc var init												*
//***************************************************************
volatile int adcTemp = 0;
volatile char* adc = 0x1404;


//***************************************************************
//	adc read functions											*
//***************************************************************
void adcRead(ADC_channel ch){
	*adc = ch;
}

ISR(INT1_vect)
{
	adcTemp = *adc;
}


//***************************************************************
//	adc joystick functions										*
//***************************************************************
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
	position.y = (int)(200*((((float)JoystickY()) / 255.0) - 0.5025));
	
	if (abs(position.x) > JOYSTICK_DIGITAL_TRESH || abs(position.y) > JOYSTICK_DIGITAL_TRESH){
		
		if(position.x > JOYSTICK_DIGITAL_TRESH){
			position.direction = 6;
		}
		
		else if(position.x < -JOYSTICK_DIGITAL_TRESH){
			position.direction = 4;
		}
		
		else if(position.y > JOYSTICK_DIGITAL_TRESH){
			position.direction = 8;
		}
		else if(position.y < JOYSTICK_DIGITAL_TRESH){
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


//***************************************************************
//	adc debug													*
//***************************************************************
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