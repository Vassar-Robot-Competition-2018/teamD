
#include <SPI.h>
#include <Pixy.h>
#include <Servo.h>

Pixy pixy;
Servo servoL;         // left motor
Servo servoR;         // right motor
Servo camServo;       // create servo object to control the Pixy cam

//Declares variables
int QRE1113_Pin = 0,  // connect to analog pin 0
    QRE_Value = 0,    // White line sensor value
    pos = 0,          // variable to store the servo position
    dir = 0,          // dictates direction of 'camServo' rotation
    sweep = 0,
    currentSize = 0,  // area of current signature
    bigSize = 0,      // Area of biggest signature detected
    bigSig = 0,       // Biggest color signature detected
    bigX = 0,         // X-coordinate of biggest signature
    bigY = 0,         // Y-coordinate of biggest signature
    minSize = 600,     // Minimum color signature to be considered
    state = 1,
    j,
    maxProd,
    prod,
    maxJ,
    maxSig,
    max_X,
    max_Y,
    minWidth = 100,
    maxWidth,
    maxHeight;

double speedL = 0,
       speedR = 0;
const int LED = 5,          // Assigns White LED to pin 5
          Red = 8,          // Assigns Red LED to pin 8
          Yellow = 9,       // Assigns Yellow LED to pin 9
          Green = 10,       // Assigns Green LED to pin 10
          Blue = 11;        // Assigns Blue LED to pin 8

void setup() {
  Serial.begin(9600);

  servoL.attach(3);  // attaches left servo on pin 50
  servoR.attach(2);  // attaches left servo on pin 51
  //  camServo.attach(12);  // attaches the servo on pin 12 to the servo object

  pinMode(LED, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);

  pixy.init();
}

/****************************************************************************************************/

void loop() {
     if (CheckWhiteLine () == 2) {
    WhiteLine ();
    }
    DriveStraight ();
    CheckBlocks ();
}

/******************************************************************************************************/

void motor_update(double L, double R) {
  double servoL_speed = (L * (-0.9)) + 90;
  double servoR_speed = (R * 0.9 ) + 91;
  servoL.write(servoL_speed);
  servoR.write(servoR_speed);
}

void DriveStraight () {
  digitalWrite(LED, LOW);
  speedL = 50.00;         // forward at half speed
  speedR = 50.00;

  motor_update(speedL, speedR);
}

int CheckWhiteLine () {
  QRE_Value = analogRead(QRE1113_Pin);
  Serial.println(QRE_Value);  // prints line sensor value to serial monitor
  if (QRE_Value <= 100) {
    return 2;
  }
  return 3;
}


void WhiteLine () {
  digitalWrite(LED, HIGH);  // turns on LED
  digitalWrite(Red, HIGH);
  digitalWrite(Yellow, HIGH);
  digitalWrite(Green, HIGH);
  digitalWrite(Blue, HIGH);
  speedL = -50;         // reverse at half speed
  speedR = -50;

  motor_update(speedL, speedR);
  delay(1000);              // time in milliseconds

  speedL = -50;         // reverse at half speed
  speedR = 50;

  motor_update(speedL, speedR);
  delay(2000);              // time in milliseconds
}


void CheckBlocks () {
  static int i = 0;
  uint16_t blocks;
  char buf[32];

  blocks = pixy.getBlocks();

  if (blocks) //if something is detected
  {
    for (j = 0; j < blocks; j++) { //find the largest object that fits the signature
      prod = pixy.blocks[j].width * pixy.blocks[j].height;
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
      Serial.println("Red");
      digitalWrite(Red, HIGH);    // LED
      digitalWrite(Yellow, LOW);    // LED
      digitalWrite(Green, LOW);    // LED
      digitalWrite(Blue, LOW);    // LED
      motor_update(0,0);
      delay(3000);
    }

    else if (maxSig == 2) {
      Serial.println("Yellow");
      digitalWrite(Red, LOW);    // LED
      digitalWrite(Yellow, HIGH);    // LED
      digitalWrite(Green, LOW);    // LED
      digitalWrite(Blue, LOW);    // LED
    }

    else if (maxSig == 3) {
      Serial.println("Green");
      digitalWrite(Red, LOW);    // LED
      digitalWrite(Yellow, LOW);    // LED
      digitalWrite(Green, HIGH);    // LED
      digitalWrite(Blue, LOW);    // LED
    }

    else if (maxSig == 4) {
      Serial.println("Blue");
      digitalWrite(Red, LOW);    // LED
      digitalWrite(Yellow, LOW);    // LED
      digitalWrite(Green, LOW);    // LED
      digitalWrite(Blue, HIGH);    // LED
    }
  }

  else {
    maxSig = 0;
    max_X = 0;
    max_Y = 0;
    maxWidth = 0;
    maxHeight = 0;

    Serial.println("None");
    digitalWrite(Red, LOW);    // LED
    digitalWrite(Yellow, LOW);    // LED
    digitalWrite(Green, LOW);    // LED
    digitalWrite(Blue, LOW);    // LED
  }

  Serial.print("Block Position: (");
  Serial.print(max_X, max_Y);
  Serial.println(")");
  Serial.print("Block Dimensions: ");
  Serial.print(maxWidth);
  Serial.print(" x ");
  Serial.println(maxWidth);



  maxJ = 0;
  maxSig = 0;
  max_X = 0;
  max_Y = 0;
  maxWidth = 0;
  maxHeight = 0;

  delay(30);
}





