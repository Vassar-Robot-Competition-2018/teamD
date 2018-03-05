#include <SPI.h>
#include <Pixy.h>
#include <Servo.h>


Pixy pixy;        // This is the main Pixy object

Servo camServo;   // create servo object to control a servo

//Declares constants
const int White = 5,          // Assigns White LED to pin 5
          Red = 8,          // Assigns Red LED to pin 8
          Yellow = 9,       // Assigns Yellow LED to pin 9
          Green = 10,       // Assigns Green LED to pin 10
          Blue = 11,        // Assigns Blue LED to pin 8
          minWidth = 100,
          setpoint = 160;

double Kp = 0.05,
       Ki = 0.0,
       Kd = 0.02;

int j,
    maxJ,
    prod,
    maxProd,
    maxSig,
    max_X,
    max_Y,
    maxWidth,
    maxHeight,
    error,
    lastError = 0,
    pos = 90,    // variable to store the servo position
    lastPos = 90,
    servoAdj;

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

void setup() {
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

void PixyPID() {
  error = setpoint - max_X;
  Serial.println(error);

  servoAdj = Kp * error + Kd * (error - lastError);
  lastError = error;
  pos = lastPos + servoAdj;
  lastPos = pos;

  if (pos > 180)
    pos = 180;

  else if (pos < 0)
    pos = 0;

  camServo.write(pos);
  delay(20);
}

void loop() {
  CheckBlocks();
  Serial.println(max_X);
  PixyPID();
}

