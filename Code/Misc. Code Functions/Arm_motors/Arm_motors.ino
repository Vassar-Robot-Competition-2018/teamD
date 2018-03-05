#include <Servo.h>

Servo blockServoL;  // left block servo
Servo blockServoR;  // right block servo

void setup() {
 blockServoL.attach(22); // attaches left block servo on pin 10
  blockServoR.attach(23); // attaches right block servo on pin 11
    blockServoL.write(94); //starts at 94
  blockServoR.write(100); //starts at 100
  Serial.begin(9600);
}

void loop() {
GrabBlock();
ReleaseBlock();
}

void GrabBlock() {
  blockServoL.write(67);
  blockServoR.write(120);
  delay(3000);
}
void ReleaseBlock() {
  blockServoL.write(94);
  blockServoR.write(100);
  delay(3000);
}

