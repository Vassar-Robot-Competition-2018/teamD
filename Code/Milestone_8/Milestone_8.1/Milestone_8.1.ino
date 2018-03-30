
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


unsigned long current, lastWheelUpdate;
double timeChange, prevTime, camError, errSum, prevErr, dErr, wheelAdj;

//Declares constants
const int IR_SHORT = A1,    // A41 has shorter range: 4-30cm
          IR_PIXY = A2,    // A21 has longer range: 10-80cm
          IR_LEFT = A3,     // Left IR 10-80cm
          IR_RIGHT = A4,    // Right IR 10-80cm
          RED = 2,          // assigns Red LED to pin 1
          YELLOW = 3,       // assigns Yellow LED to pin 2
          GREEN = 4,        // assigns Green LED to pin 3
          BLUE = 5,         // assigns Blue LED to pin 4
          WHITE = 6,        // assigns White LED to pin 5
          CAPTURED = 10,    // short IR distance for recognizing captured block
          camSP = 160,      // pixy cam setpoint for PID
          wheelInterval = 20; // time between PID updates


float kp = 0.8,
      ki = 0.0,
      kd = 0.2,

      irShort,
      irLeft,
      irRight,
      ShortIR,
      PixyIR,
      LeftIR,
      RightIR;

int Cgray,
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
    maxWidth,
    maxHeight,
    quadrant = 0,
    tempQuad = 0,
    prevQuad,
    homeQuad = 5,
    speedL = 0.0,
    speedR = 0.0,
    minSpeed = 0,
    maxSpeed = 100,
    baseSpeedL = 50,
    baseSpeedR = 50,
    cond = 0;


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  servoL.attach(8);  // attaches left wheel servo on pin 8
  servoR.attach(9);  // attaches right wheel servo on pin 9
  blockServoL.attach(11); // attaches left block servo on pin 22
  blockServoR.attach(10); // attaches right block servo on pin 23

  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  Serial.begin(9600);
  tcs.begin();
  pixy.init();

  // sets servo initial positions

  blockServoL.write(94); //L starts at 94
  blockServoR.write(100); //R starts at 100
  servoL.write(92);
  servoR.write(92);
  delay(2000);
}


void loop() {
  ColorSensor();
  IR_Check();
  CheckBlocks();

  if (((ShortIR < CAPTURED) || (PixyIR < 15)) && (maxSig != 6)) { //Block Captured
    if (cond == 0) {
      DriveForward();
      //      delay (1200);
      //      DriveStop();
      //      delay (300);
      //      GrabBlock();
      digitalWrite(GREEN, HIGH);
      delay(1200);
      digitalWrite(YELLOW, LOW);
      digitalWrite(RED, LOW);

      cond = 1;
    }
    if (cond == 1) {
      if (quadrant != homeQuad) {
        digitalWrite(GREEN, LOW);
        digitalWrite(YELLOW, HIGH);
        digitalWrite(RED, LOW);
        DriveForward();
      }
      else if (quadrant == homeQuad) {
        DriveForward();

        digitalWrite(GREEN, LOW);
        digitalWrite(YELLOW, LOW);
        digitalWrite(RED, HIGH);
        delay (1200);
        //        ReleaseBlock();
        //        delay (400);
        cond = 0;
      }
    }
  }
  else {
    CheckBlocks();
    //cond = 0;
  }
}


