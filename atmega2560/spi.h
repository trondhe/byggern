// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn
#pragma once

void SPI_Init(void);
void SPI_MasterTransmit(char cData);
char SPI_SlaveReceive(void);
void SPI_select(void);
void SPI_deselect(void);
