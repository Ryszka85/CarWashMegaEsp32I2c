#include <PS4Controller.h>
#include <Wire.h>

#define debug(x) Serial.println(x)

struct PS4Data {
  int8_t  lStickY;
  uint8_t r1;
  uint8_t r2;
  bool cross;
  bool circle;
};

void setup() {
  Serial.begin(115200);
  Wire.begin(9);
  Wire.onRequest(requestEvent);
  
  PS4.begin("e8:61:7e:47:83:30"); // replace with your PS4 controller's MAC address
  PS4.setLed(0, 255, 0); // set LED to green
}

void loop() {    
  
}

void requestEvent() {
  
  PS4Data data = {
    .lStickY = PS4.LStickY(),
    .r1 = PS4.R1(),
    .r2 = PS4.R2(),
    .cross = PS4.Cross(),
    .circle = PS4.Circle()
  };
  debug(data.lStickY);
  Wire.write(reinterpret_cast<uint8_t*>(&data), sizeof(data));
}