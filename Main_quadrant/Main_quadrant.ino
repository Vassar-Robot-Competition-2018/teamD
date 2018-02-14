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
const int QRE_Pin = A0,     // connects line sensor to analog pin 0
          IR_SHORT = A1,    // A41 has shorter range: 4-30cm
          IR_LONG = A2,     // A21 has longer range: 10-80cm
          RED = 2,          // assigns Red LED to pin 1
          YELLOW = 3,       // assigns Yellow LED to pin 2
          GREEN = 4,        // assigns Green LED to pin 3
          BLUE = 5,         // assigns Blue LED to pin 4
          WHITE = 6,        // assigns White LED to pin 5
          minWidth = 25,    // minimum color block width
          stopWidth = 40,   // block width required for completion of milestone 3
          stopDist = 35,    // IR distance required for completion of milestone 3
          pixySP = 160,     // pixy cam setpoint
          servoSP = 90,     // servo setpoint
          servoMin = 45,    // min pixy servo position
          servoMax = 135;   // max pixy servo position

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

int Cgray,
    Rgray,
    Ggray,
    Bgray,
    QRE_Val = 0,      // white line sensor value
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
    quadrant = 0,
    quadCheck = 0,
    prev_quadrant,
    homeQ = 0;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  servoL.attach(8);  // attaches left wheel servo on pin 8
  servoR.attach(9);  // attaches right wheel servo on pin 9
  blockServoL.attach(10); // attaches left block servo on pin 10
  blockServoR.attach(11); // attaches right block servo on pin 11
  camServo.attach(12);  // attaches the servo on pin 12 to the servo object

  // sets servo initial positions
  camServo.write(90);
  blockServoL.write(45);
  blockServoR.write(135);
  delay(1000);

  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  Serial.begin(9600);
  tcs.begin();
  pixy.init();
}


void loop() {
  //while (state == 0) {
    ColorSensor(); // includes ColorCalibration, WhiteCheck, and Quadrant functions
    //CheckBlocks();
    StateCheck();
  }

  //BlockComplete();
//}

