#include <SPI.h>
#include <Pixy.h>
#include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Pixy pixy;          // main Pixy object
Servo servoL;       // left motor
Servo servoR;       // right motor
Servo blockServoL;  // left block servo
Servo blockServoR;  // right block servo
Servo camServo;     // pixy servo

//Declares constants
const int IR_SHORT = A1,    // A41 has shorter range: 4-30cm
          IR_FRONT = A2,    // A21 has longer range: 10-80cm
          IR_LEFT = A3,     // Left IR 10-80cm
          IR_RIGHT = A4,    // Right IR 10-80cm
          RED = 2,          // assigns Red LED to pin 1
          YELLOW = 3,       // assigns Yellow LED to pin 2
          GREEN = 4,        // assigns Green LED to pin 3
          BLUE = 5,         // assigns Blue LED to pin 4
          WHITE = 6,        // assigns White LED to pin 5
          captured = 12,    // short IR distance for recognizing captured block
          minDist = 45,     // min IR distance for CheckBlock and wheel PID
          minWidth = 20,    // minimum color block width for CheckBlock and wheel PID
          camSP = 160,     // pixy cam setpoint for pan PID
          servoSP = 90,     // servo setpoint for wheel PID
          panMin = 0,       // min pixy servo position
          panMax = 180,     // max pixy servo position
          servoMin = 45,
          servoMax = 135,
          scanUpdateInterval = 10;

float baseSpeedL = 50,
      baseSpeedR = 50,

      panKp = 0.025,
      panKi = 0.0,
      panKd = 0.3,
      prevCamError = 0, // previous error for pan PID
      camErrSum = 0,
      trackLastTime = 0,
      panAdjust = 0,
      panPos = 90,

      lastScanUpdate = 0,

      wheelKp = 0.5,
      wheelKi = 0.0,
      wheelKd = 0.3,
      wheelAdj = 0,




      lastPos = 90,
      servoError,
      lastServoError,

      irShort,
      irFront,
      irLeft,
      irRight,
      irShortDist = 100,
      irFrontDist = 100,
      irLeftDist = 100,
      irRightDist = 100;

int tracking = 0,
    scanning = 0,

    Cgray,
    Rgray,
    Ggray,
    Bgray,

    j,
    maxJ,
    prod,
    maxProd,
    maxSig,
    max_X,
    max_Y,
    maxWidth = 0,
    increment = 5,
    maxHeight,
    lineWidth = 230,
    lineHeight = 150,
    quadrant = 0,
    quadCheck = 0,
    prev_quadrant,
    homeQ = 0,
    speedL = 0.0,
    speedR = 0.0,
    minSpeed = 0,
    maxSpeed = 100;

unsigned long current = millis();
double trackTimeChange;
double camError;
double camdErr;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  servoL.attach(8);  // attaches left wheel servo on pin 8
  servoR.attach(9);  // attaches right wheel servo on pin 9
  blockServoL.attach(10); // attaches left block servo on pin 10
  blockServoR.attach(11); // attaches right block servo on pin 11
  camServo.attach(12);  // attaches the servo on pin 12 to the servo object

  // sets servo initial positions

  blockServoL.write(45);
  blockServoR.write(135);
  servoL.write(92);
  servoR.write(92);
  camServo.write(panPos);

  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  Serial.begin(9600);
  tcs.begin();
  pixy.init();
}


//void loop() {
//  ColorSensor(); // includes ColorCalibration, WhiteCheck, and Quadrant functions
//  IR_Short();
//  IR_Front();
//
//  if (irShortDist > captured) {
//
//    CheckBlocks();
//    //    StateCheck();
//  }
//
//  else
//  {
//    // IR_Front();
//    DriveForward();
//    digitalWrite(WHITE, LOW);    // LED
//    //  BlockComplete();
//  }
//}

void loop() {
  camServo.write(panPos);
  delay(5000);

  while (1 == 1) {
    CheckBlocks();
    //Scan();
  }
}

