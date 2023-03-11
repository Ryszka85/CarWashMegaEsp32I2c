#include <PS4Controller.h>
#include <Wire.h>

#define debug(x) Serial.println(x)

struct PS4Data {
  int8_t  lStickY;  
  uint8_t r1;
  uint8_t r2;
  uint8_t l1;
  uint8_t l2;
  bool cross;
  bool circle;
  bool isPs4Connected;
};

void setup() {
  Serial.begin(115200);
  Wire.begin(9);
  Wire.onRequest(requestEvent);
  
  PS4.begin("e8:61:7e:47:83:30"); // replace with your PS4 controller's MAC address
  PS4.setLed(0, 255, 0); // set LED to green

  debug(PS4.isConnected() == 1 ? "Is connected" : "Not connected");
}

void loop() {    
  
}

void requestEvent() {
  
  debug(PS4.isConnected() == 1 ? "Is connected" : "Not connected");
  debug(PS4.Touchpad());
  PS4Data data = {
    .lStickY = PS4.LStickY(),
    .r1 = PS4.R1(),
    .r2 = PS4.R2(),
    .l1 = PS4.L1(),
    .l2 = PS4.L2(),
    .cross = PS4.Cross(),
    .circle = PS4.Circle(),
    .isPs4Connected = PS4.isConnected()
  };
  debug(data.lStickY);
  Wire.write(reinterpret_cast<uint8_t*>(&data), sizeof(data));
}