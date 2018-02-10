#include <SPI.h>
#include <Pixy.h>
#include <Servo.h>


Pixy pixy;        // This is the main Pixy object
Servo servoL;     // left motor
Servo servoR;     // right motor
Servo camServo;   // create servo object to control a servo

//Declares constants
const int White = 5,          // Assigns White LED to pin 5
          Red = 8,          // Assigns Red LED to pin 8
          Yellow = 9,       // Assigns Yellow LED to pin 9
          Green = 10,       // Assigns Green LED to pin 10
          Blue = 11,        // Assigns Blue LED to pin 8
          minWidth = 100,
          pixySP = 160,
          servoSP = 90;

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

int j,
    maxJ,
    prod,
    maxProd,
    maxSig,
    max_X,
    max_Y,
    maxWidth,
    maxHeight,
    pixyError,
    lastPixyError = 0,
    pos = 90,    // variable to store the servo position
    lastPos = 90,
    servoAdj,
    servoError,
    lastServoError,
    wheelAdj;

void setup() {
  servoL.attach(2);  // attaches left servo on pin 2
  servoR.attach(3);  // attaches right servo on pin 3
  camServo.attach(12);  // attaches the servo on pin 12 to the servo object
  camServo.write(90);
  delay(1000);

  pinMode(Red, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);

  Serial.begin(9600);

  pixy.init();

}


void loop() {
  CheckBlocks();
  PixyPID();
  wheelPID();
}


void motor_update(double L, double R) {
  double servoL_speed = (L * (-0.9)) + 90;
  double servoR_speed = (R * 0.9 ) + 91;
  servoL.write(servoL_speed);
  servoR.write(servoR_speed);
}


void CheckBlocks() {
  static int i = 0;
  uint16_t blocks;
  char buf[32];

  maxJ = 0;
  maxSig = 0;
  max_X = 0;
  max_Y = 0;
  maxWidth = 0;
  maxHeight = 0;

  blocks = pixy.getBlocks();

  if (blocks) //if something is detected
  {
    for (j = 0; j < blocks; j++) { //find the largest object that fits the signature
      prod = pixy.blocks[j].width * pixy.blocks[j].height;
      //     skip = 0;
      if (prod > maxProd)
        maxJ = j;
    }
  }

  if (pixy.blocks[maxJ].x > minWidth) {
    maxSig = pixy.blocks[maxJ].signature;
    max_X = pixy.blocks[maxJ].x;
    max_Y = pixy.blocks[maxJ].y;
    maxWidth = pixy.blocks[maxJ].width;
    maxHeight = pixy.blocks[maxJ].height;

    if (maxSig == 1) {
      //Serial.println("Red");
      digitalWrite(Red, HIGH);    // LED
      digitalWrite(Yellow, LOW);    // LED
      digitalWrite(Green, LOW);    // LED
      digitalWrite(Blue, LOW);    // LED
    }

    else if (maxSig == 2) {
      //Serial.println("Yellow");
      digitalWrite(Red, LOW);    // LED
      digitalWrite(Yellow, HIGH);    // LED
      digitalWrite(Green, LOW);    // LED
      digitalWrite(Blue, LOW);    // LED
    }

    else if (maxSig == 3) {
      //Serial.println("Green");
      digitalWrite(Red, LOW);    // LED
      digitalWrite(Yellow, LOW);    // LED
      digitalWrite(Green, HIGH);    // LED
      digitalWrite(Blue, LOW);    // LED
    }

    else if (maxSig == 4) {
      //Serial.println("Blue");
      digitalWrite(Red, LOW);    // LED
      digitalWrite(Yellow, LOW);    // LED
      digitalWrite(Green, LOW);    // LED
      digitalWrite(Blue, HIGH);    // LED
    }
  }

  else {
    //Serial.println("None");
    digitalWrite(Red, LOW);    // LED
    digitalWrite(Yellow, LOW);    // LED
    digitalWrite(Green, LOW);    // LED
    digitalWrite(Blue, LOW);    // LED
  }

  //Serial.print("Block Position: (");
  //Serial.print(max_X, max_Y);
  //Serial.println(")");
  //Serial.print("Block Dimensions: ");
  //Serial.print(maxWidth);
  //Serial.print(" x ");
  //Serial.println(maxWidth);

  delay(30);
}

void PixyPID() {
  pixyError = pixySP - max_X;
  //Serial.print(pixyError: ";
  //Serial.println(pixyError);

  servoAdj = pixyKp * pixyError + pixyKd * (pixyError - lastPixyError);
  lastPixyError = pixyError;
  pos = lastPos + servoAdj;
  lastPos = pos;

  if (pos > 180)
    pos = 180;

  else if (pos < 0)
    pos = 0;

  camServo.write(pos);
  delay(20);
}


void wheelPID() {
  servoError = servoSP - pos;
  //Serial.print("servoError: ");
  //Serial.println(servoError);

  wheelAdj = wheelKp * servoError + wheelKd * (servoError - lastServoError);
  lastServoError = servoError;
  speedL = baseSpeedL - wheelAdj;   // not sure yet if it's L minus adjust or R minus adjust
  speedR = baseSpeedR + wheelAdj;

  if (speedL > maxSpeedL)
    speedL = maxSpeedL;

  else if (speedL < minSpeedL)
    speedL = minSpeedL;

  if (speedR > maxSpeedR)
    speedR = maxSpeedR;

  else if (speedR < minSpeedR)
    speedR = minSpeedR;

  motor_update(speedL, speedR);
}

