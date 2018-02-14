// This program is for a servo attached to the Pixy Cam, causing the camera to sweep back
// and forth in search of colored blocks. 

#include <Servo.h>

Servo camServo;  // create servo object to control a servo

int pos = 0,    // variable to store the servo position
    dir = 0;    // dictates direction of 'camServo' rotation

void setup() {
  camServo.attach(9);  // attaches the servo on pin 12 to the servo object
}

void loop() {

  if (dir == 0)   // dir 0 sweeps from left to right
  {
    pos += 1;
    camServo.write(pos);
   // delay(10);
    if (pos == 180)
    {
      dir = 1;
    }
  }
  else if (dir == 1) // dir 1 sweeps from right to left
  {
    pos -= 1;
    camServo.write(pos);
    // delay(10);
    if (pos == 0)
    {
      dir = 0;
    }
  }
}

