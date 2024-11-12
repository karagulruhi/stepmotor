#include <AS5600.h>
#include <AccelStepper.h>
#include <Wire.h>



#define dir 4
#define pul 3

AS5600 enc1;
AccelStepper stepper1(AccelStepper::DRIVER, pul,dir);


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
}



void loop() {
  stepper1.moveTo(120);
  while (stepper1.distanceToGo() != 0)
    {
      readEncoderAngle();
      stepper1.run();

}
}