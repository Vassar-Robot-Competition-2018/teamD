// 9.38 introduces differential speed based on error for both following and heading 

#include <SPI.h>
#include <Pixy.h>
#include <Servo.h>
#include <LSM303.h>
#include <Wire.h>
#include <SoftwareWire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Adafruit_TCS34725softi2c.h"
#include "Adafruit_VL53L0X.h"

Pixy pixy;          // main Pixy object
Servo servoL;       // left motor
Servo servoR;       // right motor
LSM303 compass;

#define WHITE  43      // white LED
#define RED_F 47      // right red LED 
#define YELLOW_F  49   // right yellow LED
#define GREEN_F  33    // right green LED
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
#define L_WHEEL  4    // left wheel servo
#define R_WHEEL  5    // right wheel servo
#define PAN_MIN  225 // 250 (-100 mapped) far LEFT; 350 (0 mapped) is STRAIGHT FWD
#define PAN_MAX  450 // 450 (100 mapped) far RIGHT 
#define L_ARM_MIN  340 // 350 is OPEN (0 mapped)
#define L_ARM_MAX  362 // 375 is CLOSED (100 mapped)
#define R_ARM_MIN  314 // 328 is CLOSED (100 mapped)
#define R_ARM_MAX  342 // 355 is OPEN (0 mapped)
#define L_WHEEL_MIN  223 // 238 (100 mapped) full FORWARD; 360 is STOPPED (0 mapped)
#define L_WHEEL_MAX  467 // 482 (-100 mapped) full REVERSE;
#define R_WHEEL_MIN  223 // 238 (-100 mapped) full REVERSE; 360 is STOPPED (0 mapped)
#define R_WHEEL_MAX  467 // 482 (100 mapped) full FORWARD,


//Declares constants

const int camSP = 160,     // pixy cam setpoint for pan PID
          servoSP = 0,
          camInterval = 20,
          wheelInterval = 20,
          scanInterval = 20,
          blockInterval = 20,
          releaseInterval = 3000,
          TOFinterval = 50,
          colorInterval = 20,
          headingInterval = 20;

unsigned long current, lastScanUpdate, lastCamUpdate, lastWheelUpdate, lastBlockUpdate, lastTOFupdate, lastColorUpdate, lastHeadingUpdate, tempTimer;

double trackTimeChange, trackError, trackPrevErr, trackdErr, servoAdj;
double followTimeChange, followError, followPrevErr, followdErr, wheelAdj;
double avoidTimeChange, avoidError, avoidPrevErr, avoiddErr,
double headingTimeChange, headingError, headingPrevErr, headingdErr;
double TOF_distance, raw_dist, prev_dist, temp_dist;

float trackKp = 0.08, // 0.08 works well
      trackKd = 0.1,  // 0.1 works well
      followKp = 0.8, // 0.8 works well  1.6
      followKd = 0.2, // 0.2 works well 0.8
      headingKp = 0.7,
      headingKd = 0.0,
      heading = 400,
      course = 360,
      North, East, South, West;
      

int LC_gray, LR_gray, LG_gray, LB_gray,
    RC_gray, RR_gray, RG_gray, RB_gray,

    j, maxJ, prod, maxProd,
    maxSig = 6,
    max_X = camSP,
    maxWidth, maxHeight,

    quadrant = 0,
    homeQuad = 5,
    tempQuad = 0,
    prevQuad, quadL, quadR, newSide, oldQuad,

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
    increment = 6,
    servoMin = -100,
    servoMax = 100,
    speedL = 0.0,
    speedR = 0.0,
    minSpeed = -100,
    maxSpeed = 100,
    baseSpeedL = 80,
    baseSpeedR = 80,
    highSpeed = 100,
    differentialSpeed,

    Status = 0;


/* initialize L and R color sensors with specific integration time, gain values and custom SDA / SCL pin */
Adafruit_TCS34725softi2c tcsL = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X, SDApinL, SCLpinL);
Adafruit_TCS34725softi2c tcsR = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X, SDApinR, SCLpinR);

/* initialize servo driver */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/* initialize time of flight (distance) sensor */
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); // Range: 50-1200mm

