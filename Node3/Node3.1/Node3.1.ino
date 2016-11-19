#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);
unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
const int pingPin = 7;
long duration;
unsigned long startTime;
unsigned long currentTime;

long ping () {
  long duration;
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
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  while (CAN_OK != CAN.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ))              // init can bus : baudrate = 500k
  {
    Serial.println(CAN_OK);
    Serial.println("CAN BUS Shield init fail");
    Serial.println(" Init CAN BUS Shield again");
    delay(100);
  }
  Serial.println("CAN BUS Shield init ok!");
  startTime = millis();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime=millis();

  if (currentTime >= startTime){
    startTime =+ 50; //Koden under blir kjørt hvert 50ms
    
    duration = ping();
    //duration = map(duration, 870, 2800, 0, 255);
    //duration = constrain(duration, 0, 255);
    Serial.println(duration);
    analogWrite(3, duration);
    stmp[0] = duration >> 24;
    stmp[1] = duration >> 16;
    stmp[2] = duration >> 8;
    stmp[3] = duration;
    stmp[0] = 4;
    CAN.sendMsgBuf(0x10, 0, 8, stmp);
  }
}
