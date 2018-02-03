// This program uses the Pixy Cam to determine the largest signature block in view, prints the color,
// size and coordinates of that signature to the serial monitor, and lights up the corresponding
// colored LED. If a color signature is not detected, a servo causes the pixy to sweep back and forth
// until a color signature is detected.

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

// This is the main Pixy object
Pixy pixy;

Servo camServo;  // create servo object to control a servo

int pos = 0,    // variable to store the servo position
    dir = 0,    // dictates direction of 'camServo' rotation
    sweep = 0,
    currentSize = 0,  // area of current signature
    bigSize = 0,      // Area of biggest signature detected
    bigSig = 0,       // Biggest color signature detected
    bigX = 0,         // X-coordinate of biggest signature
    bigY = 0,         // Y-coordinate of biggest signature
    minSize = 600,     // Minimum color signature to be considered
    Red = 8,          // Assigns Red LED to pin 8
    Yellow = 9,       // Assigns Yellow LED to pin 9
    Green = 10,       // Assigns Green LED to pin 10
    Blue = 11;        // Assigns Blue LED to pin 8

String color = "None";

void setup()
{
  camServo.attach(12);  // attaches the servo on pin 12 to the servo object

  pinMode(Red, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);

  Serial.begin(9600);
  Serial.print("Starting...\n");

  pixy.init();
}

void loop()
{
  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32];

  blocks = pixy.getBlocks();  // Returns the number of objects Pixy has detected

  if (blocks)                 // If there are blocks detected...
  {
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

    if (bigSize > minSize)
    {
      // Turns on the colored LED that matches the biggest signature
      if (bigSig == 1)     // if biggest signature is red
      {
        color = "Red";
        digitalWrite(Red, HIGH);    // LED
        digitalWrite(Yellow, LOW);    // LED
        digitalWrite(Green, LOW);    // LED
        digitalWrite(Blue, LOW);    // LED
      }
      if (bigSig == 2)    // if biggest signature is yellow
      {
        color = "Yellow";
        digitalWrite(Yellow, HIGH);    // LED
        digitalWrite(Red, LOW);    // LED
        digitalWrite(Green, LOW);    // LED
        digitalWrite(Blue, LOW);    // LED
      }
      if (bigSig == 3)    // if biggest signature is green
      {
        color = "Green";
        digitalWrite(Green, HIGH);    // LED
        digitalWrite(Red, LOW);    // LED
        digitalWrite(Yellow, LOW);    // LED
        digitalWrite(Blue, LOW);    // LED
      }
      if (bigSig == 4)    // if biggest signature is blue
      {
        color = "Blue";
        digitalWrite(Blue, HIGH);
        digitalWrite(Red, LOW);
        digitalWrite(Yellow, LOW);
        digitalWrite(Green, LOW);
      }
    }
  }
  
  else {
    Serial.println("None");
    digitalWrite(Green, LOW);    // LED
    digitalWrite(Red, LOW);    // LED
    digitalWrite(Yellow, LOW);    // LED
    digitalWrite(Blue, LOW);    // LED

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

    Serial.print("Block Color: ");
    Serial.println(color);
    Serial.print("Block Size: ");
    Serial.println(bigSize);
    Serial.print("Coordinates: (");
    Serial.print(bigX);
    Serial.print(", ");
    Serial.print(bigY);
    Serial.println(")");
    Serial.println();

  // Resets the biggest signature variables
  currentSize = 0;
  bigSize = 0;
  bigSig = 0;
  bigX = 0;
  bigY = 0;
  color = "None";
}

