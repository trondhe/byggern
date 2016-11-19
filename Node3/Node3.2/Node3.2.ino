#include<SPI.h>



#define interruptPin 2
#define SS 9


#define MCP_RESET 0xC0
#define MCP_READ 0x03
#define MCP_WRITE 0x02
#define MCP_BITMOD 0x05
#define MCP_READ_STATUS 0xA0
#define MCP_RXB0CTRL 0x60
#define MCP_RXB0SIDH 0x61
#define MCP_RXB0SIDL 0x62
#define MCP_RXB0DLC 0x65
#define MCP_RXB0D0 0x66
#define MCP_CANCTRL 0x0F
#define MODE_MASK 0xE0
#define MODE_NORMAL 0x00
#define MODE_LOOPBACK 0x40
#define MCP_CANINTE 0x2B
#define MCP_TXB0SIDH 0x31
#define MCP_TXB0SIDL 0x32
#define MCP_TXB0DLC 0x35
#define MCP_TXB0D0 0x36
#define MCP_RTS_TX0 0x81
#define MCP_CANINTF 0x2C

unsigned long startTime;
unsigned long currentTime;
const int pingPin = 7;
unsigned long duration;
unsigned int pingDuration;


SPISettings settings(10000000, MSBFIRST, SPI_MODE0); 

typedef struct CAN_message_t{
  int id;
  uint8_t lengde;
  uint8_t data[8];
} CAN_message_t;



volatile CAN_message_t CAN_message_recieve;
CAN_message_t CAN_message_send;


void CAN_init(){
  McpInit();
  CAN_bitModify(MCP_RXB0CTRL,0x04, 0xFF);     // Rollover disable, mask/filter off      
  CAN_bitModify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);   // Normal mode
  CAN_bitModify(MCP_CANINTE, 0x01, 1);          // Enable interrupt
}

void CAN_interruptInit(void){
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), interruptHandler, RISING);
}

void CAN_byte_send(CAN_message_t* message){
  //Set the message id (use standard identifier)
  CAN_write(MCP_TXB0SIDH, (int8_t)(message->id >> 3));
  CAN_write(MCP_TXB0SIDL, (int8_t)(message->id << 5));
    
  //Set data length and use data frame (RTR = 0)
  CAN_write(MCP_TXB0DLC, (0x0F) & (message->lengde));

  //Set data bytes (max. 8 bytes)
  for (int i = 0; i < message->lengde; i++) {
    CAN_write(MCP_TXB0D0 + i, message->data[i]);
  }
    
  //CAN_write(MCP_TXB0D0, message->data[0]);
  //Request to send via TX0
  CAN_requestToSend(MCP_RTS_TX0);
}

void CAN_data_receive() {

  //Get message id
  CAN_message_recieve.id  = (CAN_read(MCP_RXB0SIDH) << 3) | (CAN_read(MCP_RXB0SIDL) >> 5);
  
  //Get message length
  CAN_message_recieve.lengde = (0x0F) & (CAN_read(MCP_RXB0DLC));
  
  //Get message data
  for(int i = 0; i < CAN_message_recieve.lengde; i++) {
    CAN_message_recieve.data[i] = CAN_read(MCP_RXB0D0 + i);
  }
  
  //Clear interrupt flag
  //printf("%d", rx_flag);
  CAN_bitModify(MCP_CANINTF, 0x01, 0);  // Clear interrupt

}

void interruptHandler(){
  CAN_data_receive();
}

void McpInit(){
  SPI.begin();
  SPI.beginTransaction(settings); // CAN reset
  digitalWrite(SS, LOW);
  SPI.transfer(MCP_RESET);
  digitalWrite(SS, HIGH);
  SPI.endTransaction;

}

uint8_t CAN_read(uint8_t address){
  volatile uint8_t i;
  SPI.beginTransaction(settings); // CAN reset
  digitalWrite(SS, LOW);
  SPI.transfer(MCP_RESET);
  SPI.transfer(address);
  i = SPI.transfer(0);
  digitalWrite(SS, HIGH);
  SPI.endTransaction;
  return i;
}

void CAN_write(uint8_t address, uint8_t data){
  SPI.beginTransaction(settings); // CAN reset
  digitalWrite(SS, LOW);
  SPI.transfer(MCP_WRITE);    
  SPI.transfer(address);
  SPI.transfer(data);
  digitalWrite(SS, HIGH);
  SPI.endTransaction;
}

void CAN_reset(){
  SPI.beginTransaction(settings); // CAN reset
  digitalWrite(SS, LOW);
  SPI.transfer(MCP_RESET);
  digitalWrite(SS, HIGH);
  SPI.endTransaction;
}

void CAN_requestToSend(int reg){
  SPI.beginTransaction(settings); // CAN reset
  digitalWrite(SS, LOW);
  SPI.transfer(reg);
  digitalWrite(SS, HIGH);
  SPI.endTransaction;
}

uint8_t CAN_readStatus(){
  volatile uint8_t i;
  SPI.beginTransaction(settings); // CAN reset
  digitalWrite(SS, LOW);
  SPI.transfer(MCP_READ_STATUS);
  i = SPI.transfer(0);
  digitalWrite(SS, HIGH);
  SPI.endTransaction;
  return i;
}

void CAN_bitModify(uint8_t address, uint8_t mask, uint8_t bitData){
  SPI.beginTransaction(settings); // CAN reset
  digitalWrite(SS, LOW);
  SPI.transfer(MCP_BITMOD); 
  SPI.transfer(address);
  SPI.transfer(mask);
  SPI.transfer(bitData);
  digitalWrite(SS, HIGH);
  SPI.endTransaction;
}

unsigned long ping () {
  unsigned long duration;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  return duration;
}

void setup() {
  delay(10);
  Serial.begin(9600);
 
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  delay(10);
  CAN_init();
  delay(10);
  CAN_interruptInit();
  delay(10);
  CAN_message_send.id = 1;
  CAN_message_send.lengde = 8;
  startTime = millis();
  Serial.println("Init Complete");
}

void loop() {
  unsigned long currentTime=millis();

  if ((currentTime - startTime) > 10 ){
    startTime += 10; //Koden under blir kjørt hvert 50ms
    duration = ping();
    pingDuration = constrain(duration, 0, 65535);
    //Serial.println(pingDuration);
    CAN_message_send.data[0] = (pingDuration >> 8);
    CAN_message_send.data[1] = pingDuration;
  
    CAN_byte_send(&CAN_message_send);
  }

  if(CAN_message_recieve.id == 1){
    Serial.println("Data motatt fra id 1");
    Serial.println(CAN_message_recieve.data[0]);
  }

    if(CAN_message_recieve.id == 2){
    Serial.println("Data motatt fra id 2");
    Serial.println(CAN_message_recieve.data[0]);
  }

    if(CAN_message_recieve.id == 3){
    Serial.println("Data motatt fra id 3");
    Serial.println(CAN_message_recieve.data[0]);
  }

}
