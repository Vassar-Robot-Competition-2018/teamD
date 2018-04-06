
#include <SPI.h>
#include <Pixy.h>
#include <Servo.h>
#include <Wire.h>
#include <SoftwareWire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Adafruit_TCS34725softi2c.h"

Pixy pixy;          // main Pixy object
Servo servoL;       // left motor
Servo servoR;       // right motor
//Servo blockServoL;  // left block servo
//Servo blockServoR;  // right block servo
//Servo camServo;     // pixy servo

#define WHITE  43      // white LED
#define RED_F 47      // right red LED 
#define YELLOW_F  49   // right yellow LED
#define GREEN_F  32    // right green LED
#define BLUE_F  53     // right blue LED
#define RED_B  35      // left red LED
#define YELLOW_B  37   // left yellow LED
#define GREEN_B  39    // left green LED
#define BLUE_B  41     // left blue LED
#define SDApinL  3     // SDA for left color sensor
#define SCLpinL  2     // SCL for left color sensor
#define SDApinR  5     // SDA for right color sensor
#define SCLpinR  4     // SCL for right color sensor
#define L_ARM  0      // left arm servo
#define R_ARM  1      // right arm servo
#define PAN_SERVO  2  // camera panning servo
#define PAN_MIN  250 // 250 (-100 mapped) far LEFT; 350 (0 mapped) is STRAIGHT FWD
#define PAN_MAX  450 // 450 (100 mapped) far RIGHT 
#define L_ARM_MIN  368 // 350 is OPEN (0 mapped)
#define L_ARM_MAX  425 // 375 is CLOSED (100 mapped)
#define R_ARM_MIN  313 // 328 is CLOSED (100 mapped)
#define R_ARM_MAX  371 // 355 is OPEN (0 mapped)


//Declares constants
const int camSP = 160,      // pixy cam setpoint for PID;
          camInterval = 20,
          wheelInterval = 20,
          servoSP = 0,
          scanInterval = 20, // time between PID updates
          releaseInterval = 3000;

unsigned long current, lastScanUpdate, lastCamUpdate, lastWheelUpdate;

double trackTimeChange, trackPrevTime, trackError, trackErrSum, trackPrevErr, trackdErr, servoAdj;
double followTimeChange, followPrevTime, followError, followErrSum, followPrevErr, followdErr, wheelAdj;

float trackKp = 0.08, // 0.08 works well
      trackKi = 0.0,  // 0
      trackKd = 0.1,  // 0.1 works well
      followKp = 0.8, // 0.8 works well  1.6
      followKi = 0.0, // 0
      followKd = 0.2; // 0.2 works well 0.8

int LC_gray, LR_gray, LG_gray, LB_gray,
    RC_gray, RR_gray, RG_gray, RB_gray,

    j, maxJ,
    maxSig = 6, max_X = camSP, max_Y = 0,
    maxWidth, maxHeight,

    quadrant = 0, newQuad = 0,
    quadL, tempQuadL, prevQuadL,
    quadR, tempQuadR, prevQuadR,

    Home = 5, 
    Neutral_1 = 1, 
    Enemy = 2, 
    Neutral_2 = 3,

    HQ_count = 3,
    NQ1_count = 3,
    EQ_count = 2,
    NQ2_count = 3,
    captureB, captureQ, releaseQ,

    servoPos = 0,
    servoMin = -100,
    servoMax = 100,
    speedL = 0.0,
    speedR = 0.0,
    minSpeed = -100,
    maxSpeed = 100,
    baseSpeedL = 0,
    baseSpeedR = 0,
    increment = 9,
    initial = 1,
    Status = 0;


/* initialize L and R color sensors with specific integration time, gain values and custom SDA / SCL pin */
Adafruit_TCS34725softi2c tcsL = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X, SDApinL, SCLpinL); // def works with 50ms; also try boosting gain(4X)
Adafruit_TCS34725softi2c tcsR = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X, SDApinR, SCLpinR); // def works with 50ms; also try boosting gain(4x)

/* initialize servo driver */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


