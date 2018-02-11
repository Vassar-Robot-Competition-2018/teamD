#include <SPI.h>
#include <Pixy.h>
#include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Pixy pixy;        // main Pixy object
Servo servoL;     // left motor
Servo servoR;     // right motor
Servo camServo;   // pixy servo

//Declares constants
const int QRE_Pin = A0,      // connects line sensor to analog pin 0
          IR_SHORT = A1,   //A41 has shorter range: 4-30cm
          IR_LONG = A2,    //A21 has longer range: 10-80cm
          White = 5,        // assigns White LED to pin 5
          Red = 8,          // assigns Red LED to pin 8
          Yellow = 9,       // assigns Yellow LED to pin 9
          Green = 10,       // assigns Green LED to pin 10
          Blue = 11,        // assigns Blue LED to pin 8
          minWidth = 100,   // minimum color block width
          pixySP = 160,     // pixy cam setpoint
          servoSP = 90;     // servo setpoint

double speedL = 0.0,
       speedR = 0.0,
       baseSpeedL = 50,
       baseSpeedR = 50,
       minSpeedL = 0,
       minSpeedR = 0,
       maxSpeedL = 100,
       maxSpeedR = 100,
       pixyKp = 0.05,
       pixyKi = 0.0,
       pixyKd = 0.02,
       wheelKp = 0.1,
       wheelKi = 0.0,
       wheelKd = 0.0;

float irShort,
      irShortDist,
      irLong,
      irLongDist;

int QRE_Val = 0,      // white line sensor value
    state = 0,
    j,
    maxJ,
    prod,
    maxProd,
    maxSig,
    max_X,
    max_Y,
    maxWidth = 0,
    maxHeight,
    pixyError,
    lastPixyError = 0,
    pos = 90,    // variable to store the servo position
    lastPos = 90,
    servoAdj,
    servoError,
    lastServoError,
    wheelAdj,
    dir = 0,
    sweep = 0,
    quadrant;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  servoL.attach(2);  // attaches left servo on pin 2
  servoR.attach(3);  // attaches right servo on pin 3
  camServo.attach(12);  // attaches the servo on pin 12 to the servo object
  camServo.write(90);
  delay(1000);

  pinMode(Red, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);

  Serial.begin(9600);
  tcs.begin();
  pixy.init();
}


void loop() {
  while (state == 0) {
    CheckFloor();
    StateCheck();
    ColorSensor();
    CheckBlocks();
  }

  BlockComplete();
}

