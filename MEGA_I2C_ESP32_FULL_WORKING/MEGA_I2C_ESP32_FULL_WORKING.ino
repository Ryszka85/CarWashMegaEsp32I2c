#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define debug(x) Serial.println(x)

#define LEFT_MOTOR_RIGHT_LIMIT 22
#define LEFT_MOTOR_LEFT_LIMIT 23
#define RIGHT_MOTOR_RIGHT_LIMIT 24
#define RIGHT_MOTOR_LEFT_LIMIT 25
#define FRONT_MOTOR_TOP_LIMIT 26

const int I2C_ADDRESS = 9;
const uint8_t SDA_PIN = 20; // choose a different pin for SDA
const uint8_t SCL_PIN = 21; // choose a different pin for SCL
const int I2C_CLOCK = 400000; // I2C clock frequency in Hz


struct PS4Controller {
  int8_t  lStickY;
  uint8_t r1;
  uint8_t r2;
  uint8_t l1;
  uint8_t l2;
  bool cross;
  bool circle;
  bool isPs4Connected;
};


Adafruit_MotorShield AFMS_TOP(0x63); 
Adafruit_DCMotor *frontMotor = AFMS_TOP.getMotor(1);
Adafruit_DCMotor *frontBrush = AFMS_TOP.getMotor(2);

Adafruit_DCMotor *groundMotor1 = AFMS_TOP.getMotor(3);
Adafruit_DCMotor *groundMotor2 = AFMS_TOP.getMotor(4);

Adafruit_MotorShield AFMS(0x60); 
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *leftBrush = AFMS.getMotor(2);
Adafruit_DCMotor *rightBrush = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(4);

bool startMotor4 = false;

void setup() {
  Serial.begin(115200); // start serial communication with Serial Monitor  

  AFMS.begin();
  AFMS_TOP.begin();

  Wire.begin();
  Wire.setClock(I2C_CLOCK);

  leftMotor->setSpeed(200);
  rightMotor->setSpeed(200);
  leftBrush->setSpeed(255);
  rightBrush->setSpeed(255);
  frontMotor->setSpeed(110);
  frontBrush->setSpeed(255);
  groundMotor1->setSpeed(255);
  groundMotor2->setSpeed(255);
  
  pinMode(LEFT_MOTOR_RIGHT_LIMIT, INPUT_PULLUP);
  pinMode(LEFT_MOTOR_LEFT_LIMIT, INPUT_PULLUP);
  pinMode(RIGHT_MOTOR_RIGHT_LIMIT, INPUT_PULLUP);
  pinMode(RIGHT_MOTOR_LEFT_LIMIT, INPUT_PULLUP);
  pinMode(FRONT_MOTOR_TOP_LIMIT, INPUT_PULLUP);  
  
}

void loop() {
  PS4Controller controllerData;
  controllerData.isPs4Connected = false;
  Wire.requestFrom(I2C_ADDRESS, sizeof(controllerData));

  while (Wire.available() < sizeof(controllerData)) {
    // Wait for data to become available
  }  

  Wire.readBytes(reinterpret_cast<uint8_t*>(&controllerData), sizeof(controllerData));

  if(controllerData.isPs4Connected) {
    handleBackMotors(controllerData);
    handleFrontMotors(controllerData);      
  }    
  delay(100);
}

void handleFrontMotors(PS4Controller ps4) {
  if(ps4.lStickY > 90) {
    stopOrRunMotor(FRONT_MOTOR_TOP_LIMIT,  frontMotor, FORWARD);
  } else if(ps4.lStickY < -90) {
    frontMotor->run(BACKWARD);
  } else {
    frontMotor->run(RELEASE);
  }

  if(ps4.l1) {
    debug("forward");
    groundMotor1->run(FORWARD);
    groundMotor2->run(FORWARD);
  } else if (ps4.l2) {
    debug("backward");
    groundMotor1->run(BACKWARD);
    groundMotor2->run(BACKWARD);
  } else {
    groundMotor1->run(RELEASE);
    groundMotor2->run(RELEASE);
  }

  if(ps4.circle) { frontBrush->run(FORWARD); } 
  else { frontBrush->run(RELEASE);  }
}

void handleBackMotors(PS4Controller ps4) {
  if(ps4.r1) {    
    stopOrRunMotor(LEFT_MOTOR_LEFT_LIMIT, leftMotor, FORWARD);
    stopOrRunMotor(RIGHT_MOTOR_LEFT_LIMIT, rightMotor, FORWARD);    
  } else if(ps4.r2) {
    stopOrRunMotor(LEFT_MOTOR_RIGHT_LIMIT, leftMotor, BACKWARD);
    stopOrRunMotor(RIGHT_MOTOR_RIGHT_LIMIT, rightMotor, BACKWARD);       
  } else {
    leftMotor->run(RELEASE); 
    rightMotor->run(RELEASE); 
  }

  if(ps4.cross) {
    leftBrush->run(FORWARD); 
    rightBrush->run(FORWARD); 
  } else {
    leftBrush->run(RELEASE); 
    rightBrush->run(RELEASE); 
  }
}

void stopOrRunMotor(int limitSwitch, Adafruit_DCMotor *motorParam, int direction) {
  if(digitalRead(limitSwitch) == LOW) { motorParam->run(RELEASE);  }
  else { motorParam->run(direction);  }
}


