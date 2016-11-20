// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

#include <avr/io.h>

#define MOTOR_DIRECTION		PH1
#define MOTOR_ENABLE		PH4
#define ENCODER_ACTIVE_LOW	PH5
#define ENCODER_RESET		PH6
#define ENCODER_SELECT		PH3


void motor_control(int8_t speed);
void motor_init();
int16_t motor_encoder_read();
void tune();
int8_t MOTOR_PDcontroller(int8_t xValue);
uint8_t endian_convert(uint8_t data);
void motor_encoder_calibrate();
void motor_encoder_debug(uint8_t MSB, uint8_t LSB);