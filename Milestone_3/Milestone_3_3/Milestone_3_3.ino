// This program causes the robot to drive forward until the downward-pointing QRE1113 line sensor detects a white surface.
// This causes the robot to drive backwards for 1 second, stop for 1 second, and then resume going forward. An LED also
// lights up when a white surface is detected.

#include <Servo.h>
#include <SPI.h>
#include <Pixy.h>

Servo servoL;         // left motor
Servo servoR;         // right motor
Servo servoCam;

// This is the main Pixy object
Pixy pixy;

int QRE1113_Pin = 0;  // connect to analog pin 0
int LED = 5;          // LED
int QRE_Value = 0;    // line sensor value
int currentSize = 0,
    bigSize = 0,      // Area of biggest signature detected
    bigSig = 0,       // Biggest color signature detected
    bigX = 0,         // X-coordinate of biggest signature
    bigY = 0,         // Y-coordinate of biggest signature
    minSize = 70,     // Minimum color signature to be considered
    Red = 8,          // Assigns Red LED to pin 8
    Yellow = 9,       // Assigns Yellow LED to pin 9
    Green = 10,       // Assigns Green LED to pin 10
    Blue = 11,        // Assigns Blue LED to pin 8
    curr = 0;


void setup() {
  Serial.begin(9600);
  servoL.attach(50);  // attaches left servo on pin 50
  servoR.attach(51);  // attaches left servo on pin 51
  servoCam.attach(12);
  pinMode(LED, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);

  pixy.init();
}

void loop() {
  servoCam.write(90);

  while (curr == 0)
  {

    QRE_Value = analogRead(QRE1113_Pin);
    Serial.println(QRE_Value);  // prints line sensor value to serial monitor

    // values less than 100 are treated as white
    if (QRE_Value >= 100) {     // no white detected
      digitalWrite(LED, LOW);
      servoL.write(45);         // forward at half speed
      servoR.write(136);
    }

    else {  // white detected
      digitalWrite(LED, HIGH);  // turns on LED
      servoL.write(135);        // backwards at half speed
      servoR.write(46);
      delay(1000);              // time in milliseconds
      servoL.write(135);        // backwards at half speed
      servoR.write(136);
      delay(2000);              // time in milliseconds
      //    servoL.write(90);         // motors stop
      //    servoR.write(91);
      //    delay(1000);              // time in milliseconds
    }

    static int i = 0;
    int j;
    uint16_t blocks;
    char buf[32];

    blocks = pixy.getBlocks();  // Returns the number of objects Pixy has detected

    if (blocks)                 // If there are blocks detected...
    {
      i++;

      // do this (print) every 50 frames because printing every
      // frame would bog down the Arduino
      if (i % 50 == 0)
      {
        //   sprintf(buf, "Detected %d:\n", blocks);
        //   Serial.print(buf);                          // Prints number of color signatures detected
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

        Serial.print("Block Color: ");

        //Turns on the colored LED that matches the biggest signature
        if (bigSig == 1)     // if biggest signature is red
        {
          Serial.println("Red");
          digitalWrite(Red, HIGH);    // LED
          digitalWrite(Yellow, LOW);    // LED
          digitalWrite(Green, LOW);    // LED
          digitalWrite(Blue, LOW);    // LED
        }
        if (bigSig == 2)    // if biggest signature is yellow
        {
          Serial.println("Yellow");
          digitalWrite(Yellow, HIGH);    // LED
          digitalWrite(Red, LOW);    // LED
          digitalWrite(Green, LOW);    // LED
          digitalWrite(Blue, LOW);    // LED
        }
        if (bigSig == 3)    // if biggest signature is green
        {
          Serial.println("Green");
          digitalWrite(Green, HIGH);    // LED
          digitalWrite(Red, LOW);    // LED
          digitalWrite(Yellow, LOW);    // LED
          digitalWrite(Blue, LOW);    // LED
        }
        if (bigSig == 4)    // if biggest signature is blue
        {
          Serial.println("Blue");
          digitalWrite(Blue, HIGH);
          digitalWrite(Red, LOW);
          digitalWrite(Yellow, LOW);
          digitalWrite(Green, LOW);
        }

        Serial.print("Block Size: ");
        Serial.println(bigSize);
        Serial.print("Coordinates: (");
        Serial.print(bigX);
        Serial.print(", ");
        Serial.print(bigY);
        Serial.println(")");
        Serial.println();

        if (bigSize < minSize)
        {
          digitalWrite(Red, LOW);    // LED
          digitalWrite(Yellow, LOW);    // LED
          digitalWrite(Green, LOW);    // LED
          digitalWrite(Blue, LOW);    // LED
        }

        // Resets the biggest signature variables
        currentSize = 0;
        bigSize = 0;
        bigSig = 0;
        bigX = 0;
        bigY = 0;
      }
    }
  }
}
