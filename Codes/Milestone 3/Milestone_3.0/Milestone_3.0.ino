// This program causes the robot to drive forward until the downward-pointing QRE1113 line sensor detects a white surface.
// This causes the robot to drive backwards for 1 second, stop for 1 second, and then resume going forward. An LED also
// lights up when a white surface is detected.
// The Pixy Cam sweeps back and forth searching for a signature block. Once a signature block is detected,
// the Pixy Cam uses a servo to continue tracking the block.

#include <SPI.h>
#include <Pixy.h>
#include <Servo.h>

Pixy pixy;
Servo servoL;         // left motor
Servo servoR;         // right motor
Servo camServo;       // create servo object to control the Pixy cam

//Declares functions
void DriveStraight ();
int CheckWhiteLine ();
void WhiteLine ();
void Sweep ();
int CheckBlocks ();
void BlockColor ();
void Track ();

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
    state = 1;
double speedL = 0,
       speedR = 0;

const int LED = 5,          // Assigns White LED to pin 5
          Red = 8,          // Assigns Red LED to pin 8
          Yellow = 9,       // Assigns Yellow LED to pin 9
          Green = 10,       // Assigns Green LED to pin 10
          Blue = 11,        // Assigns Blue LED to pin 8
          TRUE = 1,
          FALSE = 0;

String color = "None";

void setup() {
  Serial.begin(9600);

  servoL.attach(3);  // attaches left servo on pin 50
  servoR.attach(2);  // attaches left servo on pin 51
  camServo.attach(12);  // attaches the servo on pin 12 to the servo object

  pinMode(LED, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);

  pixy.init();
}

/**************************************************/

void loop() {
  while (state == 1) {
    if (CheckWhiteLine () == TRUE) {
      WhiteLine ();
    }
    if (CheckBlocks () == TRUE) {   // Turns on the colored LED that matches the biggest signature
      BlockColor ();
      motor_update(0, 0);
      state = 0;
      //Track ();
    }
    else {
      Sweep ();
      DriveStraight ();
    }
  }
}

/**************************************************/

void motor_update(double L, double R) {
  double servoL_speed = (L * -0.9) + 90;
  double servoR_speed = (R * 0.9) + 90;
  servoL.write(servoL_speed);
  servoR.write(servoR_speed);
}

void DriveStraight () {
  digitalWrite(LED, LOW);
  speedL = 50;         // forward at half speed
  speedR = 50;

  motor_update(speedL, speedR);
}

int CheckWhiteLine () {
  QRE_Value = analogRead(QRE1113_Pin);
  //Serial.println(QRE_Value);  // prints line sensor value to serial monitor
  if (QRE_Value >= 100) {
    return TRUE;
  }
}


void WhiteLine () {
  digitalWrite(LED, HIGH);  // turns on LED
  speedL = -50;         // reverse at half speed
  speedR = -50;

  motor_update(speedL, speedR);
  delay(1000);              // time in milliseconds

  speedL = -50;         // reverse at half speed
  speedR = 50;

  motor_update(speedL, speedR);
  delay(2000);              // time in milliseconds
}


int CheckBlocks () {
  currentSize = 0;
  bigSize = 0;
  bigSig = 0;
  bigX = 0;
  bigY = 0;
  color = "None";

  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32];
  blocks = pixy.getBlocks();  // Returns the number of objects Pixy has detected
  if (blocks) {                 // If there are blocks detected...
    for (j = 0; j < blocks; j++)
    {
      // sprintf(buf, "  block %d: ", j);
      // Serial.print(buf);
      //  pixy.blocks[j].print();                    // Prints information on each detected signature
      currentSize = ((pixy.blocks[j].width) * (pixy.blocks[j].height));
      // Updates the biggest signature variables if the current signature is bigger than the previous biggest signature
      if (currentSize > bigSize)
      {
        bigSize = currentSize;
        bigSig = pixy.blocks[j].signature;
        bigX = pixy.blocks[j].x,
        bigY = pixy.blocks[j].y;
      }
    }
    if (bigSize > minSize) {
      return TRUE;
    }
  }
}


void Sweep () {
  if (dir == 0)   // dir 0 sweeps from left to right
  {
    pos += 1;
    camServo.write(pos);
    delay(10);
    if (pos == 180)
    {
      dir = 1;
    }
  }
  else if (dir == 1) // dir 1 sweeps from right to left
  {
    pos -= 1;
    camServo.write(pos);
    delay(10);
    if (pos == 0)
    {
      dir = 0;
    }
  }
}


void BlockColor () {
  if (bigSig == 1) {     // if biggest signature is red
    color = "Red";
    digitalWrite(Red, HIGH);    // LED
    digitalWrite(Yellow, LOW);    // LED
    digitalWrite(Green, LOW);    // LED
    digitalWrite(Blue, LOW);    // LED
  }
  if (bigSig == 2) {   // if biggest signature is yellow
    color = "Yellow";
    digitalWrite(Yellow, HIGH);    // LED
    digitalWrite(Red, LOW);    // LED
    digitalWrite(Green, LOW);    // LED
    digitalWrite(Blue, LOW);    // LED
  }
  if (bigSig == 3) {   // if biggest signature is green
    color = "Green";
    digitalWrite(Green, HIGH);    // LED
    digitalWrite(Red, LOW);    // LED
    digitalWrite(Yellow, LOW);    // LED
    digitalWrite(Blue, LOW);    // LED
  }
  if (bigSig == 4) {   // if biggest signature is blue
    color = "Blue";
    digitalWrite(Blue, HIGH);
    digitalWrite(Red, LOW);
    digitalWrite(Yellow, LOW);
    digitalWrite(Green, LOW);
  }
  delay (1000);
}

void Track () {
  if ((bigX - 160) > 0) // if block is on right side of cam view, servo should rotate right (toward 0)
  {
    if (pos > 0)
    {
      pos -= 3;
      camServo.write(pos);
    }
  }
  else if ((bigX - 160) < 0) // if block is on left side of cam view, servo should rotate left (toward 180)
  {
    if (pos < 180)
    {
      pos += 3;
      camServo.write(pos);
    }
  }
}

