#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);


void setup() {
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    Serial.print("CAN ID: ");
    Serial.print(canMsg.can_id, DEC);   
    Serial.print(" DLC: ");
    Serial.print(canMsg.can_dlc, DEC);  
    Serial.println();

    
    if (canMsg.can_dlc == 4) {
      uint32_t newAngle = (canMsg.data[0] << 24) | (canMsg.data[1] << 16) | (canMsg.data[2] << 8) | canMsg.data[3];

      
      float angleInDegrees = newAngle / 100.0;  
      
      
      Serial.print("Decoded Angle: ");
      Serial.println(angleInDegrees);
    } else {
      Serial.println("Invalid message length");
    }
  }
}


