// Created by: Lars-Erik Notevarp Bj�rge, Trond H�bertz Emaus, Kristian Husev�g Krohn
#pragma once

// Defines
#define F_OSC 4915200
#define F_CPU F_OSC
#define BAUD 9600
#define MYUBRR (F_OSC-250)/16/BAUD-1

