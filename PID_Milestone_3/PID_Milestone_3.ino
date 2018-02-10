#include <SPI.h>
#include <Pixy.h>
#include <Servo.h>

Pixy pixy;        // main Pixy object
Servo servoL;     // left motor
Servo servoR;     // right motor
Servo camServo;   // pixy servo

//Declares constants
const int QRE_Pin = 0,      // connects line sensor to analog pin 0
          White = 5,        // assigns White LED to pin 5
          Red = 8,          // assigns Red LED to pin 8
          Yellow = 9,       // assigns Yellow LED to pin 9
          Green = 10,       // assigns Green LED to pin 10
          Blue = 11,        // assigns Blue LED to pin 8
          minWidth = 100,   // minimum color block width
          stopWidth = 500,  // width of block to end program (block found)
          pixySP = 160,     // pixy cam setpoint
          servoSP = 90;     // servo setpoint

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

int QRE_Val = 0,      // white line sensor value
    state = 0,
    j,
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
    wheelAdj,
    dir = 0,
    sweep = 0;


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
  while (maxWidth < stopWidth) {
    CheckFloor();
    StateCheck();
    CheckBlocks();
    PixyPID();
    wheelPID();
  }

  BlockComplete();
}


void CheckFloor() {
  QRE_Val = analogRead(QRE_Pin);
  if (QRE_Val < 100) {
    digitalWrite(White, HIGH); //turns on white LED

    DriveReverse();
    delay(1000);

    RightTurn();
    delay(2000);
  }
}


void MotorUpdate(double L, double R) {
  double servoL_speed = (L * (-0.9)) + 90;
  double servoR_speed = (R * 0.9 ) + 91;
  servoL.write(servoL_speed);
  servoR.write(servoR_speed);
}


void StateCheck() {
  if (state == 0) { // if searching for block
    DriveForward();
    Sweep();
  }
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
    state = 1;

    BlockColor();
  }

  else {
    //Serial.println("None");
    digitalWrite(Red, LOW);    // LED
    digitalWrite(Yellow, LOW);    // LED
    digitalWrite(Green, LOW);    // LED
    digitalWrite(Blue, LOW);    // LED
    state = 0;
    StateCheck();
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

void BlockColor () {
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

  MotorUpdate(speedL, speedR);
}


void DriveForward() {
  speedL = 50;
  speedR = 50;
  MotorUpdate(speedL, speedR);
}


void DriveReverse() {
  speedL = -50;
  speedR = -50;
  MotorUpdate(speedL, speedR);
}


void LeftTurn() {
  speedL = -50;
  speedR = 50;
  MotorUpdate(speedL, speedR);
}


void RightTurn() {
  speedL = 50;
  speedR = -50;
  MotorUpdate(speedL, speedR);
}


void DriveStop() {
  speedL = 0;
  speedR = 0;
  MotorUpdate(speedL, speedR);
}


void Sweep() {
  if (dir == 0)   // dir 0 sweeps from left to right
  {
    pos += 2;
    camServo.write(pos);
    delay(10);
    if (pos == 180)
    {
      dir = 1;
    }
  }

  else if (dir == 1) // dir 1 sweeps from right to left
  {
    pos -= 2;
    camServo.write(pos);
    delay(10);
    if (pos == 0)
    {
      dir = 0;
    }
  }
}


void BlockComplete() {
  speedL = 0;
  speedR = 0;
  MotorUpdate(speedL, speedR);
  BlockColor();
}

