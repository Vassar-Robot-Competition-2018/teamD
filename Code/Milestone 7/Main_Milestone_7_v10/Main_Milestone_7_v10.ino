//doesn't set homeQuad yet?

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


unsigned long current;
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
          camSP = 160;     // pixy cam setpoint for pan PID

float kp = 0.5,
      ki = 0.0,
      kd = 0.1,

      irShort,
      irPixy,
      irLeft,
      irRight,
      irShortDist,
      irPixyDist,
      irLeftDist,
      irRightDist;

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
    homeQuad = 0,
    speedL = 0.0,
    speedR = 0.0,
    minSpeed = 0,
    maxSpeed = 100,
    baseSpeedL = 50,
    baseSpeedR = 50;


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  servoL.attach(8);  // attaches left wheel servo on pin 8
  servoR.attach(9);  // attaches right wheel servo on pin 9
  // blockServoL.attach(10); // attaches left block servo on pin 10
  // blockServoR.attach(11); // attaches right block servo on pin 11

  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  Serial.begin(9600);
  tcs.begin();
  pixy.init();

  // sets servo initial positions

  // blockServoL.write(45);
  // blockServoR.write(135);
  servoL.write(92);
  servoR.write(92);
  delay(2000);
}


void loop() {
  ColorSensor();
  IR_Check();

  //    Serial.print("IR Distance: ");
  //    Serial.println(irShortDist);

  if (((irShortDist < CAPTURED) || (irPixyDist < 15)) && (maxSig != 6)) { //Block Captured 
    digitalWrite(WHITE, HIGH);    // LED
    DriveStop();
    delay (4000);
  }
  
  if (irShortDist > CAPTURED) {
    digitalWrite(WHITE, LOW);    // LED
    CheckBlocks();
    //if (irPixyDist < 25) {
     // IR_Pixy();
     // if (irPixyDist < 25) {
        //IR_Pixy();
       // if (irPixyDist < 25) {
          //CheckBlocks();
       // }
     // }
    }
    
/*    else if ((irLeftDist < 20) && (irLeftDist > 10)) {
      IR_Left();
      if (irLeftDist < 20) {
        IR_Left();
        if (irLeftDist < 20) {
          DriveStop();
          delay(100);
          LeftTurn();
          delay(800);
          CheckBlocks();
        }
      }
    }
   else if ((irRightDist < 20) && (irRightDist > 10)) {
      IR_Right();
      if (irRightDist < 20) {
        IR_Right();
        if (irRightDist < 20) {
          DriveStop();
          delay(100);
          RightTurn();
          delay(800);
          CheckBlocks();
        }
      }
    }
    else CheckBlocks();
  }*/
  else {
    CheckBlocks();
  }
}




