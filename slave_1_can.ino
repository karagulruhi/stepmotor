#include <AS5600.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <SPI.h>
#include <mcp2515.h>

#define dir 4
#define pul 3
#define ena 7
AS5600 enc1;
AccelStepper stepper1(AccelStepper::DRIVER, pul, dir);
MCP2515 mcp2515(10);

struct can_frame canMsg1;

float setaspeed(float accelera, float maxspeed) {
    stepper1.setMaxSpeed(maxspeed);
    stepper1.setAcceleration(accelera);
}

bool encoderCheck() {
  if (enc1.isConnected()) {  
    Serial.println("ENC BAĞLANDI");
    return true; 
  } else {
    Serial.println("ENC BAĞLANAMADI");
    return false;  
  }
}

float readEncoderAngle() {
  int rawAngle = enc1.rawAngle(); 
  float angleInDegrees = rawAngle * 0.087890625;  // Convert to degrees
  delay(20);
  Serial.print("\tEnd 1 Degree: ");
  Serial.println(angleInDegrees);
  canMsg1.can_id  = 0x01;
  canMsg1.can_dlc = 4;

  uint32_t newAngle = angleInDegrees * 100;  // Convert angle to an integer representation
  uint8_t byte0 = (newAngle >> 24) & 0xFF;  // First 8 bits
  uint8_t byte1 = (newAngle >> 16) & 0xFF;  // Second 8 bits
  uint8_t byte2 = (newAngle >> 8) & 0xFF;   // Third 8 bits
  uint8_t byte3 = newAngle & 0xFF;          // Last 8 bits

  canMsg1.data[0] = byte0;
  canMsg1.data[1] = byte1;
  canMsg1.data[2] = byte2;
  canMsg1.data[3] = byte3;

  mcp2515.sendMessage(&canMsg1);
  return angleInDegrees; 
}



void setup() {
  Serial.begin(115200);
  Wire.begin();
  enc1.begin(6);
  encoderCheck();
  enc1.setDirection(AS5600_CLOCK_WISE);
  setaspeed(50, 200);
  pinMode(ena,OUTPUT);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
}

void loop() {
  
  stepper1.moveTo(30 * 4);  // Set target position
  while (stepper1.distanceToGo() != 0) {
    float a = readEncoderAngle();  // Read encoder angle
    stepper1.run();  
  }
digitalWrite(ena, HIGH);
}
