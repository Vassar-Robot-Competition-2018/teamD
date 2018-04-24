
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
const int IR_SHORT = A1,    // A41 has shorter range: 4-30cm
          CAPTURED = 10,    // short IR distance for recognizing captured block
          wheelInterval = 20; // time between PID updates
unsigned long current, lastWheelUpdate;
double timeChange, prevTime, camError, errSum, prevErr, dErr, wheelAdj;

float kp = 0.3,
      ki = 0.0,
      kd = 0.1,
      irShort,
      ShortIR;

int LC_gray, LR_gray, LG_gray, LB_gray,
    RC_gray, RR_gray, RG_gray, RB_gray,
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
    maxWidth,
    maxHeight,
    quadrant = 0,
    tempQuad = 0,
    prevQuad,
    homeQuad = 5,
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
    initial = 1,
    camSP = 160;      // pixy cam setpoint for PID;


/* initialize L and R color sensors with specific integration time, gain values and custom SDA / SCL pin */
Adafruit_TCS34725softi2c tcsL = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X, SDApinL, SCLpinL);
Adafruit_TCS34725softi2c tcsR = Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X, SDApinR, SCLpinR);

/* initialize servo driver */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  servoL.attach(8);  // attaches left wheel servo on pin 8
  servoR.attach(9);  // attaches right wheel servo on pin 9
  //  blockServoL.attach(11); // attaches left block servo on pin 22
  //  blockServoR.attach(10); // attaches right block servo on pin 23

  pinMode(RED_F, OUTPUT);
  pinMode(YELLOW_F, OUTPUT);
  pinMode(GREEN_F, OUTPUT);
  pinMode(BLUE_F, OUTPUT);
  pinMode(RED_B, OUTPUT);
  pinMode(YELLOW_B, OUTPUT);
  pinMode(GREEN_B, OUTPUT);
  pinMode(BLUE_B, OUTPUT);

  Serial.begin(115200);
  tcsL.begin();  // initializes left color sensor
  tcsR.begin();  // initializes right color sensor
  pixy.init();

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);

  // sets servo initial positions
  //  MotorUpdate(0, 0);
  //  UpdatePan(0);
  //UpdateArms(100, 100);
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

  //ColorSensor();
}


void loop() {
  CheckBlocks();
}

void CheckBlocks() {
  // for (int k = 0; k < 10; k++) {
  static int i = 0;
  uint16_t blocks;
  char buf[32];

  maxJ = 0;
  maxSig = 6;
  max_X = camSP;
  max_Y = 0;
  maxWidth = 0;
  maxHeight = 0;
  prod = 0;
  maxProd = 0;
  blocks = pixy.getBlocks();

  if (blocks) { //if a color sig is detected by pixy cam
    for (j = 0; j < blocks; j++) { //find the largest signature
      prod = pixy.blocks[j].width * pixy.blocks[j].height;
      // if (prod > maxProd) {
      if ((pixy.blocks[j].height > 20) && (pixy.blocks[j].height < 125)) {
        if ((pixy.blocks[j].height > 20) && (pixy.blocks[j].height < 125))
          //  maxProd = prod;
          maxJ = j;
        maxSig = pixy.blocks[maxJ].signature;
        max_X = pixy.blocks[maxJ].x;
        max_Y = pixy.blocks[maxJ].y;
        maxHeight = pixy.blocks[maxJ].height;
        maxWidth = pixy.blocks[maxJ].width;

        Serial.println("Block Height: ");
        Serial.print(maxHeight);



        
        Serial.print("         ");
        Serial.print("Block Width: ");
        Serial.println(maxWidth);

      }
    }

  }
}


