// Created by: Lars-Erik Notevarp Bj�rge, Trond H�bertz Emaus, Kristian Husev�g Krohn
#pragma once

void SPI_Init(void);
void SPI_MasterTransmit(char cData);
char SPI_SlaveReceive(void);
void SPI_select(void);
void SPI_deselect(void);
