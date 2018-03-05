// This program uses the Pixy Cam to determine the largest signature block in view, prints the color,
// size and coordinates of that signature to the serial monitor, and lights up the corresponding
// colored LED.

/*
   Fields within the pixy.blocks[] array:
   pixy.blocks[i].signature The signature number of the detected object (1-7 for normal signatures)
   pixy.blocks[i].x The x location of the center of the detected object (0 to 319)
   pixy.blocks[i].y The y location of the center of the detected object (0 to 199)
   pixy.blocks[i].width The width of the detected object (1 to 320)
   pixy.blocks[i].height The height of the detected object (1 to 200)
   pixy.blocks[i].angle The angle of the object detected object if the detected object is a color code.
   pixy.blocks[i].print() A member function that prints the detected object information to the serial port
*/

#include <SPI.h>
#include <Pixy.h>
#include <Servo.h>

Servo servoL;         // left motor
Servo servoR;         // right motor
Servo servoCam;

// This is the main Pixy object
Pixy pixy;

int currentSize = 0,
    bigSize = 0,      // Area of biggest signature detected
    bigSig = 0,       // Biggest color signature detected
    bigX = 0,         // X-coordinate of biggest signature
    bigY = 0,         // Y-coordinate of biggest signature
    minSize = 70,     // Minimum color signature to be considered
    Red = 8,          // Assigns Red LED to pin 8
    Yellow = 9,       // Assigns Yellow LED to pin 9
    Green = 10,       // Assigns Green LED to pin 10
    Blue = 11;        // Assigns Blue LED to pin 8
int curr = 0;

int QRE1113_Pin = 0;  // connect to analog pin 0
int LED = 5;          // LED
int QRE_Value = 0;    // line sensor value

void setup()
{
  pinMode(Red, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);

  Serial.begin(9600);
  servoL.attach(50);  // attaches left servo on pin 50
  servoR.attach(51);  // attaches left servo on pin 51
  servoCam.attach(12);
  pinMode(LED, OUTPUT);

  Serial.print("Starting...\n");


  pixy.init();
}

void loop()
{
  servoCam.write(90);

  //  while (curr == 0)
  //  {
  QRE_Value = analogRead(QRE1113_Pin);
  //Serial.println(QRE_Value);  // prints line sensor value to serial monitor

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

  //    static int i = 0;
  //    int j;
  //    uint16_t blocks;
  //    char buf[32];

  //    blocks = pixy.getBlocks();  // Returns the number of objects Pixy has detected
  //
  //    if (blocks)                 // If there are blocks detected...
  //    {
  //      i++;
  //
  //      // do this (print) every 50 frames because printing every
  //      // frame would bog down the Arduino
  //      if (i % 50 == 0)
  //      {
  //        //   sprintf(buf, "Detected %d:\n", blocks);
  //        //   Serial.print(buf);                          // Prints number of color signatures detected
  //        for (j = 0; j < blocks; j++)
  //        {
  //          // sprintf(buf, "  block %d: ", j);
  //          // Serial.print(buf);
  //          //  pixy.blocks[j].print();                    // Prints information on each detected signature
  //
  //          currentSize = ((pixy.blocks[j].width) * (pixy.blocks[j].height));
  //
  //
  //          // Updates the biggest signature variables if the current signature is bigger than the previous biggest signature
  //          if (currentSize > bigSize)
  //          {
  //            bigSize = currentSize;
  //            bigSig = pixy.blocks[j].signature;
  //            bigX = pixy.blocks[j].x,
  //            bigY = pixy.blocks[j].y;
  //          }
  //        }
  //
  //        Serial.print("Block Color: ");
  //
  //        //Turns on the colored LED that matches the biggest signature
  //        if (bigSig == 1)     // if biggest signature is red
  //        {
  //          Serial.println("Red");
  //     //     digitalWrite(Red, HIGH);    // LED
  //          digitalWrite(Yellow, LOW);    // LED
  //          digitalWrite(Green, LOW);    // LED
  //          digitalWrite(Blue, LOW);    // LED
  //          curr = 1;
  //        }
  //        if (bigSig == 2)    // if biggest signature is yellow
  //        {
  //          Serial.println("Yellow");
  //       //   digitalWrite(Yellow, HIGH);    // LED
  //          digitalWrite(Red, LOW);    // LED
  //          digitalWrite(Green, LOW);    // LED
  //          digitalWrite(Blue, LOW);    // LED
  //          curr = 2;
  //        }
  //        if (bigSig == 3)    // if biggest signature is green
  //        {
  //          Serial.println("Green");
  //       //   digitalWrite(Green, HIGH);    // LED
  //          digitalWrite(Red, LOW);    // LED
  //          digitalWrite(Yellow, LOW);    // LED
  //          digitalWrite(Blue, LOW);    // LED
  //          curr = 3;
  //        }
  //        if (bigSig == 4)    // if biggest signature is blue
  //        {
  //          Serial.println("Blue");
  //       //   digitalWrite(Blue, HIGH);
  //          digitalWrite(Red, LOW);
  //          digitalWrite(Yellow, LOW);
  //          digitalWrite(Green, LOW);
  //          curr = 4;
  //        }
  //
  //        Serial.print("Block Size: ");
  //        Serial.println(bigSize);
  //        Serial.print("Coordinates: (");
  //        Serial.print(bigX);
  //        Serial.print(", ");
  //        Serial.print(bigY);
  //        Serial.println(")");
  //        Serial.println();
  //
  //        if (bigSize < minSize)
  //        {
  //          digitalWrite(Red, LOW);    // LED
  //          digitalWrite(Yellow, LOW);    // LED
  //          digitalWrite(Green, LOW);    // LED
  //          digitalWrite(Blue, LOW);    // LED
  //        }
  //
  //        // Resets the biggest signature variables
  //        currentSize = 0;
  //        bigSize = 0;
  //        bigSig = 0;
  //        bigX = 0;
  //        bigY = 0;
  // }
  //}
  //}
  //
  //servoL.write(90);        // backwards stop
  //servoR.write(91);
  //delay(1000);
  //
  ////Turns on the colored LED that matches the biggest signature
  //if (curr == 1)     // if biggest signature is red
  //{
  //  Serial.println("Red");
  //  digitalWrite(Red, HIGH);    // LED
  //  digitalWrite(Yellow, LOW);    // LED
  //  digitalWrite(Green, LOW);    // LED
  //  digitalWrite(Blue, LOW);    // LED
  //}
  //if (curr == 2)    // if biggest signature is yellow
  //{
  //  Serial.println("Yellow");
  //  digitalWrite(Yellow, HIGH);    // LED
  //  digitalWrite(Red, LOW);    // LED
  //  digitalWrite(Green, LOW);    // LED
  //  digitalWrite(Blue, LOW);    // LED
  //}
  //if (curr == 3)    // if biggest signature is green
  //{
  //  Serial.println("Green");
  //  digitalWrite(Green, HIGH);    // LED
  //  digitalWrite(Red, LOW);    // LED
  //  digitalWrite(Yellow, LOW);    // LED
  //  digitalWrite(Blue, LOW);    // LED
  //}
  //if (curr == 4)    // if biggest signature is blue
  //{
  //  Serial.println("Blue");
  //  digitalWrite(Blue, HIGH);
  //  digitalWrite(Red, LOW);
  //  digitalWrite(Yellow, LOW);
  //  digitalWrite(Green, LOW);
  //}
}
