// adds two color sensors
// uses timing to PID for UpdateWheels and UpdatePan
// spots, tracks, follows, grabs, moves, and releases block

#include <SPI.h>
#include <Pixy.h>
#include <Servo.h>
#include <Wire.h>
#include <SoftwareWire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Adafruit_TCS34725softi2c.h"
#include "Adafruit_VL53L0X.h"

Pixy pixy;            // main Pixy object

#define WHITE  45      // white LED
#define RED_R  46      // right red LED 
#define YELLOW_R  48   // right yellow LED
#define GREEN_R  42    // right green LED
#define BLUE_R  44     // right blue LED
#define RED_L  47      // left red LED
#define YELLOW_L  49   // left yellow LED
#define GREEN_L  43    // left green LED
#define BLUE_L  53     // left blue LED
#define SDApinL  3     // SDA for left color sensor
#define SCLpinL  4     // SCL for left color sensor
#define SDApinR  5     // SDA for right color sensor
#define SCLpinR  6     // SCL for right color sensor
#define PAN_SERVO  11  // camera panning servo
#define R_ARM  12      // right arm servo
#define L_ARM  13      // left arm servo
#define R_WHEEL  14    // right wheel servo
#define L_WHEEL  15    // left wheel servo
#define PAN_MIN  250 // 250 (-100 mapped) far LEFT; 350 (0 mapped) is STRAIGHT
#define PAN_MAX  450 // 450 (100 mapped) far RIGHT 
#define L_ARM_MIN  350 // 350 is OPEN (0 mapped)
#define L_ARM_MAX  375 // 375 is CLOSED (100 mapped)
#define R_ARM_MIN  328 // 328 is CLOSED (100 mapped)
#define R_ARM_MAX  355 // 355 is OPEN (0 mapped)
#define L_WHEEL_MIN  238 // 238 (100 mapped) full FORWARD; 360 is STOPPED (0 mapped)
#define L_WHEEL_MAX  482 // 482 (-100 mapped) full REVERSE;
#define R_WHEEL_MIN  238 // 238 (-100 mapped) full REVERSE; 360 is STOPPED (0 mapped)
#define R_WHEEL_MAX  482 // 482 (100 mapped) full FORWARD,



const int camSP = 160,     // pixy cam setpoint for pan PID
          camInterval = 20,
          wheelInterval = 20,
          servoSP = 0,
          scanInterval = 20,
          LEFT = 1,
          RIGHT = 2;

unsigned long current, lastScanUpdate, lastCamUpdate, lastWheelUpdate;

double trackTimeChange, trackPrevTime, trackError, trackErrSum, trackPrevErr, trackdErr, servoAdj;
double followTimeChange, followPrevTime, followError, followErrSum, followPrevErr, followdErr, wheelAdj;
double TOF_distance, raw_dist, prev_dist, temp_dist;

float trackKp = 0.08, // 0.08 works well
      trackKi = 0.0,  // 0
      trackKd = 0.1,  // 0.1 works well
      followKp = 0.8, // 0.8 works well  1.6
      followKi = 0.0, // 0
      followKd = 0.2; // 0.2 works well 0.8

int LC_gray, LR_gray, LG_gray, LB_gray,
    RC_gray, RR_gray, RG_gray, RB_gray,
    j, maxJ,
    prod, maxProd,
    max_Y, maxWidth, maxHeight,
    max_X = camSP,
    maxSig = 6,
    quadrant = 0,
    tempQuad = 0,
    prevQuad,
    homeQuad = 0,
    servoPos = 0,
    servoMin = -100,
    servoMax = 100,
    speedL = 0.0,
    speedR = 0.0,
    minSpeed = -100,
    maxSpeed = 100,
    baseSpeedL = 50,
    baseSpeedR = 50,
    increment = 8,
    initial = 1;


/* initialize L and R color sensors with specific integration time, gain values and custom SDA / SCL pin */
Adafruit_TCS34725softi2c tcsL = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X, SDApinL, SCLpinL);
Adafruit_TCS34725softi2c tcsR = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X, SDApinR, SCLpinR);

/* initialize time of flight (distance) sensor */
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); // Range: 50-1200mm

/* initialize servo driver */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {

  pinMode(WHITE, OUTPUT);
  pinMode(RED_L, OUTPUT);
  pinMode(YELLOW_L, OUTPUT);
  pinMode(GREEN_L, OUTPUT);
  pinMode(BLUE_L, OUTPUT);
  pinMode(RED_R, OUTPUT);
  pinMode(YELLOW_R, OUTPUT);
  pinMode(GREEN_R, OUTPUT);
  pinMode(BLUE_R, OUTPUT);

  Serial.begin(115200);
  tcsL.begin();  // initializes left color sensor
  tcsR.begin();  // initializes right color sensor
  pixy.init();

  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  // power
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);

  // sets servo initial positions
  UpdatePan(0);
  UpdateArms(0, 0);
  UpdateWheels(0, 0);
  delay(1000);

  digitalWrite(RED_L, HIGH);    // LED
  digitalWrite(RED_R, HIGH);    // LED
  delay(250);
  digitalWrite(YELLOW_L, HIGH);    // LED
  digitalWrite(YELLOW_R, HIGH);    // LED
  digitalWrite(RED_L, LOW);    // LED
  digitalWrite(RED_R, LOW);    // LED
  delay(250);
  digitalWrite(GREEN_L, HIGH);    // LED
  digitalWrite(GREEN_R, HIGH);    // LED
  digitalWrite(YELLOW_L, LOW);    // LED
  digitalWrite(YELLOW_R, LOW);    // LED
  delay(250);
  digitalWrite(BLUE_L, HIGH);    // LED
  digitalWrite(BLUE_R, HIGH);    // LED
  digitalWrite(GREEN_L, LOW);    // LED
  digitalWrite(GREEN_R, LOW);    // LED
  delay(250);
  digitalWrite(WHITE, HIGH);    // LED
  digitalWrite(BLUE_L, LOW);    // LED
  digitalWrite(BLUE_R, LOW);    // LED
  delay(500);
  digitalWrite(WHITE, LOW);    // LED
  delay(250);
}


void loop() {

ColorSensor();  
UpdateWheels(baseSpeedL,baseSpeedR);

//  TOF();
//  if ((TOF_distance < 90) && (TOF_distance > 50)) {
//    Grab();
//    Release();
//  }
//  
//  CheckBlocks();
//  BlockColor();


}



