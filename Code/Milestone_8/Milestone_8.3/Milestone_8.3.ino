
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

#define WHITE  43      // white LED
#define RED_F 47      // right red LED 
#define YELLOW_F  49   // right yellow LED
#define GREEN_F  32    // right green LED
#define BLUE_F  53     // right blue LED
#define RED_B  35      // left red LED
#define YELLOW_B  37   // left yellow LED
#define GREEN_B  39    // left green LED
#define BLUE_B  41     // left blue LED


unsigned long current, lastWheelUpdate;
double timeChange, prevTime, camError, errSum, prevErr, dErr, wheelAdj;

//Declares constants
const int IR_SHORT = A1,    // A41 has shorter range: 4-30cm
          CAPTURED = 10,    // short IR distance for recognizing captured block
          camSP = 160,      // pixy cam setpoint for PID
          wheelInterval = 20; // time between PID updates


float kp = 0.3,
      ki = 0.0,
      kd = 0.1,
      irShort,
      ShortIR;

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

  pinMode(RED_F, OUTPUT);
  pinMode(YELLOW_F, OUTPUT);
  pinMode(GREEN_F, OUTPUT);
  pinMode(BLUE_F, OUTPUT);
  pinMode(RED_B, OUTPUT);
  pinMode(YELLOW_B, OUTPUT);
  pinMode(GREEN_B, OUTPUT);
  pinMode(BLUE_B, OUTPUT);

  Serial.begin(9600);
  tcs.begin();
  pixy.init();

  // sets servo initial positions

  //blockServoL.write(94); //L starts at 94
  //blockServoR.write(100); //R starts at 100
  servoL.write(92);
  servoR.write(92);
  delay(2000);

  digitalWrite(RED_B, HIGH);
delay(100);
digitalWrite(RED_B, LOW);
digitalWrite(YELLOW_B, HIGH);
delay(100);
digitalWrite(YELLOW_B, LOW);
digitalWrite(GREEN_B, HIGH);
delay(100);
digitalWrite(GREEN_B, LOW);
digitalWrite(BLUE_B, HIGH);
delay(100);
digitalWrite(BLUE_B, LOW);
digitalWrite(WHITE, HIGH);
delay(100);
digitalWrite(WHITE, LOW);
digitalWrite(RED_F, HIGH);
delay(100);
digitalWrite(RED_F, LOW);
digitalWrite(YELLOW_F, HIGH);
delay(100);
digitalWrite(YELLOW_F, LOW);
digitalWrite(GREEN_F, HIGH);
delay(100);
digitalWrite(GREEN_F, LOW);
digitalWrite(BLUE_F, HIGH);
delay(100);
digitalWrite(BLUE_F, LOW);
delay(1000);
}


void loop() {
  ColorSensor();

  if (Captured()) { //Block Captured
    DriveForward();
    delay (1000);
    DriveStop();
    digitalWrite(GREEN_B, HIGH);
    digitalWrite(RED_B, LOW);

    delay (3000);
    //GrabBlock();
    // digitalWrite(WHITE, HIGH);
    while (quadrant != homeQuad) {
      ColorSensor();
      DriveForward();
    }
    DriveStop();
    digitalWrite(GREEN_B, LOW);
    digitalWrite(RED_B, HIGH);
    delay (4000);
  }
  else {
    CheckBlocks();
    digitalWrite(RED_B, LOW);
    digitalWrite(YELLOW_B, LOW);
    digitalWrite(GREEN_B, LOW);
    digitalWrite(BLUE_B, LOW);
  }
}
