#include <AS5600.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <SPI.h>
#include <mcp2515.h>


#define dir 4
#define pul 3

AS5600 enc1;
AccelStepper stepper1(AccelStepper::DRIVER, pul,dir);
MCP2515 mcp2515(10);

struct can_frame canMsg1;


float setaspeed(float accelera, float maxspeed) {
    stepper1.setMaxSpeed(maxspeed);
    stepper1.setAcceleration(accelera);

}

float encoderCheck(){
  if (enc1.isConnected()) {  
    Serial.println("ENC BAĞLANDI");
    return 1.0; 
  }
  else {
    Serial.println("ENC BAĞLANAMADI");
    return 0.0;  }
}

float readEncoderAngle(){
  int rawAngle = enc1.rawAngle(); 
  float angleInDegrees = rawAngle * 0.087890625;
  Serial.print("\tend 1Derece: ");
  Serial.println(angleInDegrees);
  return angleInDegrees; 
  
}




void setup() {
  Serial.begin(115200);
  Wire.begin();
  enc1.begin();
  encoderCheck();
  setaspeed(50,200);
  canMsg1.can_id  = 0x001;
  canMsg1.can_dlc = 1;
  canMsg1.data[0] = 250;
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
}



void loop() {
  stepper1.moveTo(30*4);
  while (stepper1.distanceToGo() != 0)
    {
      float a= readEncoderAngle();
      canMsg1.data[0] = 210 ;
      stepper1.run();
      mcp2515.sendMessage(&canMsg1);
      
}
}