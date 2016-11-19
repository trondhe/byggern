// Created by: Lars-Erik Notevarp Bjørge, Trond Hübertz Emaus, Kristian Husevåg Krohn

void McpInit(){
	SPI_Init();
	CAN_reset();
}

uint8_t CAN_read(uint8_t address){
	volatile uint8_t i;
	SPI_select();
	SPI_MasterTransmit(MCP_READ);	
	SPI_MasterTransmit(address);
	i = SPI_SlaveReceive();
	SPI_deselect();
	return i;
}

void CAN_write(uint8_t address, uint8_t data){
	SPI_select();
	SPI_MasterTransmit(MCP_WRITE);		
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(data);
	SPI_deselect();
}

void CAN_reset(){
	SPI_select();
	SPI_MasterTransmit(MCP_RESET);
	SPI_deselect();
}

void CAN_requestToSend(int reg){
	SPI_select();
	SPI_MasterTransmit(reg);
	SPI_deselect();
}

uint8_t CAN_readStatus(){
	volatile uint8_t i;
	SPI_select();
	SPI_MasterTransmit(MCP_READ_STATUS);
	i = SPI_SlaveReceive();
	SPI_deselect();
	return i;
}

void CAN_bitModify(uint8_t address, uint8_t mask, uint8_t bitData){
	SPI_select();
	SPI_MasterTransmit(MCP_BITMOD);	
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(mask);
	SPI_MasterTransmit(bitData);
	SPI_deselect();
}
