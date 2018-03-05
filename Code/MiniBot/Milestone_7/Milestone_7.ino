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


#define IR_FRONT A2   // front IR sensor; acurate from 7-35 cm 
#define IR_LEFT A3    // left IR sensor
#define IR_RIGHT A4   // right IR sensor
#define WHITE 45      // white LED
#define RED_R 46      // right red LED 
#define YELLOW_R 48   // right yellow LED
#define GREEN_R 50    // right green LED
#define BLUE_R 52     // right blue LED
#define RED_L 47      // left red LED
#define YELLOW_L 49   // left yellow LED
#define GREEN_L 51    // left green LED
#define BLUE_L 53     // left blue LED



unsigned long current;
double timeChange, prevTime, error, errSum, prevErr, dErr, wheelAdj;

//Declares constants
const int RED = 2,          // assigns Red LED to pin 1
          YELLOW = 3,       // assigns Yellow LED to pin 2
          GREEN = 4,        // assigns Green LED to pin 3
          BLUE = 5,         // assigns Blue LED to pin 4
          WHITE = 6,        // assigns White LED to pin 5
          CAPTURED = 10,    // short IR distance for recognizing captured block
          sp = 160;     // pixy cam setpoint for pan PID

float kp = 0.5,
      ki = 0.0,
      kd = 0.1,

      double frontIR, leftIR, rightDist;

int LC_gray,
    Rgray,
    Ggray,
    Bgray,
    j,
    maxJ,
    prod,
    maxProd,
    maxSig = 6,
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
  //    Serial.println(frontIR);

  if ((frontIR < CAPTURED) && (maxSig != 6)) { //Block Captured
    digitalWrite(WHITE, HIGH);    // LED
    DriveStop();
    delay (4000);
  }

  if (frontIR > CAPTURED) {
    digitalWrite(WHITE, LOW);    // LED
    CheckBlocks();
    
    if (leftIR < 35) {
      BlockLeft();
    }
    if (right < 35) {
      BlockRight();
    }
  }

  else {
    CheckBlocks();
  }
}
